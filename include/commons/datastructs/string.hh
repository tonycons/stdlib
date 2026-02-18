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
#ifndef __inline_core_header__
#warning Do not include this file directly; include "datastructs.hh" instead
#else


namespace cm {


class String;


///
/// A heap-allocated, resizable string
/// @see docs/String.md
///
class String final : public StringBuf {

    ByteVector _data;

public:
    constexpr static auto FORMAT_DELIMITER = '`';


    template<long long N>
    String(char const (&cstring)[N])
        : String(cstring, ::cm::max(N - 1, 0))
    {}

    constexpr String()
        : _data("", 1)
    {}

    ~String() override = default;

    constexpr String(char c)
        : _data(Data::oneCharStringTable(c), 1)
    {}

    constexpr String(char const* str)
        : String(StringRef(str))
    {}

    constexpr String(char const* cstring, usize len)
        : _data(cstring, len + 1)
    {}

    constexpr String(StringRef const& sv)  // NOLINT
        : String(sv.data(), sv.length())
    {}

    ///
    ///
    [[nodiscard]] String(String const&) = default;

    ///
    ///
    [[nodiscard]] String& operator=(String const&) = default;

    ///
    ///
    [[nodiscard]] char& operator[](Index i) override
    {
        UNSAFE(u8* r = &_data.data()[getActualIndex(i) * sizeof(char)]);
        return *reinterpret_cast<char*>(r);
    }

    ///
    ///
    [[nodiscard]] char operator[](Index i) const override
    {
        UNSAFE(u8 const* r = &_data.data()[getActualIndex(i) * sizeof(char)]);
        return *reinterpret_cast<char const*>(r);
    }

    ///
    ///
    void erase(Index i, usize n) override
    {
        usize const actualIndex = getActualIndex(i);
        _data.erase(actualIndex * sizeof(char), n * sizeof(char));
    }

    ///
    ///
    void insert(Index i, StringRef const& s) override
    {
        usize const actualIndex = getActualIndex(i);
        _data.insert(actualIndex * sizeof(char), s.data(), s.sizeBytes());
    }

    ///
    ///
    void fmt(StringRef const& format, ArrayRef<RefWrapper<Printable const>> const& objects) override
    {
        auto& result = *this;
        result.clear();
        auto fmtIter = format.begin();
        auto objIter = objects.begin();

        while (fmtIter.isNotEnd() && *fmtIter != '`') {
            result.append(*fmtIter);
            ++fmtIter;
        }
        while (objIter.isNotEnd()) {
            Assert(fmtIter != format.end() && *fmtIter == '`', "More arguments than specified in format string");
            String str;
            (*objIter)->output(str);
            result.append(str);
            ++objIter;
            ++fmtIter;
            while (fmtIter.isNotEnd() && *fmtIter != '`') {
                result.append(*fmtIter);
                ++fmtIter;
            }
        }
    }

    ///
    ///
    template<typename... Args>
    static String fmt(StringRef const& format, Args&&... args)
    {
        String result;
        result.fmt(format, {PrintableT(args)...});
        return result;
    }

    ///
    Optional<usize> find(StringRef const& substr, usize baseIndex) const override
    {
        (void)substr;
        (void)baseIndex;
        ;  // TODO
        return None;
    }


    [[nodiscard]] usize getActualIndex(Index i) const
    {
        return i.match(
            [&](usize k) { return k; },
            [&](isize k) {  // Possible negative index
                return k < 0 ? usize(isize(length()) + k) : usize(k);
            });
    }

    [[nodiscard]] constexpr operator ArrayRef<char>() { return ArrayRef(cstr(), length()); }
    [[nodiscard]] constexpr operator ArrayRef<char>() const { return ArrayRef(cstr(), length()); }
    [[nodiscard]] constexpr operator StringRef() { return StringRef(cstr(), length()); }
    [[nodiscard]] constexpr operator StringRef() const { return StringRef(cstr(), length()); }
    [[nodiscard]] constexpr usize length() const override { return _data.length() - 1; }

    [[nodiscard]] constexpr char* data() override { return reinterpret_cast<char*>(_data.data()); }
    [[nodiscard]] constexpr char const* data() const override { return reinterpret_cast<char const*>(_data.data()); }

    [[nodiscard]] constexpr char* cstr() override { return this->data(); }
    [[nodiscard]] constexpr char const* cstr() const override { return this->data(); }
    [[nodiscard]] constexpr bool equals(StringRef const& value) const { return StringRef(*this).equals(value); }
};


template<usize N>
struct FormatLiteral
{
    char fmt[N];

    // Constructor to capture the string literal into the struct
    constexpr inline FormatLiteral(char const (&str)[N])
    {
        for (usize i = 0; i < N; ++i)
            fmt[i] = str[i];
    }

    constexpr inline usize size() const { return N - 1; }

    // Use a variadic operator() to handle formatting arguments
    template<typename... Args>
    inline String operator()(Args&&... args) const
    {
        // Replace with your String::fmt logic
        return String::fmt(fmt, Forward<Args>(args)...);
    }
};

template<usize N>
FormatLiteral(char const (&str)[N]) -> FormatLiteral<N>;

template<FormatLiteral L>
constexpr inline auto operator""_fmt()
{
    return L;
}


// static_assert(IsDerivedFrom<Array<char>, String>);

}  // namespace cm
#endif
