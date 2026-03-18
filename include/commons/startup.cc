/*
   Copyright 2025 Anthony A. Constantinescu.

   Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
   in compliance with the License. You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software distributed under the License
   is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
   or implied. See the License for the specific language governing permissions and limitations under
   the License.

   File: commons/system.hh
   Description: This is to be included in exactly ONE c++ source file of the project using this library.
   It runs a few subroutines that are necessary to initialize the standard library.
*/

#pragma once

#include "system.hh"                   // IWYU pragma: keep
#include "config.hh"                   // IWYU pragma: keep
#include "runtime/globals.hh"          // IWYU pragmaL keep
#include "runtime/symbols.hh"          // IWYU pragma: keep
#include "runtime/instrumentation.cc"  // IWYU pragma: keep
#include "runtime/validator.cc"        // IWYU pragma: keep

#if __linux__
// Fun fact: any identifier starting with sa_ is reserved when <signal.h> is included!
#include <stdlib.h>
#include <malloc.h>
#endif


namespace cm {

///
/// Implementation of the panic function
///
[[noreturn]]
inline void ::cm::panic(char const* message, char const* reason, SourceLocation const& src)
{
    startup::panicMutex.lock();
    setIfNull(message, "");
    setIfNull(reason, "");

    auto write = [](char const* str, usize len) {
        kernel::call(kernel::write, kernel::stderr, str, len);
    };
    write("\x1B[31m", sizeof("\x1B[31m"));
    write(message, CArrays::stringLen(message));
    write(" in ", 4);
    write(getIfNull(src.function(), "<unknown function>"), CArrays::stringLen(src.function()));
    write((" at \""), 5);
    write(getIfNull(src.file(), "<unknown file>"), CArrays::stringLen(src.file()));
    write("\"", 1);
    Profiler::printStackTrace();
    write("\x1B[0m", sizeof("\x1B[0m"));
    startup::panicMutex.unlock();
    CPU::trap();
}


///
/// Global constructors
///


///
/// Adds a new precondition
///
void ::cm::addPreInitAssertion(PreInitAssertion const& assertion)
{
    if (startup::preInitAssertionsCount >= LibraryConfig::MAX_PRE_INIT_ASSERTIONS) {
        _emergencyPrint("Maximum number of PreInitAssertions exceeded\n");
        CPU::trap();
    }
    startup::preInitAssertions[startup::preInitAssertionsCount] = assertion;
    ++startup::preInitAssertionsCount;
}

///
/// Verifies all the preconditions are true before main starts.
///
[[gnu::constructor(LibraryConfig::GLOBAL_CTOR_PRECONDITION_CHECK_PRIO), gnu::no_instrument_function]]
void checkPreInitAssertions(usize max_fails = LibraryConfig::MAX_PRE_INIT_FAILURES)
{
    usize fails = 0;
    for (usize i = 0; i < startup::preInitAssertionsCount; i++) {
        auto const& t = startup::preInitAssertions[i];
        if (!t.test()) {
            console.out.println(
                R"(`test failed: "`" at "`:``")", LibraryConfig::CONSOLE_ERROR_HIGHLIGHT_BEGIN, t.name, t.file, t.line,
                LibraryConfig::CONSOLE_ERROR_HIGHLIGHT_END);
            ++fails;
            if (fails == max_fails) {
                break;
            }
        }
    }
    if (fails == 0) {
        console.out.println("All tests successful!");
    } else {
        console.out.println(
            "`Testing stopped with ` failure``", LibraryConfig::CONSOLE_ERROR_HIGHLIGHT_BEGIN, fails,
            fails == 1 ? "." : "s.", LibraryConfig::CONSOLE_ERROR_HIGHLIGHT_END);
        exit(-1);
    }
}

}  // namespace cm


constexpr auto DEFAULT_ALIGNMENT = static_cast<std::align_val_t>(8);


inline void* newImpl(std::size_t const size, std::align_val_t alignment)
{
    void* ptr;
    if (alignment == DEFAULT_ALIGNMENT) {
        ptr = malloc(size);
    } else {
        ptr = aligned_alloc(static_cast<size_t>(alignment), size);
    }
    cm::startup::memoryStats.bytesAllocated += size;
    ::cm::Assert(ptr);
    return ptr;
}

inline void* newImplNothrow(std::size_t const size, std::align_val_t alignment) noexcept
{
    void* ptr = aligned_alloc(static_cast<size_t>(alignment), size);
    ::cm::startup::memoryStats.bytesAllocated += size;
    ::cm::Assert(ptr);
    return ptr;
}

inline void deleteImpl(void* ptr, usize size = 1)
{
    VALIDATE_SIZED(static_cast<u8*>(ptr), size);
    auto sz = malloc_usable_size(ptr);
    free(ptr);
    if (sz > ::cm::startup::memoryStats.bytesAllocated) {
        sz = ::cm::startup::memoryStats.bytesAllocated;
    }
    ::cm::startup::memoryStats.bytesAllocated -= sz;
    (void)ptr;
}


//

void* operator new(usize size) { return newImpl(size, DEFAULT_ALIGNMENT); }

void* operator new[](usize size) { return newImpl(size, DEFAULT_ALIGNMENT); }

void* operator new(usize size, std::align_val_t al) { return newImpl(size, al); }

void* operator new[](usize size, std::align_val_t al) { return newImpl(size, al); }

void* operator new(usize size, std::nothrow_t const&) noexcept { return newImplNothrow(size, DEFAULT_ALIGNMENT); }

void* operator new[](usize size, std::nothrow_t const&) noexcept { return newImplNothrow(size, DEFAULT_ALIGNMENT); }

void* operator new(usize size, std::align_val_t al, std::nothrow_t const&) noexcept { return newImplNothrow(size, al); }

void* operator new[](usize size, std::align_val_t al, std::nothrow_t const&) noexcept
{
    return newImplNothrow(size, al);
}

//

void operator delete(void* ptr) noexcept { return deleteImpl(ptr); }

void operator delete[](void* ptr) noexcept { return deleteImpl(ptr); }

void operator delete(void* ptr, usize size) noexcept { return deleteImpl(ptr, size); }

void operator delete[](void* ptr, usize size) noexcept { return deleteImpl(ptr, size); }

extern "C" void __cxa_pure_virtual()  // NOLINT
{
    __builtin_trap();
}


#ifdef sa_handler
#undef sa_handler
#endif
