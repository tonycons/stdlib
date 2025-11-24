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
#pragma once
#ifdef __linux__
#include <commons/system.hh>

namespace cm {

class LinuxStandardOutStream : public Stream {
private:
    bool _state;

public:
    LinuxStandardOutStream() = default;
    ~LinuxStandardOutStream() override = default;

    virtual Stream& writeBytes(void const* data, size_t sizeBytes) override;
    virtual Stream& flush() override;

protected:
    virtual int _fd() const;
};

class LinuxStandardErrStream final : public LinuxStandardOutStream {
    int _fd() const override;
};

extern LinuxStandardOutStream _stdout;
extern LinuxStandardErrStream _stderr;

}  // namespace cm


#endif
