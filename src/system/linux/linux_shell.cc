/*
   Copyright 2025 Anthony A. Constantinescu.

   Licensed under the Apache License, Version 2.0 (the "License"); you may not
   use this file except in compliance with the License. You may obtain a copy of
   the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
   WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
   License for the specific language governing permissions and limitations under
   the License.
*/

#ifdef __linux__
#include <commons/system.hh>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>


namespace cm {

class LinuxShell : public Shell {
public:
    LinuxShell() = default;
    ~LinuxShell() override = default;

    virtual int execute(String const& command, Optional<OutStream&> const& output) override;
};

}  // namespace cm


static auto _shell = cm::LinuxShell();
cm::Optional<cm::Shell&> const cm::shell = _shell;

///
/// popen(command,...) ends up doing sh -c "command", where command is surrounded in quotes, so any quotes inside the
/// command string need to be escaped.
///
static auto _escapeCommand(cm::String const& command) { return command.replace("\"", "\\\""); }


int cm::LinuxShell::execute(String const& command, Optional<OutStream&> const& output)
{
    //__builtin_printf("Command is this: %s\n", command.cstr());
    auto s = _escapeCommand(command);
    // stdout->println("escaped: `", s);
    FILE* fp = popen(s.cstr(), "r");
    if (fp == NULL) {
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


#endif
