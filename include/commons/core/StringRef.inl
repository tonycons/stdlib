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
#warning Do not include this file directly; include "core.hh" instead
#else


namespace cm {

class StringBuf;

/// Represents a non owning reference to a string.
struct StringRef final : ArrayRef<char>
{
    using ArrayRef::ArrayRef;

    /// Constructs a StringRef from a C-style null terminated string. Be careful that the string is actually null
    /// terminated
    constexpr StringRef(char const* str, usize len)  // NOLINT
        : ArrayRef(str, len + 1)
    {
        // Assert(str[len] == '\0');
    }

    /// Constructs a StringRef from a C-style null terminated string.
    constexpr StringRef(char const* str)             // NOLINT
        : ArrayRef(str, CArrays::stringLen(str) + 1) /* ArrayRef expects the length including null terminator */
    {}

    /// Constructs a StringRef from a char.
    /// How is this possible? It references a static, one-byte null terminated C string from a lookup table.
    constexpr StringRef(char ch)  // NOLINT
        : ArrayRef(::cm::Data::oneCharStringTable(ch), 2)
    {}

    /// @return The length of the string
    [[nodiscard]] constexpr usize length() const override
    {
        return static_cast<usize>(::cm::max(0, static_cast<isize>(ArrayRef::length()) - 1));
    }

    /// Returns a pointer to the string data. The returned pointer should not outlive ``*this``.
    [[nodiscard]] constexpr char const* cstr() const noexcept { return this->data(); }

    template<typename Hasher>
    constexpr auto hash(auto seed)
    {
        // do not use memory address for seed -- otherwise equivalent strings could produce different hash values!!
        return Hasher::hashCString(this->data(), seed);
    }

    /// Pretty self-explanatory
    constexpr static void outputString(StringRef const& s, auto const& out)
    {
        for (char c : s) {
            out(c);
        }
    }
};

class Printable {
public:
    virtual constexpr ~Printable() = default;
    virtual constexpr void output(StringBuf& result) const = 0;
};

template<typename T>
class PrintableT final : public Printable {
public:
    T* ptr;

    constexpr PrintableT(T const& ref)  // NOLINT
        : ptr(&const_cast<T&>(ref))
    {}
    constexpr void output(StringBuf& result) const override;
};

template<typename T>
[[maybe_unused]] PrintableT(T const&) -> PrintableT<T>;


///
/// An owning string data structure.
/// A base class providing functionality shared between String and FixedString.
/// Derived must implement: insert and erase
///
class StringBuf : public IArray<char> {
public:
    constexpr StringBuf() = default;
    constexpr StringBuf(StringBuf const&) = default;

    virtual constexpr char& operator[](Index const& i) = 0;
    virtual constexpr void erase(Index i, usize n) = 0;
    virtual constexpr void insert(Index i, StringRef const& s) = 0;
    virtual constexpr void fmt(StringRef const& fmt, ArrayRef<RefWrapper<Printable const>> const& args) = 0;

    [[nodiscard]] virtual constexpr auto cstr() -> char* = 0;
    [[nodiscard]] virtual constexpr auto cstr() const -> char const* = 0;

    [[nodiscard]] virtual constexpr auto find(StringRef const& substr, usize baseIndex) const -> Optional<usize> = 0;

    [[nodiscard]] constexpr bool empty() const { return length() == 0; }

    /// Set the string to empty.
    constexpr void clear() { erase(0, length()); }

    /// Alias for insert(length(), s);
    constexpr void append(StringRef const& s) { return insert(length(), s); }

    /// Alias for append.
    constexpr void push(StringRef const& s) { return insert(length(), s); }

    /// Removes the final character from the string
    /// @return The character that was removed from the end of the string
    constexpr char pop()
    {
        char const c = (*this)[-1];
        erase(-1, 1);
        return c;
    }

    /// Replace every occurrence of a substring with a replacement. This is NOT a regex.
    /// @param substr The substring to replace
    /// @param replacement The replacement string
    /// @note If used on FixedString, replacements that cause the string length to exceed capacity will be ignored.
    constexpr void replace(StringRef const& substr, StringRef const& replacement) &
    {
        usize baseIndex = 0;
        Optional<usize> index = None;
        while ((index = find(substr, baseIndex)) != None) {
            erase(index.ref(), substr.length());
            insert(index.ref(), replacement);
            baseIndex += replacement.length();
        }
    }

    /// Removes the final element if it is equal to some value.
    /// @note If the element is not a simple data type and is removed, its destructor is called.
    /// @param refValue The value to remove.
    constexpr void removeSuffix(char refValue)
    {
        if (length() != 0 && (*this)[length() - 1] == refValue)
            erase(length() - 1, 1);
    }

    /// Returns a reference to a segment of an array from startIndex (inclusive) to endIndex (exclusive)
    constexpr StringRef slice(usize startIndex, usize endIndex)
    {
        UNSAFE_BEGIN;
        Assert(endIndex <= length());
        Assert(startIndex < endIndex);
        return StringRef(data() + startIndex, endIndex - startIndex);
        UNSAFE_END;
    }

