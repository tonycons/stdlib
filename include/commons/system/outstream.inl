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

namespace cm {

enum class FileOpenMode : u8 {
    Read = (1 << 0),    // 'r'
    Write = (1 << 1),   // 'w'
    Append = (1 << 2),  // 'a'
    Update = (1 << 3),  // '+'
    Binary = (1 << 4)   // 'b'
};

constexpr FileOpenMode operator|(FileOpenMode lhs, FileOpenMode rhs) { return FileOpenMode(u8(lhs) | u8(rhs)); }
constexpr bool operator&(FileOpenMode lhs, FileOpenMode rhs) { return bool(u8(lhs) & u8(rhs)); }


///
/// Interface for streaming data out of something
///
template<typename Derived>
class InStream {
public:
    using Status = StreamStatus;

    constexpr ~InStream() = default;
    constexpr InStream() = default;
    constexpr InStream(InStream const&) = default;
    constexpr InStream& operator=(InStream const&) = default;
};

///
/// Interface for streaming data to a target.
///
template<typename Derived>
class IOutStream {
public:
    ///
    /// Represents the OS-dependent line separator.
    ///
    constexpr static StringRef LS =
#ifdef _WIN32
        "\r\n";
#elif defined(__APPLE__) || defined(__MACH___)
        "\r";
#else
        "\n";
#endif

    constexpr ~IOutStream() = default;
    constexpr IOutStream() = default;
    constexpr IOutStream(IOutStream const&) = default;
    constexpr IOutStream& operator=(IOutStream const&) = default;

    ///
    /// Adds a series of bytes to the currently pending data.
    /// @param data The data
    /// @param sizeBytes The data size
    ///
    constexpr inline auto& writeBytes(void const* data, size_t sizeBytes) const
    {
        return static_cast<Derived*>(const_cast<IOutStream*>(this))->writeBytes(data, sizeBytes);
    }

    ///
    /// Sends all pending data to the target.
    ///
    constexpr auto& flush() const { return static_cast<Derived*>(const_cast<IOutStream*>(this))->flush(); }

    ///
    /// Closes the stream. Returns a bitmask indicating the success of the close operation.
    /// It's an error to write anything to the stream after close() is called and true is returned, even for streams
    /// where close() does nothing (such as StringStream).
    /// For streams where close() does nothing (such as StringStream), close() returns true
    ///
    [[nodiscard]] constexpr Result<StreamStatus, StreamStatus> close() const
    {
        return static_cast<Derived*>(const_cast<IOutStream*>(this))->close();
    }

    ///
    /// Get the general status of the stream
    ///
    [[nodiscard]] constexpr StreamStatus status() const
    {
        return static_cast<Derived*>(const_cast<IOutStream*>(this))->status();
    }

    ///
    /// Returns true if the stream has no errors
    ///
    [[nodiscard]] constexpr bool ok() const { return status() == STATUS_OK; }

    ///
    /// Print a value to the stream.
    /// @param value The value
    void print(auto const& value) const
    {
        auto str = String::fmt("`", value);
        writeBytes(str.cstr(), str.sizeBytes());
        //_print('`', ArrayRef<RefWrapper<Printable const>>{RefWrapper<Printable const>(PrintableT(value))});
    }

    /// Print a string literal to the stream
    template<int N>
    void print(char const (&str)[N]) const
    {
        writeBytes(str, max(N - 1, 0));
    }

    /// Print a text followed to the stream with a format specifier.
    /// @param sFmt The format string
    /// @param args The arguments
    void print(StringRef const& sFmt, auto const&... args) const
    {
        auto str = String::fmt(sFmt, args...);
        writeBytes(str.cstr(), str.sizeBytes());
        //_print(sFmt, ArrayRef<RefWrapper<Printable const>>{(RefWrapper<Printable const>(PrintableT(args)))...});
    }

    /// Print a value followed by a newline to the stream.
    /// @param value The value
    void println(auto const& value) const
    {
        print(value);
        print(LS);
    }

    /// Prints a string literal to the stream followed by a newline.
    template<int N>
    void println(char const (&str)[N]) const
    {
        writeBytes(str, max(N - 1, 0));
        print(LS);
    }

    /// Print a text followed by a newline to the stream with a format specifier.
    /// @param sFmt The format string
    /// @param args The arguments
    void println(StringRef sFmt, auto const&... args) const
    {
        print(sFmt, args...);
        print(LS);
    }
};

}  // namespace cm
#endif
