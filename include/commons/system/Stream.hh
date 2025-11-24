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
class Stream {
public:
    /**
     * Represents the error states of the stream.
     */
    enum : uint64_t {
        STATE_FAILED_WRITE = 1,
        STATE_FAILED_FLUSH = 2,
        STATE_NOT_ALL_BYTES_FLUSHED = 4
    };

    /**
     * Represents the OS-dependent line separator.
     */
    constexpr static StringValue LINE_SEPARATOR =
#ifdef _WIN32
        "\r\n";
#elif defined(__APPLE__) || defined(__MACH___)
        "\r";
#else
        "\n";
#endif

    virtual ~Stream() = default;

    /**
     * Adds a series of bytes to the currently pending data.
     * @param data The data
     * @param sizeBytes The data size
     */
    virtual auto writeBytes(void const* data, size_t sizeBytes) -> Stream& = 0;

    /**
     * Sends all pending data to the target.
     */
    virtual auto flush() -> Stream& = 0;

    /**
     * Print a value to the stream.
     * @param arg The value
     */
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
struct StringStream : public Stream
{
    StringStream(String& to)
        : _to(to)
    {}
    ~StringStream() override = default;

    virtual Stream& writeBytes(void const* data, size_t sizeBytes) override
    {
        _to.insert(_to.length(), String(static_cast<char const*>(data), sizeBytes));
        return *this;
    }

    virtual Stream& flush() override { return *this; }

private:
    String& _to;
};

///
/// An Optional standard output stream.
/// It may be set to None on systems that don't have a standard output stream.
///
extern Optional<Stream&> const stdout;

///
/// An Optional standard error stream.
/// It may be set to None on systems that don't have a standard error stream.
///
extern Optional<Stream&> const stderr;

struct Shell
{
    virtual ~Shell() = default;
    virtual int execute(String const& command, Optional<Stream&> const& output = stdout) = 0;
};

///
/// An Optional system shell e.g. Bash.
/// It may be set to None on systems that don't have a shell.
///
extern Optional<Shell&> const shell;


}  // namespace cm
#endif
