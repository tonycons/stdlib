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
#ifdef __inline_core_header__

namespace LinuxFileStreamHelper {
///
/// Translates Linux errno values in the context of files into (system-independent) Status codes
/// The reason why some distinct Linux errno values map to the same Status value is because not all operating
/// systems might have the same level of detail, and Status should only represent conditions that are common across
/// all operating systems. There's always going to be some information loss when you make a system-independent
/// abstraction.
///
inline StreamStatus setStatusFromErrno(StreamStatus& status, int const& err = errno)
{
    return status = StreamStatus(err);
}

}  // namespace LinuxFileStreamHelper

///
/// Linux implementation of a stream that reads from a file
///
struct LinuxFileInStream final : InStream<LinuxFileInStream>, NonCopyable
{
    constexpr LinuxFileInStream(LinuxFileInStream&& other) = default;
    constexpr LinuxFileInStream& operator=(LinuxFileInStream&& other) = default;

    ///
    /// @param path Path to the file
    /// @param mode File opening mode
    ///
    LinuxFileInStream(String const& path, FileOpenMode const mode)
    {
        u64 sysMode = 0;
        if (mode & FileOpenMode::Read) {
            sysMode = (mode & FileOpenMode::Update) ? O_RDWR : O_RDONLY;
        } else if (mode & FileOpenMode::Write) {
            sysMode = ((mode & FileOpenMode::Read) ? O_RDWR : O_WRONLY) | O_CREAT | O_TRUNC;
        } else if (mode & FileOpenMode::Append) {
            sysMode = ((mode & FileOpenMode::Update) ? O_RDWR : O_WRONLY) | O_CREAT | O_APPEND;
        } else {
            _status = StreamStatus(EINVAL);
            return;
        }
        if (auto const result =
                static_cast<i64>(LinuxSyscall(LinuxSyscall.open, reinterpret_cast<u64>(path.cstr()), sysMode));
            result < 0 && result > -0x1000)
        {
            auto const err = static_cast<int>(-result);
            LinuxFileStreamHelper::setStatusFromErrno(_status, err);
            _fd = -1;
        } else {
            _fd = static_cast<int>(static_cast<unsigned>(result));
        }
    }

    ///
    /// Destructor
    ///
    ~LinuxFileInStream() { close(); }

    ///
    /// @return Status of close
    ///
    Result<Status, Status> close()
    {
        __builtin_printf("Closed\n");
        if (auto const result = static_cast<isize>(LinuxSyscall(LinuxSyscall.close, static_cast<usize>(_fd)));
            result < 0)
        {
            auto const err = static_cast<int>(-result);
            return Err(LinuxFileStreamHelper::setStatusFromErrno(_status, err));
        }
        return Ok(_status = STATUS_OK);
    }

    ///
    /// Reads bytes from the file into the buffer, attempting to fill the entire buffer if possible
    /// @tparam BufferType The type of the buffer structure to read into
    /// @param buffer The buffer to read into
    /// @return On success, returns the number of bytes read into the buffer (this quantity could be less than the
    /// capacity of the buffer, if, for example, the size of the file is less than the size of the buffer). On error,
    /// returns a Status error code.
    ///
    template<typename BufferType>
    Result<usize, Status> readBytes(BufferType& buffer)
    {
        auto const result = static_cast<isize>(LinuxSyscall(
            LinuxSyscall.read, static_cast<usize>(_fd), reinterpret_cast<usize>(buffer.data()), buffer.sizeBytes()));
        if (result < 0) {
            auto const err = static_cast<int>(-result);
            return Err(LinuxFileStreamHelper::setStatusFromErrno(_status, err));
        }
        return Ok(static_cast<usize>(result));
    }

    ///
    /// @return On success, returns a string containing the contents of the file interpreted as text. On error, returns
    /// a Status error code.
    ///
    template<usize BufferSize = 64>
    Result<String, Status> readAllAsString()
    {
        String str;
        while (true) {
            Array<u8, BufferSize> tmp{};
            auto const result = readBytes(tmp);
            str.append(StringRef(reinterpret_cast<char const*>(tmp.data()), tmp.sizeBytes() / sizeof(char)));
            if (result.isErr()) {
                return Err(result.errVal());
            }
            auto const bytesRead = result.okVal();
            if (bytesRead < BufferSize) {  // This means EOF was reached.
                break;
            }
        }
        return Ok(str);
    }

