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


#include <Commons/core.hh>
#include <Commons/system.hh>
#include <unistd.h>


[[noreturn]]
void ::cm::panic(char const* message, char const* reason, SourceLocation src)
{
    if (message == nullptr)
        message = "";

    if (reason == nullptr)
        reason = "";

    write(STDERR_FILENO, "\x1B[31m", sizeof("\x1B[31m"));
    write(STDERR_FILENO, message, __builtin_strlen(message));

    if (src.function() != nullptr || src.file() != nullptr) {
        write(STDERR_FILENO, " in ", 4);
    }
    if (src.function() != nullptr) {
        write(STDERR_FILENO, src.function(), __builtin_strlen(src.function()));
    }
    if (src.file() != nullptr) {
        write(STDERR_FILENO, " at \"", 5);
        write(STDERR_FILENO, src.file(), __builtin_strlen(src.file()));
        write(STDERR_FILENO, "\"", 1);
    }
    if (reason != nullptr) {
        write(STDERR_FILENO, ".\n\treason: ", 11);
        write(STDERR_FILENO, reason, __builtin_strlen(reason));
    }

    Profiler::printStackTrace();

    write(STDERR_FILENO, "\x1B[0m", sizeof("\x1B[0m"));
    __builtin_trap();
    //_exit(-1);
}
