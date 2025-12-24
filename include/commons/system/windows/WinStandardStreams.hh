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
#include <commons/system/io/stream.hh>
#include <commons/datastructs/array.hh>

namespace cm {

class Win32StandardOutStream : public OutStream {
private:
    void* _handle;
    bool _state;
    String _buffer;

public:
    Win32StandardOutStream() = default;
    ~Win32StandardOutStream() override = default;

    virtual OutStream& writeBytes(void const* data, i32 sizeBytes) override;
    virtual OutStream& flush() override;

protected:
    virtual void* _getHandle();
};

class Win32StandardErrOutStream : public Win32StandardOutStream {
    void* _getHandle() override;
};

using PtrWin32StandardOutStream = Win32StandardOutStream*;
using PtrWin32StandardErrOutStream = Win32StandardErrOutStream*;

}  // namespace cm