    constexpr Status status() const { return _status; }
    constexpr bool ok() const { return _status == STATUS_OK; }

    int _fd = 0;
    Status _status = STATUS_OK;
};

using FileInStream = LinuxFileInStream;

///
/// Linux implementation for a stream that writes to a file
///
struct LinuxFileOutStream final : IOutStream<LinuxFileOutStream>, NonCopyable
{
    constexpr LinuxFileOutStream(LinuxFileOutStream&& other) = default;
    constexpr LinuxFileOutStream& operator=(LinuxFileOutStream&& other) = default;

    int _fd = 0;
    StreamStatus _status = STATUS_OK;
    Array<u8> _buffer;
    usize _bufferUsed = 0;


    ///
    /// Creates a file descriptor for writing, assuming file exists
    /// @param path Absolute path to the file
    /// @param bufferCapacity An optional capacity for the buffer, default 4KB
    ///
    explicit LinuxFileOutStream(StringRef const& path, Optional<usize> const& bufferCapacity = None)
        : _buffer(bufferCapacity.valueOr<usize>(4_KB))
    {
        // mode_t mode = S_IRUSR | S_IWUSR;
        auto const result =
            static_cast<i64>(LinuxSyscall(LinuxSyscall.open, reinterpret_cast<u64>(path.cstr()), O_WRONLY));

        if (result < 0 && result > -0x1000) {
            auto const err = static_cast<int>(-result);
            LinuxFileStreamHelper::setStatusFromErrno(_status, err);
            _fd = -1;
        } else {
            _fd = static_cast<int>(static_cast<unsigned>(result));
        }
    }

    /// Destructor
    ~LinuxFileOutStream()
    {
        if (_fd < 3) {
            return;
        }
        // TODO: maybe warn if file destroyed without flushing? if _bufferUsed != 0 ...
        this->flush();
        (void)this->close();
    }

    ///
    LinuxFileOutStream& writeBytes(void const* data, usize const sizeBytes)
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
    bool doWrite(void const* buffer, size_t const size)
    {
        if (size == 0) {
            return true;
        }
        auto const r = static_cast<ssize_t>(
            LinuxSyscall(LinuxSyscall.write, static_cast<usize>(_fd), reinterpret_cast<usize>(buffer), size));
        if (r < 0) {
            auto const err = static_cast<int>(-r);
            _status = LinuxFileStreamHelper::setStatusFromErrno(_status, err);
            return false;
        }
        [[assume(r >= 0)]];
        if (size != static_cast<usize>(r)) {
            _status = StreamStatus(EADV);
            return false;
        }
        return true;
    };

    ///
    LinuxFileOutStream& flush()
    {
        (void)doWrite(_buffer.data(), _bufferUsed);
        return *this;
    }

    ///
    /// Closes the file descriptor.
    ///
    Result<StreamStatus, StreamStatus> close()
    {
        if (auto const result = static_cast<isize>(LinuxSyscall(LinuxSyscall.close, static_cast<usize>(_fd)));
            result < 0)
        {
            auto const err = static_cast<int>(-result);
            return Err(LinuxFileStreamHelper::setStatusFromErrno(_status, err));
        }
        return Ok(_status = STATUS_OK);
    }

    ///
    /// Returns the status.
    ///
    StreamStatus status() const { return _status; }
};

///
/// A stream that writes to a file.
///
struct FileOutStream : Optional<LinuxFileOutStream>
{
    using Optional::Optional;

    ///
    /// Creates a file descriptor for writing, assuming file exists
    /// @param path Absolute path to the file
    /// @param bufferCapacity An optional capacity for the buffer, default 4KB
    ///
    explicit FileOutStream(StringRef const& path, Optional<usize> const& bufferCapacity = 4_KB)
        : Optional(LinuxFileOutStream(path, bufferCapacity))
    {}

    ~FileOutStream() = default;
};

#endif