    /// Builds a string according to a format specifier.
    /// tldr; similar to sprintf, except a backtick ` denotes an argument.
    /// Formal explanation: the string will equal the format specifier with each occurrence of the Nth format character
    /// (a backtick `) replaced with the string representation of the Nth argument to the function.
    /// @param sFmt format specifier
    /// @param args arguments
    template<typename... Args>
    constexpr static void fmt(StringRef const& sFmt, Args... args)
    {
        fmt(sFmt, {PrintableT(args)...});
    }

    /// This needs no explanation
    constexpr static void outputString(StringBuf const& self, auto out)
    {
        for (char c : self) {
            out(c);
        }
    }


public:
    // constexpr long long toInteger()
    // {
    //     // FIXME: this is the bad atoi, use functions from intbase
    //     auto result = 0, negate = 0;
    //     auto s = begin();
    //     while (*s == ' ' || *s == '\t') {
    //         ++s;
    //     }
    //     if (*s == '+' && *(s.next()) != '-') {
    //         ++s;
    //     }
    //     if (*s == '-') {
    //         negate = 1;
    //         ++s;
    //     }
    //     for (; *s != '\0'; ++s) {
    //         switch (*s) {
    //         case '0' ... '9':
    //             if (__builtin_mul_overflow(result, 10, &result) || __builtin_add_overflow(result, -('0' - *s),
    //             &result))
    //                 return negate ? -2147483648 : 2147483647;
    //             continue;
    //         default: break;
    //         }
    //         break;
    //     }
    //     return negate ? -result : result;
    // }

    // constexpr double toDouble()
    // {
    //     if (self == "-inf") {
    //         return Double::NEG_INF;
    //     }
    //     if (self == "inf") {
    //         return Double::POS_INF;
    //     }
    //     if (self == "NaN") {
    //         return Double::QNAN;
    //     }
    //     auto it = self.begin();
    //     while (it != self.last() && *it != '.') {
    //         ++it;
    //     }
    //     auto wholePart = static_cast<double>(self.toInteger());

    // Derived a;
    // while (it != self.last()) {
    //     ++it;
    //     a.append(*it);
    // }
    // if (auto fracInt = a.toInteger(); fracInt != 0) {
    //     wholePart +=
    //         (static_cast<double>(fracInt) /
    //          (Double::pow(10.0, static_cast<int>(Double::floor(Double::log10(static_cast<double>(fracInt))))) *
    //           10.0));
    // }
    // return wholePart;
    // }
};


template<typename T>
constexpr void PrintableT<T>::output(StringBuf& result) const
{
    if constexpr (IsUnderlyingTypeOneOf<T, StringRef, char*, char>) {
        result.append(*ptr);
    } else {
        OutputString(*ptr, [&](char c) { result.append(c); });
    }
}


/// An owning string with a fixed-size storage buffer that is capable of expanding up to a given capacity.
/// @tparam Capacity The maximum length of the string
template<usize Capacity>
requires (Capacity > 1)
class FixedString : Iterable<FixedString<Capacity>>,
                    IEquatable<FixedString<Capacity>>,
                    IComparable<FixedString<Capacity>> {
    UintRanged<Capacity> _length = 0;
    char _str[Capacity]{};
    static_assert(sizeof(_str) == Capacity * sizeof(char));

public:
    using Iterable<FixedString>::Iterable;
    using IEquatable<FixedString>::operator==;
    using IEquatable<FixedString>::operator!=;

    constexpr FixedString() = default;

    /// Az
    // NOLINTNEXTLINE
    constexpr FixedString(char const* str)
    {
        if consteval {
            for (usize i = 0; i < Capacity - 1 && *str != '\0'; i++) {
                _str[i] = str[i];
            }
        } else {
            __builtin_strncpy(_str, str, (Capacity - 1) * sizeof(char));
        }
    }

    /// A
    // NOLINTNEXTLINE
    constexpr FixedString(char ch) { _str[0] = ch; }

    constexpr bool equals(FixedString const& other) const
    {
        if consteval {
            return Iterable(*this).equals(other);
        } else {
            return __builtin_memcmp(_str, other._str, _length * sizeof(char)) == 0;
        }
    }

    constexpr int compare(FixedString const& other) const
    {
        if consteval {
            return Iterable(*this).compare(other);
        } else {
            return __builtin_memcmp(_str, other._str, _length * sizeof(char));
        }
    }


    [[gnu::format(printf, 1, 2)]]
    constexpr static FixedString cformat(char const* fmt_, ...)
    {
        FixedString result;

        __builtin_va_list args;
        __builtin_va_start(args, fmt_);

        __builtin_memset_inline(result._str, 0, sizeof(_str));
        result._length = static_cast<UintRanged<Capacity>>(
            min(Capacity - 1,
                static_cast<usize>(__builtin_vsnprintf(result._str, sizeof(_str) - sizeof(char), fmt_, args))));

        __builtin_va_end(args);
        return result;
    }

    constexpr static FixedString format(StringRef const& fmt, auto&&... args);


    constexpr auto data() const noexcept { return this->_str; }
    constexpr auto cstr() const noexcept { return this->data(); }
    constexpr usize length() const noexcept { return _length; }
    constexpr auto sizeBytes() const noexcept { return length() * sizeof(char); }
};


}  // namespace cm

#endif
