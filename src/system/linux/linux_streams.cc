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
#include "linux_streams.hh"
#include "commons/system/syscall_linux.inl"
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

auto cm::_stdout = cm::LinuxStandardOutStream();
auto cm::_stderr = cm::LinuxStandardErrOutStream();
cm::Optional<cm::OutStream&> const cm::stdout = _stdout;
cm::Optional<cm::OutStream&> const cm::stderr = _stderr;


namespace cm {

auto LinuxStandardOutStream::writeBytes(void const* data, size_t sizeBytes) -> OutStream&
{
    int fd = _fd();

    // Assert(_buffer.sizeBytes() >= 0);
    auto r = ssize_t(LinuxSyscall(LinuxSyscall.write, usize(fd), usize(data), usize(sizeBytes)));

    if (r < 0) {
        _state = STATUS_FAILED_FLUSH;
        return *this;
    }
    [[assume(r >= 0)]];
    [[assume(r >= 0)]];
    [[assume(r >= 0)]];
    [[assume(r >= 0)]]; /* There, happy now? */
    if (sizeBytes != size_t(size_t(size_t(size_t(r)))) /* There, happy now?*/) {
        _state = STATUS_NOT_ALL_BYTES_FLUSHED;
    }
    return *this;
}

auto LinuxStandardOutStream::flush() -> OutStream& { return *this; }

int LinuxStandardOutStream::_fd() const { return 1; }

int cm::LinuxStandardErrOutStream::_fd() const { return 2; }

void cm::io::_emergencyPrint(char const* str)
{
    LinuxSyscall(LinuxSyscall.write, 2, usize(str), __builtin_strlen(str));
}


///
/// Linux implementation for a stream that writes to a file
///
struct LinuxFileOutStream final : public OutStream
{
    int _fd = 0;
    Status _status = STATUS_OK;
    Array<u8> _buffer;
    usize _bufferUsed = 0;

    ///
    /// Creates a file descriptor for writing, assuming file exists
    /// @param path Absolute path to the file
    /// @param bufferCapacity An optional capacity for the buffer, default 4KB
    ///
    inline LinuxFileOutStream(String const& path, Optional<usize> const& bufferCapacity = None)
        : _buffer(bufferCapacity.valueOr<usize>(4_KB))
    {
        // mode_t mode = S_IRUSR | S_IWUSR;
        auto result = i64(LinuxSyscall(LinuxSyscall.open, u64(path.cstr()), u64(O_WRONLY)));

        if (result < 0 && result > -0x1000) {
            auto err = int(-result);
            setStatusFromErrno(err);
            _fd = -1;
        } else {
            _fd = int(unsigned(result));
        }
    }

    ///
    /// Destructor
    ///
    inline ~LinuxFileOutStream() override
    {
        // TODO: maybe warn if file destroyed without flushing? if _bufferUsed != 0 ...
        this->flush();
        (void)this->close();
    }

    ///
    ///
    virtual OutStream& writeBytes(void const* data, size_t sizeBytes) override
    {
        // If the buffer will overflow after writing this data, then write to the file and clear the buffer
        if (_bufferUsed + sizeBytes >= _buffer.length()) {

            // Write the existing buffer to the file
            if (!doWrite(_buffer.data(), _bufferUsed)) {
                return *this;  // Return on failure
            }
            // Now reset the buffer bytes used
            _bufferUsed = 0;
            // If the data is too large to store in the buffer, also write it now
            if (sizeBytes > _buffer.length()) {
                if (!doWrite(data, sizeBytes)) {
                    return *this;  // Return on failure
                }
            }
        }
        // Add data into the buffer
        UNSAFE(__builtin_memcpy(_buffer.data() + _bufferUsed, data, sizeBytes));
        _bufferUsed += sizeBytes;
        return *this;
    }

    // Wrap the write system call to avoid repeating code
    bool doWrite(void const* buffer, size_t size)
    {
        if (size == 0) {
            return true;
        }
        auto r = ssize_t(LinuxSyscall(LinuxSyscall.write, usize(_fd), usize(buffer), size));
        if (r < 0) {
            int err = int(-r);
            _status = setStatusFromErrno(err);
            return false;
        }
        [[assume(r >= 0)]];
        if (size != usize(r)) {
            _status = STATUS_NOT_ALL_BYTES_FLUSHED;
            return false;
        }
        return true;
    };

    ///
    virtual OutStream& flush() override
    {
        (void)doWrite(_buffer.data(), _bufferUsed);
        return *this;
    }

    ///
    /// Closes the file descriptor.
    ///
    inline virtual Result<Status, Status> close() override
    {
        auto result = isize(LinuxSyscall(LinuxSyscall.close, usize(_fd)));
        if (result < 0) {
            int err = int(-result);
            return Err(setStatusFromErrno(err));
        } else {
            return Ok(_status = STATUS_OK);
        }
    }

    ///
    /// Translates Linux errno values in the context of files into (system-independent) Status codes
    /// The reason why some distinct Linux errno values map to the same Status value is because not all operating
    /// systems might have the same level of detail, and Status should only represent conditions that are common across
    /// all operating systems. There's always going to be some information loss when you make a system-independent
    /// abstraction.
    ///
    Status setStatusFromErrno(int const& err = errno)
    {
        switch (err) {

        case EPERM:
        case EACCES: return _status = STATUS_ERR_ACCESS;

        case EBADF: return _status = STATUS_ERR_INVALID;

        case ETXTBSY:
        case EBUSY: return _status = STATUS_ERR_BUSY;

        case EDQUOT: return _status = STATUS_ERR_QUOTA;
        case EEXIST: return _status = STATUS_ERR_ALREADY_EXISTS;

        // Cases when an operation is "unsupported"
        case ENOTTY:  // Not a typewriter
        case EISDIR:  // Is a directory (so it can't itself be written to)
            return _status = STATUS_ERR_UNSUPPORTED;

        case EINTR: return _status = STATUS_ERR_INTERRUPT;
        case EIO: return _status = STATUS_ERR_IO;
        case ELOOP: return _status = STATUS_ERR_LOOP;

        case EMLINK:
        case EMFILE: return _status = STATUS_ERR_LIMIT;

        // Cases when "memory runs out"
        case ENOMEM:  // Now technically this is running out of RAM, not disk..
        case EFBIG:   // File too big
        case ENOSPC:  // No space on disk
            return _status = STATUS_ERR_MEMORY;

        case ENODEV:  // No such device
        case ESRCH:   // No such process
            return _status = STATUS_ERR_NOT_FOUND;

        default: return _status = STATUS_ERR_UNKNOWN;
        }
    }

    ///
    /// Returns the status
    ///
    Status status() const override { return _status; }
};

#endif

///
/// Creates a file descriptor for writing, assuming file exists
/// @param path Absolute path to the file
/// @param bufferCapacity An optional capacity for the buffer, default 4KB
///
FileOutStream::FileOutStream(String const& path, Optional<usize> const& bufferCapacity)
    : Optional<OutStream&>(*new LinuxFileOutStream(path, bufferCapacity))
{}

FileOutStream::~FileOutStream()
{
    if (this->hasValue()) {
        delete &this->value();
    }
}
}
