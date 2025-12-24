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

#include "system.hh"  // IWYU pragma: keep

#if __has_feature(address_sanitizer)
#include <sanitizer/asan_interface.h>
#include <sanitizer/common_interface_defs.h>
#elif __has_feature(dataflow_sanitizer)
#include <sanitizer/dfsan_interface.h>
#endif
#if __linux__
#include <signal.h>  // IWYU pragma: keep


inline u32 cm::FastPRNG::_state;
;
inline u32 cm::ND_PRNG::_state;
;


namespace cm {

[[noreturn]]
inline void ::cm::panic(char const* message, char const* reason, SourceLocation src)
{
    if (message == nullptr)
        message = "";

    if (reason == nullptr)
        reason = "";

    auto write = [](char const* str, usize len) {
        LinuxSyscall(LinuxSyscall.write, 2, u64(str), len);
    };
    write("\x1B[31m", sizeof("\x1B[31m"));
    write(message, CArrays::stringLen(message));

    if (src.function() != nullptr || src.file() != nullptr) {
        write((" in "), 4);
    }
    if (src.function() != nullptr) {
        write((src.function()), CArrays::stringLen(src.function()));
    }
    if (src.file() != nullptr) {
        write((" at \""), 5);
        write(src.file(), CArrays::stringLen(src.file()));
        write("\"", 1);
    }
    if (reason != nullptr) {
        write(".\n\treason: ", 11);
        write(reason, CArrays::stringLen(reason));
    }
    Profiler::printStackTrace();
    write("\x1B[0m", sizeof("\x1B[0m"));
    __builtin_trap();
    //_exit(-1);
}

inline void _emergencyPrint(char const* msg)
{
    auto write = [](char const* str, usize len) {
        LinuxSyscall(LinuxSyscall.write, 2, u64(str), len);
    };
    write(msg, CArrays::stringLen(msg));
}

}  // namespace cm
#endif


namespace cm {
namespace startup {

constexpr static auto MAX_PROFILER_STACK_FRAMES = 65_KB;
static Array<::cm::Profiler::StackFrame, MAX_PROFILER_STACK_FRAMES> s_stackFrames;
static unsigned int s_currentStackFrameIndex{};
static bool s_printed = false;
static Array<char, 1024> s_stackTraceBuffer;
}

[[gnu::no_instrument_function]]
inline void Profiler::printStackTrace()
{
    if (startup::s_printed)
        return;

    startup::s_printed = true;
    _emergencyPrint("\nStack Trace:\n");
#if __has_feature(address_sanitizer)
    __sanitizer_print_stack_trace();
    _emergencyPrint("\n");
#elif __has_feature(dataflow_sanitizer)
    dfsan_sprint_stack_trace(s_stackTraceBuffer, 1024);
    _emergencyPrint(s_stackTraceBuffer);
    _emergencyPrint("\x1B[0m\n");
#else
    //
#endif
    // Assert(s_currentStackFrameIndex < MAX_PROFILER_STACK_FRAMES, "Profiler: currentStackFrameIndex OOB");
    // io::_emergencyPrint("\nStack Trace:\n");

    // for (long long i = s_currentStackFrameIndex; i >= 0; i--) {
    //     StackFrame const& stackFrame = s_stackFrames[i];
    //     io::_emergencyPrint("\t");
    //     io::_emergencyPrint(String::valueOf(stackFrame.funcAddr).cstr());

    // #if __has_feature(address_sanitizer)
    //         __asan_describe_address(const_cast<void*>(stackFrame.callAddr));
    // #endif
    //         io::_emergencyPrint(" at \"");
    //         io::_emergencyPrint("<unknown>");
    //         io::_emergencyPrint(":");
    //         io::_emergencyPrint("<line>");
    //         io::_emergencyPrint(":");
    //         io::_emergencyPrint("<column>");
    //         io::_emergencyPrint("\"\n");
    //     }
}
/**
 * @param funcAddr A pointer to the address of the function being entered.
 * @param callAddr A pointer to the address of the instruction that called the current function.
 * The no_instrument_function attribute must be applied to prevent infinite recursion, as these functions should not be
 * instrumented themselves.
 */
extern "C" [[maybe_unused, gnu::no_instrument_function]]
inline void __cyg_profile_func_enter(void* funcAddr, void* callAddr)
{
    startup::s_currentStackFrameIndex++;
    if (startup::s_currentStackFrameIndex >= startup::MAX_PROFILER_STACK_FRAMES) {
        __builtin_trap();
    } else {
        startup::s_stackFrames[startup::s_currentStackFrameIndex] = {funcAddr, callAddr, 0};
    }
}

/**
 * @param funcAddr A pointer to the address of the function being exited.
 * @param callAddr A pointer to the address of the instruction that called the current function.
 * The no_instrument_function attribute must be applied to prevent infinite recursion, as these functions should not be
 * instrumented themselves.
 */
extern "C" [[maybe_unused, gnu::no_instrument_function]]
inline void __cyg_profile_func_exit(void* funcAddr, void* callAddr)
{
    (void)funcAddr;
    (void)callAddr;
    if (startup::s_currentStackFrameIndex <= 0) {
        __builtin_trap();
    } else {
        startup::s_stackFrames[startup::s_currentStackFrameIndex].tElapsed = 0;
        startup::s_currentStackFrameIndex--;
    }
}

#if __linux
///
/// This registers a signal handler in order to print the stack trace when an error like segfault or trap occurs.
///
[[gnu::constructor(100), gnu::no_instrument_function]]
inline void signalHandlers()
{

    struct sigaction const segFaultHandler{
#ifdef sa_handler
#define old_sa_handler sa_handler
#undef sa_handler
        .__sigaction_handler.sa_handler
#define sa_handler old_sa_handler
#else
        .sa_handler
#endif
        = [] [[gnu::no_instrument_function]] (int) -> void {
            _emergencyPrint("\x1B[31mSegmentation Fault\n");
            Profiler::printStackTrace();
            _emergencyPrint("\x1B[0m");
            raise(SIGKILL);
        },
        .sa_mask = {0},
        .sa_flags = 0,
        .sa_restorer = nullptr};

    struct sigaction const trapHandler{
#ifdef sa_handler
#undef sa_handler
        .__sigaction_handler.sa_handler
#else
        .sa_handler
#endif
        = [] [[gnu::no_instrument_function]] (int) -> void {
            _emergencyPrint("\x1B[31mPanic: Program halted (CPU trap)\n");
            Profiler::printStackTrace();
            _emergencyPrint("\x1B[0m");
            raise(SIGKILL);
        },
        .sa_mask = {0},
        .sa_flags = 0,
        .sa_restorer = nullptr};

    sigaction(SIGSEGV, &segFaultHandler, nullptr);
    sigaction(SIGILL, &trapHandler, nullptr);
}
#endif

}  // namespace cm

