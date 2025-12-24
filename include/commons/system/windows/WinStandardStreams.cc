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
#include <commons/system/io/print.hh>
#include "WinStandardOutStream.hh"
#include "WindowsNT.hh"  // IWYU pragma: keep

///
/// edit 5/8/2025
///
auto cm::Win32StandardOutStream::writeBytes(void const* data, i32 sizeBytes) -> OutStream&
{

    _buffer.append(reinterpret_cast<char const*>(data), sizeBytes / static_cast<i32>(sizeof(char)));
    return *this;
}

///
/// edit 5/9/2025
///
auto cm::Win32StandardOutStream::flush() -> OutStream&
{

    DWORD bytesWritten;

    if (_handle == nullptr) {
        _handle = _getHandle();
    }
    if (_handle == nullptr || _handle == INVALID_HANDLE_VALUE) {
        _state = false;
        return *this;
    }

    Assert(_buffer.sizeBytes() >= 0);

    if (0 == ::WriteFile(_handle, _buffer.data(), static_cast<DWORD>(_buffer.sizeBytes()), &bytesWritten, nullptr)) {
        if (::GetLastError() != ERROR_IO_PENDING) {
            _state |= STATE_FAILED_FLUSH;
            _buffer.clear();
            return *this;
        }
    }
    // Assert(::FlushFileBuffers(_handle));
    _state = static_cast<i32>(bytesWritten) == _buffer.sizeBytes();
    _buffer.clear();

    return *this;
}

///
/// edit 5/8/2025
///
auto cm::Win32StandardOutStream::_getHandle() -> HANDLE { return ::GetStdHandle(STD_OUTPUT_HANDLE); }

///
/// edit 5/8/2025
///
auto cm::Win32StandardErrOutStream::_getHandle() -> HANDLE { return ::GetStdHandle(STD_ERROR_HANDLE); }

///
/// edit 5/9/2025
///
void cm::io::_emergencyPrint(char const* str)
{
    DWORD _;
    UNSAFE({
        ::WriteFile(
            ::GetStdHandle(STD_ERROR_HANDLE), str, static_cast<DWORD>(min(__builtin_strlen(str), 0xffffffff)), &_,
            nullptr);
    })
}
#endif
