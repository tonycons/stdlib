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

///
/// Interface for streaming data to a target.
///
class OutStream {
public:
    using Status = StreamStatus;

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

    virtual ~OutStream() = default;
    constexpr OutStream() = default;
    constexpr OutStream(OutStream const&) = default;
    constexpr OutStream& operator=(OutStream const&) = default;

    ///
    /// Adds a series of bytes to the currently pending data.
    /// @param data The data
    /// @param sizeBytes The data size
    ///
    virtual OutStream& writeBytes(void const* data, size_t sizeBytes) = 0;

    ///
    /// Sends all pending data to the target.
    ///
    virtual OutStream& flush() = 0;

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

    ///
    /// Returns true if the stream has no errors
    ///
    inline bool ok() { return status() == STATUS_OK; }


    /// =========================================================================================================================

    ///
    /// Print a value to the stream.
    /// @param arg The value
    ///
    inline void print(auto const& value)
    {
        _print('`', ArrayRef<RefWrapper<Printable const>>{RefWrapper<Printable const>(PrintableT(value))});
    }
    template<int N>
    inline void print(char const (&str)[N])
    {
        writeBytes(str, max(N - 1, 0));
    }

    ///
    /// Print a text followed to the stream with a format specifier.
    /// @param sFmt The format string
    /// @param args The arguments
    ///
    inline void print(StringRef const& sFmt, auto const&... args)
    {
        this->_print(sFmt, ArrayRef<RefWrapper<Printable const>>{(RefWrapper<Printable const>(PrintableT(args)))...});
    }

    /// =========================================================================================================================

    ///
    /// Print a value followed by a newline to the stream.
    /// @param arg The value
    ///
    inline void println(auto const& value)
    {
        this->print(value);
        this->print(LS);
    }
    template<int N>
    inline void println(char const (&str)[N])
    {
        this->writeBytes(str, max(N - 1, 0));
        this->writeBytes(LS.data(), LS.sizeBytes());
    }

    ///
    /// Print a text followed by a newline to the stream with a format specifier.
    /// @param sFmt The format string
    /// @param args The arguments
    ///
    inline void println(StringRef sFmt, auto const&... args) { print(sFmt, args...), print(LS); }


private:
    void _print(StringRef const& format, ArrayRef<RefWrapper<Printable const>> const& objects)
    {
        auto fmtIter = format.begin();
        auto objIter = objects.begin();

        while (fmtIter.isNotEnd() && *fmtIter != '`') {
            char ch = *fmtIter;
            this->writeBytes(&ch, sizeof(char));
            ++fmtIter;
        }
        while (objIter.isNotEnd()) {
            Assert(fmtIter != format.end() && *fmtIter == '`', "More arguments than specified in format string");

            String str;
            (*objIter)->output(str);
            this->writeBytes(str.cstr(), str.sizeBytes());

            ++objIter;
            ++fmtIter;

            while (fmtIter.isNotEnd() && *fmtIter != '`') {
                char ch = *fmtIter;
                this->writeBytes(&ch, sizeof(char));
                ++fmtIter;
            }
        }
    }
};

}  // namespace cm
#endif