#include <stdlib.h>


constexpr std::align_val_t DEFAULT_ALIGNMENT = std::align_val_t(8);


inline void* newImpl(std::size_t size, std::align_val_t alignment) noexcept
{
    void* ptr;
    if (alignment == DEFAULT_ALIGNMENT) {
        ptr = malloc(size);
    } else {
        ptr = aligned_alloc(static_cast<size_t>(alignment), size);
    }
    ::cm::Assert(ptr);
    return ptr;
}

inline void* newImplNothrow(std::size_t size, std::align_val_t alignment) noexcept
{
    return aligned_alloc(static_cast<size_t>(alignment), size);
    // return GC_alloc(size, size_t(alignment));
}

inline void deleteImpl(void* ptr)
{
    // GC_free(ptr);
    free(ptr);
    (void)ptr;
}


///
///
///

void* operator new(std::size_t size) { return newImpl(size, DEFAULT_ALIGNMENT); }

void* operator new[](std::size_t size) { return newImpl(size, DEFAULT_ALIGNMENT); }

void* operator new(std::size_t size, std::align_val_t al) { return newImpl(size, al); }

void* operator new[](std::size_t size, std::align_val_t al) { return newImpl(size, al); }


///
///
///
///

void* operator new(std::size_t size, std::nothrow_t const&) noexcept { return newImplNothrow(size, DEFAULT_ALIGNMENT); }

void* operator new[](std::size_t size, std::nothrow_t const&) noexcept
{
    return newImplNothrow(size, DEFAULT_ALIGNMENT);
}

void* operator new(std::size_t size, std::align_val_t al, std::nothrow_t const&) noexcept
{
    return newImplNothrow(size, al);
}

void* operator new[](std::size_t size, std::align_val_t al, std::nothrow_t const&) noexcept
{
    return newImplNothrow(size, al);
}

///
///
///
///

void operator delete(void* ptr) noexcept { return deleteImpl(ptr); }

void operator delete[](void* ptr) noexcept { return deleteImpl(ptr); }

void operator delete(void* ptr, __SIZE_TYPE__) noexcept { return deleteImpl(ptr); }

void operator delete[](void* ptr, __SIZE_TYPE__) noexcept { return deleteImpl(ptr); }

extern "C" [[noreturn]]
void __cxa_pure_virtual()
{
    __builtin_trap();
}
