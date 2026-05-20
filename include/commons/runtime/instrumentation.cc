/*
   Copyright 2025 Anthony A. Constantinescu.

   Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
   in compliance with the License. You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software distributed under the License
   is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
   or implied. See the License for the specific language governing permissions and limitations under
   the License.

   File: runtime/instrumentation.cc
   Description: Functions related to profiling/instrumentation
*/

#pragma once

#include "globals.hh"
#include "symbols.hh"

namespace cm {

///
/// Pretty self-explanatory, prints a full stack trace of whatever function called printStackTrace().
/// Address sanitizer already provides a function to print the stack trace. Without that, there currently isn't a way I
/// have figured out to get this information (perhaps parse the executable's symbols/ DWARF metadata?
///
[[gnu::no_instrument_function]]
inline void Profiler::printStackTrace()
{
    thread_local bool s_printed = false;
    if (s_printed) {
        return;
    }
    s_printed = true;
    {
        panicPrint(
            FixedString<64>::cformat(
                "Process: %s\nMemory: %zu bytes\nStack Trace: %zu\n", "main", startup::memoryStats.bytesAllocated,
                startup::s_currentStackFrameIndex)
                .cstr());
    }
    for (auto i = startup::s_currentStackFrameIndex; i > 0; --i) {
        auto const& frame = startup::s_stackFrames[i];
        panicPrint(
            FixedString<LibraryConfig::MAX_SYMBOL_LENGTH + 16>::cformat(
                "\x1B[38;5;68m----(%zu) \x1B[32m%p\x1B[0m: \x1B[33m%s\x1B[0m\n", i, frame.funcAddr,
                debug::getSymbolName(frame.funcAddr).valueOr("<?>").cstr())
                .cstr());
    }
}


///
/// This function (when profiling is the enabled) is called by the compiler for each function call to push a stack frame
/// to the stack trace.
/// @param funcAddr A pointer to the address of the function being entered.
/// @param callAddr A pointer to the address of the instruction that called the current function.
/// The no_instrument_function attribute must be applied to prevent infinite recursion, as these functions should not be
/// instrumented themselves.
///
extern "C" [[maybe_unused, gnu::no_instrument_function]]
void __cyg_profile_func_enter(void* funcAddr, void* callAddr)  // NOLINT
{
    startup::s_currentStackFrameIndex++;
    if (startup::s_currentStackFrameIndex >= LibraryConfig::MAX_STACK_FRAMES) {
        panic("Maximum stack depth", "", SourceLocation::current());
    } else {
        // s_stackFrames is a C-style array because using a FixedArray, with its actual member functions, could call
        // __cyg_profile_func_enter in an infinite loop.
        startup::s_stackFrames[startup::s_currentStackFrameIndex] = {funcAddr, callAddr, 0};
    }
}

///
/// This function (when profiling is enabled) is called by compiler for each function return to pop a stack frame from
/// the stack trace.
/// The no_instrument_function attribute must be applied to prevent infinite recursion, as these functions should not be
/// instrumented themselves.
///
extern "C" [[maybe_unused, gnu::no_instrument_function]]
void __cyg_profile_func_exit(void*, void*)  // NOLINT
{
    if (startup::s_currentStackFrameIndex == 0) {
        // This should never happen because the compiler inserts an enter for every exit, so it should never pop the
        // first stack frame.
        panic("Compiler bug", "", SourceLocation::current());
    } else {
        startup::s_stackFrames[startup::s_currentStackFrameIndex].tElapsed = 0;
        startup::s_currentStackFrameIndex--;
    }
}
}  // namespace cm
