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
#ifdef __inline_sys_header__

///
/// Linux implementation for a stream that writes to standard output
///
class LinuxStandardOutStream : public IOutStream<LinuxStandardOutStream> {
protected:
    Status _state = STATUS_OK;
    int _fd = 1;

public:
    consteval inline LinuxStandardOutStream() = default;
    consteval inline LinuxStandardOutStream(LinuxStandardOutStream const&) = default;
    consteval inline LinuxStandardOutStream& operator=(LinuxStandardOutStream const&) = default;


    auto const& writeBytes(void const* data, size_t sizeBytes) const
    {
        // Assert(_buffer.sizeBytes() >= 0);
        // auto r =
        (LinuxSyscall(LinuxSyscall.write, usize(_fd), usize(data), usize(sizeBytes)));

        // if (r < 0) {
        //     //_state = STATUS_FAILED_FLUSH;
        //     return *this;
        // }
        // [[assume(r >= 0)]]; /* There, happy now? */
        // if (sizeBytes != size_t(size_t(size_t(size_t(r)))) /* There, happy now?*/) {
        //     _state = STATUS_NOT_ALL_BYTES_FLUSHED;
        // }
        return *this;
    }

    constexpr inline auto const& flush() const { return *this; }
};

///
/// Linux implementation for a stream that writes to standard error
///
class LinuxStandardErrOutStream final : public LinuxStandardOutStream {
public:
    consteval inline LinuxStandardErrOutStream() { this->_fd = 2; }
};

///
/// An Optional standard output stream.
/// It may be set to None on systems that don't have a standard output stream.
///
constexpr inline auto stdout = LinuxStandardOutStream();

///
/// An Optional standard error stream.
/// It may be set to None on systems that don't have a standard error stream.
///
constexpr inline auto stderr = LinuxStandardErrOutStream();


#endif
