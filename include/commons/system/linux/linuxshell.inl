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

class LinuxShell : public Shell {
public:
    LinuxShell() = default;
    ~LinuxShell() override = default;

    inline static LinuxShell& instance()
    {
        static LinuxShell s;
        return s;
    }

    inline static auto _escapeCommand(String const& command) { return command.replace("\"", "\\\""); }

    virtual int execute(String const& command, Optional<OutStream&> const& output) override
    {
        //__builtin_printf("Command is this: %s\n", command.cstr());
        auto s = _escapeCommand(command);
        // stdout->println("escaped: `", s);
        FILE* fp = popen(s.cstr(), "r");
        if (fp == nullptr) {
            return -1;  // TODO
        }
        if (output != None) {
            for (int value = fgetc(fp); value != EOF; value = fgetc(fp)) {
                auto byte = char(value);
                output->writeBytes(&byte, 1);
            }
        }
        return pclose(fp);
    }
};


template<int = 0>
Optional<Shell&> const shell = LinuxShell::instance();

#endif
