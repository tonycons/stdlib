/*
   Copyright 2025 Anthony A. Constantinescu.

   Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
   in compliance with the License. You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software distributed under the License
   is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
   or implied. See the License for the specific language governing permissions and limitations under
   the License.
*/

#include <Commons/system.hh>

#if __has_feature(address_sanitizer)
#include <sanitizer/asan_interface.h>
#include <sanitizer/common_interface_defs.h>
#elif __has_feature(dataflow_sanitizer)
#include <sanitizer/dfsan_interface.h>
#endif
[[maybe_unused]]
static char s_stackTraceBuffer[1024];
#if __linux__
#include <signal.h>
#endif

///


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"

namespace cm {

constexpr static auto MAX_PROFILER_STACK_FRAMES = 65_KB;
static ::cm::Profiler::StackFrame s_stackFrames[MAX_PROFILER_STACK_FRAMES]{};
static unsigned int s_currentStackFrameIndex{};
static bool s_printed = false;


[[gnu::no_instrument_function]]
void Profiler::printStackTrace()
{
    if (s_printed)
        return;

    s_printed = true;
    io::_emergencyPrint("\nStack Trace:\n");
#if __has_feature(address_sanitizer)
    __sanitizer_print_stack_trace();
    io::_emergencyPrint("\n");
#elif __has_feature(dataflow_sanitizer)
    dfsan_sprint_stack_trace(s_stackTraceBuffer, 1024);
    io::_emergencyPrint(s_stackTraceBuffer);
    io::_emergencyPrint("\x1B[0m\n");
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
void __cyg_profile_func_enter(void* funcAddr, void* callAddr)
{
    s_currentStackFrameIndex++;
    if (s_currentStackFrameIndex >= MAX_PROFILER_STACK_FRAMES) {
        __builtin_trap();
    } else {
        s_stackFrames[s_currentStackFrameIndex] = {funcAddr, callAddr, 0};
    }
}

/**
 * @param funcAddr A pointer to the address of the function being exited.
 * @param callAddr A pointer to the address of the instruction that called the current function.
 * The no_instrument_function attribute must be applied to prevent infinite recursion, as these functions should not be
 * instrumented themselves.
 */
extern "C" [[maybe_unused, gnu::no_instrument_function]]
void __cyg_profile_func_exit(void* funcAddr, void* callAddr)
{
    (void)funcAddr;
    (void)callAddr;
    if (s_currentStackFrameIndex <= 0) {
        __builtin_trap();
    } else {
        s_stackFrames[s_currentStackFrameIndex].tElapsed = 0;
        s_currentStackFrameIndex--;
    }
}

#if __linux
///
/// This registers a signal handler in order to print the stack trace when an error like segfault occurs.
///
[[gnu::constructor(100), gnu::no_instrument_function]]
void registerSignalHandlers()
{

    struct sigaction const segFaultHandler{
#ifdef sa_handler
#undef sa_handler
        .__sigaction_handler.sa_handler
#else
        .sa_handler
#endif
        = [] [[gnu::no_instrument_function]]
          (int) -> void {
            io::_emergencyPrint("\x1B[31mSegmentation Fault\n");
            Profiler::printStackTrace();
            io::_emergencyPrint("\x1B[0m");
            raise(SIGKILL);
        },
        .sa_mask = {0},
        .sa_flags = 0,
        .sa_restorer = nullptr};

    sigaction(SIGSEGV, &segFaultHandler, nullptr);
}
#endif

}  // namespace cm
