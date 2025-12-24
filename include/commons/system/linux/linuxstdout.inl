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
class LinuxStandardOutStream : public OutStream {
private:
    bool _state;

public:
    LinuxStandardOutStream() = default;
    ~LinuxStandardOutStream() override = default;

    constexpr static LinuxStandardOutStream& instance()
    {
        static LinuxStandardOutStream s;
        return s;
    }

    virtual OutStream& writeBytes(void const* data, size_t sizeBytes) override
    {
        int fd = _fd();

        // Assert(_buffer.sizeBytes() >= 0);
        auto r = ssize_t(LinuxSyscall(LinuxSyscall.write, usize(fd), usize(data), usize(sizeBytes)));

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
    virtual OutStream& flush() override { return *this; }

protected:
    virtual int _fd() const { return 1; }
};

///
/// Linux implementation for a stream that writes to standard error
///
class LinuxStandardErrOutStream final : public LinuxStandardOutStream {
    int _fd() const override { return 2; }
};

///
/// An Optional standard output stream.
/// It may be set to None on systems that don't have a standard output stream.
///
inline Optional<OutStream&> const stdout = LinuxStandardOutStream::instance();

///
/// An Optional standard error stream.
/// It may be set to None on systems that don't have a standard error stream.
///
inline Optional<OutStream&> const stderr = LinuxStandardOutStream::instance();


#endif
