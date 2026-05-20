/*
   Copyright 2026 Anthony A. Constantinescu.

   Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
   in compliance with the License. You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software distributed under the License
   is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
   or implied. See the License for the specific language governing permissions and limitations under
   the License.

   File: commons/core.hh
   Description: Exports the core features of this library
*/

#pragma once

#include "commons/core/initializer_list.hh"
#include <commons/core/array.hh>
#include <commons/core/assert.hh>
#include <commons/resources/OneCharStringTable.hh>

namespace cm {

///
/// Represents a non owning reference to a string.
///
template<typename T>
struct TStringRef final : public ArrayRef<T>
{
    using ArrayRef<T>::ArrayRef;

    constexpr TStringRef(char const* str, usize len)  // NOLINT
        : ArrayRef<T>(len + 1, str)
    {
        Assert(str[len] == '\0');
    }

    constexpr TStringRef(char const* str)
        : ArrayRef<T>(cStringLen(str) + 1, str)
    {}

    constexpr TStringRef(char ch)  // NOLINT
        : ArrayRef<T>(2, ::cm::Data::oneCharStringTable(ch))
    {}

    template<typename Hasher>
    constexpr auto hash(auto seed)
    {
        // do not use memory address for seed -- otherwise equivalent strings could produce different hash values!!
        return Hasher::hashCString(this->data(), seed);
    }

    constexpr static void outputString(TStringRef const& s, auto const& out)
    {
        for (char c : s) {
            out(c);
        }
    }

    constexpr usize length() const
    {
        return static_cast<usize>(::cm::max(0, static_cast<isize>(ArrayRef<T>::length()) - 1));
    }

    [[gnu::format(printf, 3, 4)]]
    constexpr void nprintf(usize n, char const* fmt_, ...) requires (!IsConst<T>)
    {
        __builtin_va_list args;
        __builtin_va_start(args, fmt_);
        clear(n);
        ArrayRef<T>::_data._length =
            ::cm::min(ArrayRef<T>::_data._length, static_cast<usize>(__builtin_vsnprintf(cstr(), n, fmt_, args)));
        __builtin_va_end(args);
    }

    constexpr void clear(usize n = 0) requires (!IsConst<T>)
    {
        //$validate(cstr());
        __builtin_memset(cstr(), 0, n == 0 ? length() : n);
    }

    constexpr char const* cstr() const { return this->data(); }
};

using StringRef = TStringRef<char const>;
using MutableStringRef = TStringRef<char>;

///
/// An owning string with a fixed-size storage buffer that is capable of expanding up to a given capacity.
/// @tparam Capacity The maximum length of the string
///
template<usize Capacity>
requires (Capacity > 1)
class FixedString : public Iterable<FixedString<Capacity>>,
                    public IEquatable<FixedString<Capacity>>,
                    public IComparable<FixedString<Capacity>> {
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
        for (usize i = 0; i < Capacity - 1 && str[i] != '\0'; i++) {
            _str[i] = str[i];
            _length++;
        }
    }

    /// A
    // NOLINTNEXTLINE
    constexpr FixedString(char ch)
    {
        _str[0] = ch;
        _str[1] = '\0';
        _length = 1;
    }

    constexpr bool equals(FixedString const& other) const
    {
        if consteval {
            return Iterable(*this).equals(other);
        } else {
            return memcmp(_str, other._str, _length * sizeof(char)) == 0;
        }
    }

    constexpr int compare(FixedString const& other) const
    {
        if consteval {
            return Iterable(*this).compare(other);
        } else {
            return memcmp(_str, other._str, _length * sizeof(char));
        }
    }


    [[gnu::format(printf, 1, 2)]]
    constexpr static FixedString cformat(char const* fmt_, ...)
    {
        FixedString result;

        va_list args;
        va_start(args, fmt_);
        memset(result._str, 0, sizeof(_str));

        using U = UintRanged<Capacity>;
        result._length = static_cast<U>(
            min(Capacity - 1, static_cast<usize>(vsnprintf(result._str, sizeof(_str) - sizeof(char), fmt_, args))));

        va_end(args);
        return result;
    }

    constexpr static FixedString format(StringRef const& fmt, auto&&... args);


    constexpr auto data() const noexcept { return this->_str; }
    constexpr auto cstr() const noexcept { return this->data(); }
    constexpr usize length() const noexcept { return _length; }
    constexpr auto sizeBytes() const noexcept { return length() * sizeof(char); }
    constexpr auto capacity() { return Capacity - 1; }
    constexpr auto capacityBytes() { return capacity() * sizeof(char); }
};

}  // namespace cm
