/*
   Copyright 2025 Anthony A. Constantinescu.

   Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
   in compliance with the License. You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software distributed under the License
   is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
   or implied. See the License for the specific language governing permissions and limitations under
   the License.

   File: commons/system/syscall_linux_x64_numbers.inl
   Description: Functionality for making direct syscalls on Linux
*/

#pragma once
#ifdef __inline_sys_header__

struct LinuxShell : NonCopyable
{
    constexpr LinuxShell() noexcept = default;
    constexpr LinuxShell(LinuxShell&&) noexcept = default;
    constexpr LinuxShell& operator=(LinuxShell&&) noexcept = default;
    constexpr ~LinuxShell() noexcept = default;


    inline int execute(String const& command, Optional<Function<void(void const*, usize)>> output = None)
    {
        //__builtin_printf("Command is this: %s\n", command.cstr());
        auto s = _escapeCommand(command);
        // stdout->println("escaped: `", s);
        FILE* fp = popen(s.cstr(), "r");
        if (fp == nullptr) {
            return -1;  // TODO
        }
        if (output.hasValue()) {
            for (int value = fgetc(fp); value != EOF; value = fgetc(fp)) {
                auto byte = char(value);
                output.value()(&byte, 1);
            }
        }
        return pclose(fp);
    }

    inline String _escapeCommand(String const& command) { return command.replace("\"", "\\\""); }
};

inline Optional<LinuxShell> const shell = LinuxShell();

#endif
