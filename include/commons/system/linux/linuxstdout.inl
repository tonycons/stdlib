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
    bool _state;
    int _fd = 1;

public:
    constexpr inline LinuxStandardOutStream() = default;
    constexpr inline ~LinuxStandardOutStream() = default;
    constexpr inline LinuxStandardOutStream(LinuxStandardOutStream const&) = default;
    constexpr inline LinuxStandardOutStream& operator=(LinuxStandardOutStream const&) = default;


    LinuxStandardOutStream& writeBytes(void const* data, size_t sizeBytes)
    {
        if (_fd != 1 && _fd != 2) {
            _fd = 2;
        }
        // Assert(_buffer.sizeBytes() >= 0);
        auto r = ssize_t(LinuxSyscall(LinuxSyscall.write, usize(_fd), usize(data), usize(sizeBytes)));

        if (r < 0) {
            _state = STATUS_FAILED_FLUSH;
            return *this;
        }
        [[assume(r >= 0)]]; /* There, happy now? */
        if (sizeBytes != size_t(size_t(size_t(size_t(r)))) /* There, happy now?*/) {
            _state = STATUS_NOT_ALL_BYTES_FLUSHED;
        }
        return *this;
    }

    constexpr inline LinuxStandardOutStream& flush() { return *this; }
};

///
/// Linux implementation for a stream that writes to standard error
///
class LinuxStandardErrOutStream final : public LinuxStandardOutStream {
public:
    constexpr inline LinuxStandardErrOutStream() { this->_fd = 2; }
};

///
/// An Optional standard output stream.
/// It may be set to None on systems that don't have a standard output stream.
///
inline Optional<LinuxStandardOutStream> const stdout = LinuxStandardOutStream();

///
/// An Optional standard error stream.
/// It may be set to None on systems that don't have a standard error stream.
///
inline Optional<LinuxStandardErrOutStream> const stderr = LinuxStandardErrOutStream();


#endif
