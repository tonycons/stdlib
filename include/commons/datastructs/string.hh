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

struct Printable
{
    virtual ~Printable() = default;
    virtual void output(String& result) const = 0;
};

template<typename T>
struct PrintableT final : Printable
{
    T* ptr;

    PrintableT(T const& ref)
        : ptr(&const_cast<T&>(ref))
    {}
    constexpr void output(String& result) const override;
};

template<typename T>
[[maybe_unused]] PrintableT(T const&) -> PrintableT<T>;


///
/// A dynamic, heap-allocated string
/// @see docs/String.md
///
class String : public LinearIteratorComponent<String, char>,  //
               public Iterable<String>,
               public IEquatable<String> {

    ByteVector _data;

public:
    constexpr static auto FORMAT_DELIMITER = '`';
    using Index = Union<usize, isize> const&;

    ///
    /// Constructor from a string literal.
    /// @param cstring The values
    ///
    template<long long N>
    String(char const (&cstring)[N])
        : String(cstring, ::cm::max(N - 1, 0))
    {}

    constexpr String()
        : _data("", 1)
    {}

    constexpr String(char c)
        : _data(Data::oneCharStringTable(c), 1)
    {}

    constexpr String(char const* str)
        : String(StringRef(str))
    {}

    constexpr String(char const* cstring, usize len)
        : _data(cstring, len + 1)
    {}

    constexpr String(StringRef const& sv)
        : String(sv.data(), sv.length())
    {}

    NODISCARD String(String const&) = default;
    NODISCARD String& operator=(String const&) = default;

    NODISCARD operator ArrayRef<char>() { return ArrayRef<char>(const_cast<char*>(cstr()), length()); }
    NODISCARD operator ArrayRef<char>() const { return ArrayRef<char>(const_cast<char*>(cstr()), length()); }
    NODISCARD operator StringRef() { return StringRef(const_cast<char*>(cstr()), length()); }
    NODISCARD operator StringRef() const { return StringRef(const_cast<char*>(cstr()), length()); }

    NODISCARD inline usize getActualIndex(Index i) const
    {
        return i.match(
            [&](usize k) { return k; },
            [&](isize k) {  // Possible negative index
                return k < 0 ? usize(isize(length()) + k) : usize(k);
            });
    }

    NODISCARD char& operator[](Index i)
    {
        UNSAFE(u8* r = &_data.data()[getActualIndex(i) * sizeof(char)]);
        return *reinterpret_cast<char*>(r);
    }

    NODISCARD char operator[](Index i) const
    {
        UNSAFE(u8 const* r = &_data.data()[getActualIndex(i) * sizeof(char)]);
        return *reinterpret_cast<char const*>(r);
    }

    NODISCARD constexpr usize length() const { return _data.length() - 1; }

    NODISCARD constexpr usize sizeBytes() const { return length() * sizeof(char); }

    NODISCARD constexpr bool empty() const { return length() == 0; }

    NODISCARD constexpr char const* data() const { return reinterpret_cast<char const*>(_data.data()); }

    NODISCARD constexpr char const* cstr() const { return this->data(); }

    NODISCARD constexpr bool equals(StringRef value) const { return StringRef(*this).equals(value); }

    void ensureNullTermination();

    void erase(Index i, usize n) &
    {
        usize actualIndex = getActualIndex(i);
        _data.erase(actualIndex * sizeof(char), n * sizeof(char));
    }

    void insert(Index i, StringRef const& s) &
    {
        usize actualIndex = getActualIndex(i);
        _data.insert(actualIndex * sizeof(char), s.data(), s.sizeBytes());
    }


    void insertf(Index i, u64 value);


    /**
     * TODO
     */
    template<unsigned N, typename... Args>
    NODISCARD FORCEINLINE static String fmt(char const (&sFmt)[N], Args... args)
    {
        return _fmt(sFmt, {PrintableT(args)...});
    }

    /**
     * TODO
     */
    // NODISCARD FORCEINLINE static String valueOf(auto const& val) { return ToString(val); }

    FORCEINLINE void append(StringRef s) & { return insert(length(), s); }
    FORCEINLINE void push(StringRef s) & { return insert(length(), s); }
    inline char pop() &
    {
        char c = (*this)[-1];
        erase(-1, 1);
        return c;
    }

    NODISCARD FORCEINLINE String replace(StringRef s, StringRef r) const { return String(*this).replace(s, r); }
    NODISCARD FORCEINLINE String replace(StringRef s, StringRef r) && { return (replace(s, r), *this); }

    void replace(StringRef substr, StringRef replacement) &
    {
        usize baseIndex = 0;
        Optional<usize> index = None;
        while ((index = this->find(substr, baseIndex)) != None) {
            erase(index.value(), substr.length());
            insert(index.value(), replacement);
            baseIndex += replacement.length();
        }
    }


    NODISCARD FORCEINLINE String erase(Index i, usize n) const { return String(*this).erase(i, n); }
    NODISCARD FORCEINLINE String erase(Index i, usize n) && { return (erase(i, n), *this); }

    NODISCARD FORCEINLINE String insert(Index i, StringRef s) const { return String(*this).insert(i, s); }
    NODISCARD FORCEINLINE String insert(Index i, StringRef s) && { return (insert(i, s), *this); };

    NODISCARD FORCEINLINE String append(StringRef s) const { return String(*this).append(s); }
    NODISCARD FORCEINLINE String append(StringRef s) && { return (append(s), *this); }


    ///
    /// Removes the final element if it is equal to some value.
    /// @note If the element is not a simple data type and is removed, its destructor is called.
    /// @param refValue The value to remove.
    ///
    void removeSuffix(char refValue) &
    {
        if (length() != 0 && (*this)[length() - 1] == refValue) {
            erase(length() - 1, 1);
        }
    }


    ///
    /// Returns a reference to a segment of an array from startIndex (inclusive) to endIndex (exclusive)
    ///
    constexpr StringRef slice(usize startIndex, usize endIndex) const
    {
        UNSAFE_BEGIN;
        Assert(endIndex <= length());
        Assert(startIndex < endIndex);
        return StringRef(this->data() + startIndex, endIndex - startIndex);
        UNSAFE_END;
    }

public:
    template<int = 0>
    long long toInteger() const
    {
        // FIXME: this is the bad atoi, use functions from intbase
        auto result = 0, negate = 0;
        auto s = begin();
        while (*s == ' ' || *s == '\t') {
            ++s;
        }
        if (*s == '+' && *(s.next()) != '-') {
            ++s;
        }
        if (*s == '-') {
            negate = 1;
            ++s;
        }
        for (; *s != '\0'; ++s) {
            switch (*s) {
            case '0' ... '9':
                if (__builtin_mul_overflow(result, 10, &result) || __builtin_add_overflow(result, -('0' - *s), &result))
                    return negate ? -2147483648 : 2147483647;
                continue;
            default: break;
            }
            break;
        }
        return negate ? -result : result;
    }


    template<int = 0>
    double toDouble() const
    {
        if (*this == "-inf") {
            return -__builtin_huge_val();
        } else if (*this == "inf") {
            return __builtin_huge_val();
        } else if (*this == "NaN") {
            return __builtin_nan("");
        }
        auto it = begin();
        while (it != last() && *it != '.') {
            ++it;
        }
        auto wholePart = double(toInteger());

        String a;
        while (it != last()) {
            ++it;
            a.append(*it);
        }
        if (auto fracInt = a.toInteger(); fracInt != 0) {
            wholePart +=
                (double(fracInt) / (Double::pow(10.0, int(__builtin_floor(Double::log10(double(fracInt))))) * 10.0));
        }
        return wholePart;
    }

private:
    template<int = 0>
    static String _fmt(StringRef const& format, ArrayRef<RefWrapper<Printable const>> const& objects)
    {
        String result;
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
        return result;
    }
};


template<typename T>
constexpr inline void PrintableT<T>::output(String& result) const
{
    if constexpr (IsUnderlyingTypeOneOf<T, String, StringRef, char*, char>) {
        result.append(*ptr);
    } else {
        OutputString(*ptr, [&](char c) { result.append(c); });
    }
}


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
