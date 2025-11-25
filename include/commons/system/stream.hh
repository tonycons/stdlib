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

namespace cm {

/**
 * Interface for streaming data to a target.
 */
class OutStream {
public:
    ///
    /// Bitmask representing the error states of the stream.
    ///
    enum Status : u64 {
        STATUS_OK = 0,
        STATUS_FAILED_INIT = (1 << 0),
        STATUS_FAILED_WRITE = (1 << 1),
        STATUS_FAILED_FLUSH = (1 << 2),
        STATUS_NOT_ALL_BYTES_FLUSHED = (1 << 3),

        // The last operation failed because the stream is invalid
        STATUS_ERR_INVALID = (1 << 4),

        // The last operation was interrupted by a signal before it could complete.
        STATUS_ERR_INTERRUPT = (1 << 5),

        // The last operation failed due to an I/O error.
        STATUS_ERR_IO = (1 << 6),

        // The last operation failed due to an unknown error.
        STATUS_ERR_UNKNOWN = (1 << 7),

        STATUS_ERR_ACCESS = (1 << 8),
        STATUS_ERR_BUSY = (1 << 9),
        STATUS_ERR_QUOTA = (1 << 10),

        STATUS_ERR_ALREADY_EXISTS = (1 << 11),

        // The operation failed because the type of the stream might not support that operation.
        STATUS_ERR_UNSUPPORTED = (1 << 12),

        STATUS_ERR_LOOP = (1 << 13),
        STATUS_ERR_LIMIT = (1 << 14),

        STATUS_ERR_NOT_FOUND = (1 << 15),
        STATUS_ERR_MEMORY = (1 << 16),


        // The last operation failed for any reason
        STATUS_ANY_ERR = 0xffff'ffff'ffff'fffe,
    };

    ///
    /// Represents the OS-dependent line separator.
    ///
    constexpr static StringValue LINE_SEPARATOR =
#ifdef _WIN32
        "\r\n";
#elif defined(__APPLE__) || defined(__MACH___)
        "\r";
#else
        "\n";
#endif

    virtual ~OutStream() = default;

    ///
    /// Adds a series of bytes to the currently pending data.
    /// @param data The data
    /// @param sizeBytes The data size
    ///
    virtual auto writeBytes(void const* data, size_t sizeBytes) -> OutStream& = 0;

    ///
    /// Sends all pending data to the target.
    ///
    virtual auto flush() -> OutStream& = 0;

    ///
    /// Closes the stream. Returns a bitmask indicating the success of the close operation.
    /// It's an error to write anything to the stream after close() is called and true is returned, even for streams
    /// where close() does nothing (such as StringStream).
    /// For streams where close() does nothing (such as StringStream), close() returns true
    ///
    inline virtual Result<Status, Status> close() { return Ok(STATUS_OK); }

    ///
    /// Get the general status of the stream
    ///
    inline virtual Status status() const { return STATUS_OK; }


    inline bool ok() { return status() == STATUS_OK; }

    ///
    /// Print a value to the stream.
    /// @param arg The value
    ///
    void print(auto const& value)
    {
        auto str = String::valueOf(value);
        writeBytes(str.cstr(), str.sizeBytes());
    }

    template<int N>
    void print(char const (&str)[N])
    {
        writeBytes(str, max(N - 1, 0));
    }


    /**
     * Print a text followed to the stream with a format specifier.
     * @param sFmt The format string
     * @param args The arguments
     */
    template<int N>
    void print(char const (&sFmt)[N], auto const&... args)
    {
        _print(sFmt, {AnyRefT(args)...});
    }

    /**
     * Print a text followed by a newline to the stream with a format specifier.
     * @param sFmt The format string
     * @param args The arguments
     */
    template<int N>
    void println(char const (&sFmt)[N], auto const&... args)
    {
        print(sFmt, args...);
        writeBytes(LINE_SEPARATOR.data(), LINE_SEPARATOR.sizeBytes());
    }

    template<int N, typename T>
    void println(char const (&sFmt)[N], std::initializer_list<T> const& args)
    {
        println<N>(sFmt, ArrayRef(args.begin(), args.size()));
    }

    /**
     * Print a value followed by a newline to the stream.
     * @param arg The value
     */
    void println(auto const& value)
    {
        print(value);
        writeBytes(LINE_SEPARATOR.data(), LINE_SEPARATOR.sizeBytes());
    }

    template<int N>
    void println(char const (&str)[N])
    {
        writeBytes(str, max(N - 1, 0));
        writeBytes(LINE_SEPARATOR.data(), LINE_SEPARATOR.sizeBytes());
    }

private:
    void _print(char const* sFmt, std::initializer_list<ConstRefWrapper<AnyRef>> const& objects);
};

///
/// Defines a stream that just writes to some string
///
struct StringStream : public OutStream
{
    StringStream() = delete;
    StringStream(String& to)
        : _to(to)
    {}
    ~StringStream() override = default;

    virtual OutStream& writeBytes(void const* data, size_t sizeBytes) override
    {
        _to.insert(_to.length(), String(static_cast<char const*>(data), sizeBytes));
        return *this;
    }

    virtual OutStream& flush() override { return *this; }

private:
    String& _to;
};


///
/// An Optional standard output stream.
/// It may be set to None on systems that don't have a standard output stream.
///
extern Optional<OutStream&> const stdout;

///
/// An Optional standard error stream.
/// It may be set to None on systems that don't have a standard error stream.
///
extern Optional<OutStream&> const stderr;

struct Shell
{
    virtual ~Shell() = default;
    virtual int execute(String const& command, Optional<OutStream&> const& output = stdout) = 0;
};

///
/// An Optional system shell e.g. Bash.
/// It may be set to None on systems that don't have a shell.
///
extern Optional<Shell&> const shell;

///
/// Creates a stream that writes to a file
///
struct FileOutStream : public Optional<OutStream&>
{
    using Optional<OutStream&>::Optional;

    FileOutStream(String const& path, Optional<usize> const& bufferCapacity = 4_KB);
    ~FileOutStream();
};

}  // namespace cm
#endif
