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
/// Linux implementation for a stream that writes to a file
///
struct LinuxFileOutStream final : public OutStream, public NonCopyable
{
    constexpr LinuxFileOutStream(LinuxFileOutStream&& other) = default;
    constexpr LinuxFileOutStream& operator=(LinuxFileOutStream&& other) = default;

    int _fd = 0;
    Status _status = STATUS_OK;
    Array<u8> _buffer;
    usize _bufferUsed = 0;

    ///
    /// Translates Linux errno values in the context of files into (system-independent) Status codes
    /// The reason why some distinct Linux errno values map to the same Status value is because not all operating
    /// systems might have the same level of detail, and Status should only represent conditions that are common across
    /// all operating systems. There's always going to be some information loss when you make a system-independent
    /// abstraction.
    ///
    StreamStatus setStatusFromErrno(StreamStatus& status, int const& err = errno)
    {
        switch (err) {

        case EPERM:
        case EACCES: return status = STATUS_ERR_ACCESS;

        case EBADF: return status = STATUS_ERR_INVALID;

        case ETXTBSY:
        case EBUSY: return status = STATUS_ERR_BUSY;

        case EDQUOT: return status = STATUS_ERR_QUOTA;
        case EEXIST: return status = STATUS_ERR_ALREADY_EXISTS;

        // Cases when an operation is "unsupported"
        case ENOTTY:  // Not a typewriter
        case EISDIR:  // Is a directory (so it can't itself be written to)
            return status = STATUS_ERR_UNSUPPORTED;

        case EINTR: return status = STATUS_ERR_INTERRUPT;
        case EIO: return status = STATUS_ERR_IO;
        case ELOOP: return status = STATUS_ERR_LOOP;

        case EMLINK:
        case EMFILE: return status = STATUS_ERR_LIMIT;

        // Cases when "memory runs out"
        case ENOMEM:  // Now technically this is running out of RAM, not disk..
        case EFBIG:   // File too big
        case ENOSPC:  // No space on disk
            return status = STATUS_ERR_MEMORY;

        case ENODEV:  // No such device
        case ESRCH:   // No such process
            return status = STATUS_ERR_NOT_FOUND;

        default: return status = STATUS_ERR_UNKNOWN;
        }
    }

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
            setStatusFromErrno(_status, err);
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
        if (_fd < 3) {
            return;
        }

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
            _status = setStatusFromErrno(_status, err);
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
            return Err(setStatusFromErrno(_status, err));
        } else {
            return Ok(_status = STATUS_OK);
        }
    }


    ///
    /// Returns the status
    ///
    Status status() const override { return _status; }
};

///
/// A stream that writes to a file.
///
struct FileOutStream : public Optional<LinuxFileOutStream>
{
    using Optional<LinuxFileOutStream>::Optional;

    ///
    /// Creates a file descriptor for writing, assuming file exists
    /// @param path Absolute path to the file
    /// @param bufferCapacity An optional capacity for the buffer, default 4KB
    ///
    FileOutStream(String const& path, Optional<usize> const& bufferCapacity = 4_KB)
        : Optional<LinuxFileOutStream>(LinuxFileOutStream(path, bufferCapacity))
    {}

    ~FileOutStream() = default;
};

#endif
