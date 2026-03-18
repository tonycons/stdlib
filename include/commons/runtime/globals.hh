/*
   Copyright 2025 Anthony A. Constantinescu.

   Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
   in compliance with the License. You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software distributed under the License
   is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
   or implied. See the License for the specific language governing permissions and limitations under
   the License.

   File: runtime/globals.hh
   Description: Global variables for the library's runtime
*/


#pragma once

#include "commons/system.hh"  // IWYU pragma: keep
#include "commons/config.hh"  // IWYU pragma: keep


inline u32 cm::FastPRNG::_state;
inline u32 cm::ND_PRNG::_state;
namespace cm {

namespace validator {
// global variables for the validator
inline thread_local SourceLocation loc;
inline thread_local char msg[32]{};
inline thread_local bool volatile checking;
inline thread_local char const* _detail;
}

// global variables for the main runtime
namespace startup {
static thread_local Profiler::StackFrame s_stackFrames[LibraryConfig::MAX_STACK_FRAMES]{};
static thread_local usize s_currentStackFrameIndex{};
static PreInitAssertion preInitAssertions[LibraryConfig::MAX_PRE_INIT_ASSERTIONS]{};
static usize preInitAssertionsCount = 0;
/// Memory usage statistics
struct
{
    usize bytesAllocated;
} static memoryStats{};

[[gnu::init_priority(LibraryConfig::GLOBAL_CTOR_BASE_PRIO)]]
static Mutex panicMutex;

}  // namespace startup

/// A print function that goes directly to the system, used case the normal print function is unusable.
/// For example, if the normal print function panics, then panic() cannot use the normal print function because there
/// would be an infinite panic loop.
inline void _emergencyPrint(char const* msg)
{
    kernel::call(kernel::write, kernel::stderr, msg, CArrays::stringLen(msg));
}

}  // namespace cm
