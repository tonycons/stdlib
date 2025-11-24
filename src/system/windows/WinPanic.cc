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

#ifdef _WIN32

#include <commons/core.hh>
#include <commons/system/Profiler.hh>
#include "WindowsNT.hh"


void ::cm::panic(char const* message, char const* reason, SourceLocation src)
{
    if (message == nullptr)
        message = "";

    DWORD _;
    HANDLE out = GetStdHandle(STD_ERROR_HANDLE);

    // Red color begin
    WriteFile(out, "\x1B[31m", sizeof("\x1B[31m"), &_, nullptr);
    WriteFile(out, message, __builtin_strlen(message), &_, nullptr);

    // Print source location and reason
    if (src.function() != nullptr || src.file() != nullptr) {
        WriteFile(out, " in ", 4, &_, nullptr);
    }
    if (src.function() != nullptr) {
        WriteFile(out, src.function(), __builtin_strlen(src.function()), &_, nullptr);
    }
    if (src.file() != nullptr) {
        WriteFile(out, " at \"", 5, &_, nullptr);
        WriteFile(out, src.file(), __builtin_strlen(src.file()), &_, nullptr);
        WriteFile(out, "\"", 1, &_, nullptr);
    }
    if (reason != nullptr) {
        WriteFile(out, ".\n\treason: ", 11, &_, nullptr);
        WriteFile(out, reason, __builtin_strlen(reason), &_, nullptr);
    }

    Profiler::printStackTrace();

    // Red color end
    WriteFile(out, "\x1B[0m", sizeof("\x1B[0m"), &_, nullptr);


    ExitProcess(-1);
    while (true)
        ;
}

#endif
