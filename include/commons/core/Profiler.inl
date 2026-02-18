/*
   Copyright 2024 Anthony A. Constantinescu.

    Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
    in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software distributed under the License
    is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
    or implied. See the License for the specific language governing permissions and limitations under
    the License.
*/
#pragma once
#ifndef __inline_core_header__
#warning Do not include this file directly; include "core.hh" instead
#else


namespace cm {

/**
 * @brief Basic profiler that can generate a stack trace
 */
class Profiler {
public:
    struct StackFrame
    {
        void const* funcAddr;
        void const* callAddr;
        long tElapsed = 0;  // TODO
    };

    static void init();
    static void push(SourceLocation src);
    static void pop();
    static void printStackTrace();
    static StackFrame const& getCurrentStackFrame();
};

/*
 The statement "ProfiledScope _" pushes the current stack frame onto the profiler and automatically
 pops it when the scope is finished.
 It also measures the clock cycles elapsed during that time (TODO)
*/
struct ProfiledScope
{
    inline ProfiledScope(SourceLocation src = SourceLocation::current()) { Profiler::push(src); }

    inline ~ProfiledScope() { Profiler::pop(); }
};

#define CM_PROFILED_SCOPE ProfiledScope _
}  // namespace cm
#endif
