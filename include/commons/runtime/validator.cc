/*
   Copyright 2025 Anthony A. Constantinescu.

   Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
   in compliance with the License. You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software distributed under the License
   is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
   or implied. See the License for the specific language governing permissions and limitations under
   the License.

   File: runtime/validator.cc
   Description: Functions related to validation/sanity checking/error reporting
*/
#include "globals.hh"


namespace cm {
namespace validator {

/// validates that a pointer is readable.
/// if a segfault happens, this will print it nicely instead of showing the horrid addresssanitizer crash dump
template<size_t namesize>
void check(auto const* ptr, char const* name, SourceLocation const& loc_)
{
    using T = CVRefRemoved<decltype(*ptr)>;
    // save message
    __builtin_memset_inline(validator::msg, 0, sizeof(validator::msg));
    __builtin_memcpy_inline(validator::msg, name, sizeof(char) * namesize);
    validator::loc = loc_;
    // _segfault_triggered_by_check is volatile, which prevents compiler from
    // optimizing out this access
    validator::checking = true;
    validator::_detail = "Memory not readable";
    // force a dummy read of the pointer. If a segfault happens, the segfault handler will take care of printing.
    asm volatile("" ::: "memory");
    (void volatile) * reinterpret_cast<FixedArray<u8, sizeof(T)> const volatile*>(ptr);
    asm volatile("" ::: "memory");
    validator::checking = false;
}

/// validates that a pointer is readable and writable.
/// if a segfault happens, this will print it nicely instead of showing the horrid addresssanitizer crash dump
template<size_t namesize>
void check(auto* ptr, char const* name, SourceLocation const& loc_)
{
    using T = CVRefRemoved<decltype(*ptr)>;
    // save message
    __builtin_memset_inline(validator::msg, 0, sizeof(validator::msg));
    __builtin_memcpy_inline(validator::msg, name, sizeof(char) * namesize);
    validator::loc = loc_;
    // _segfault_triggered_by_check is volatile, which prevents compiler from
    // optimizing out this access
    validator::checking = true;
    validator::_detail = "Memory not readable and writable";
    // force a dummy write to the pointer (read and write it to itself). If a segfault happens, the segfault handler
    // will take care of printing. use memmove to avoid the copy constructor and make compiler not optimize it away
    asm volatile("" ::: "memory");
    auto volatile vbytes = reinterpret_cast<u8 volatile*>(ptr);
    for (usize i = 0; i < sizeof(T); i++) {
        vbytes[i] = reinterpret_cast<u8*>(ptr)[i];
    }
    asm volatile("" ::: "memory");
    validator::checking = false;
}

// validates that a pointer (of unknown type but variable size) is readable.
template<size_t namesize>
void checkBytes(void const* ptr, usize size, char const* name, SourceLocation const& loc_)
{
    auto bytes = static_cast<u8 const*>(ptr);
    for (usize i = 0; i < size; i++) {
        check<namesize>(bytes + i, name, loc_);
    }
}

// validates that a pointer (of unknown type but variable size) is readable and writable.
template<size_t namesize>
void checkBytes(void* ptr, usize size, char const* name, SourceLocation const& loc_)
{
    auto bytes = static_cast<u8*>(ptr);
    for (usize i = 0; i < size; i++) {
        check<namesize>(bytes + i, name, loc_);
    }
}

// Print the message when a validator sanity check fails
void printFailure()
{
    panicPrint(
        FixedString<256>::cformat(
            "\x1B[31mValidation failed: %s:\x1B[0m %s\x1B[31m at \"%s:%u:%u\"\x1B[0m\n", validator::_detail,
            validator::msg, validator::loc.file(), validator::loc.line(), validator::loc.column())
            .cstr());
}

#if __linux
/// This registers a signal handler in order to print the stack trace when an error like segfault or trap occurs.
[[gnu::constructor(101), gnu::no_instrument_function]]
inline void signalHandlers()
{
    constexpr auto segFaultHandler = [] {
        unix::sigaction_t sa;
        sa.sa_mask = {0};
        sa.sa_flags = 0;
        sa.sa_restorer = nullptr;
        sa.sa_sigaction = [] [[gnu::no_instrument_function]] (int, void*, void*) -> void {
            // first check if the segfault was triggered during a validator sanity check.
            if (validator::checking) {
                validator::printFailure();
            } else {
                // otherwise just print the regular stack trace for a segfault.
                panicPrint("\x1B[31mSegmentation Fault\n");
                panicPrint("\x1B[0m");
            }
            Profiler::printStackTrace();
            exit(-1);
        };
        return sa;
    }();

    constexpr auto trapHandler = [] {
        unix::sigaction_t sa;
        sa.sa_mask = {0};
        sa.sa_flags = 0;
        sa.sa_restorer = nullptr;
        sa.sa_sigaction = [] [[gnu::no_instrument_function]] (int, void*, void*) -> void {
            panicPrint("\x1B[31mPanic: Program halted (CPU trap)\n");
            Profiler::printStackTrace();
            panicPrint("\x1B[0m");
            exit(-1);
        };
        return sa;
    }();
    unix::sigaction(SIGSEGV, &segFaultHandler, nullptr);
    unix::sigaction(SIGILL, &trapHandler, nullptr);
}
#endif

}  // namespace validator
}  // namespace cm
