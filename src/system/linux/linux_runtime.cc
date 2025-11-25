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


#include <commons/core.hh>
#include <commons/system.hh>
#include <commons/system/syscall_linux.inl>

[[noreturn]]
void ::cm::panic(char const* message, char const* reason, SourceLocation src)
{
    if (message == nullptr)
        message = "";

    if (reason == nullptr)
        reason = "";

    auto write = [](char const* str, usize len) {
        LinuxSyscall(LinuxSyscall.write, 2, u64(str), len);
    };
    write("\x1B[31m", sizeof("\x1B[31m"));
    write(message, __builtin_strlen(message));

    if (src.function() != nullptr || src.file() != nullptr) {
        write((" in "), 4);
    }
    if (src.function() != nullptr) {
        write((src.function()), __builtin_strlen(src.function()));
    }
    if (src.file() != nullptr) {
        write((" at \""), 5);
        write(src.file(), __builtin_strlen(src.file()));
        write("\"", 1);
    }
    if (reason != nullptr) {
        write(".\n\treason: ", 11);
        write(reason, __builtin_strlen(reason));
    }
    Profiler::printStackTrace();
    write("\x1B[0m", sizeof("\x1B[0m"));
    __builtin_trap();
    //_exit(-1);
}
