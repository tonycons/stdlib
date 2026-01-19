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


///
/// A non owning reference to a string.
///
struct StringRef : public ArrayRef<char>, public IEquatable<StringRef>, public IComparable<StringRef>
{
private:
    using Base = ArrayRef<char>;

public:
    using ArrayRef<char>::ArrayRef;
    using IEquatable<StringRef>::operator==;
    using IEquatable<StringRef>::operator!=;

    ///
    /// Constructs a StringRef from a C-style null terminated string. Be careful that the string is actually null
    /// terminated
    ///
    StringRef(char const* str, usize len)
        : Base(str, len + 1)
    {
        // Assert(str[len] == '\0');
    }

    ///
    /// Constructs a StringRef from a C-style null terminated string.
    ///
    constexpr StringRef(char const* str)
        : Base(str, CArrays::stringLen(str) + 1) /* Arrayref expects the length including null terminator */
    {}

    ///
    /// Constructs a StringRef from a char.
    /// How is this possible? It references a static, one-byte null terminated C string from a lookup table.
    ///
    constexpr StringRef(char ch)
        : Base(::cm::Data::oneCharStringTable(ch), 2)
    {}

    constexpr bool equals(StringRef const& s) const { return Base::equals(s); }
    constexpr bool equalsTimesafe(StringRef const& s) { return Base::equalsTimesafe(s); }
    constexpr int compare(StringRef const& s) const { return Base::compare(s); }
    constexpr int compareTimesafe(StringRef const& s) { return Base::compareTimesafe(s); }

    constexpr usize length() const { return usize(::cm::max(0, isize(Base::length()) - 1)); }

    constexpr usize sizeBytes() const { return this->length() * sizeof(char); }

    constexpr static void outputString(StringRef const& s, auto const& out)
    {
        for (char c : s) {
            out(c);
        }
    }

    ///
    /// Returns a pointer to the string data. The returned pointer should not outlive ``*this``.
    ///
    constexpr char const* cstr() const noexcept { return this->data(); }

    ///
    /// Compare two strings, ignoring case.
    ///

    ///
    /// The hash
    ///
    template<typename Hasher>
    constexpr auto hash(auto seed)
    {
        // (do not use memory address for seed -- otherwise equivalent strings could produce different hash values!!
        return Hasher::hashCString(this->data(), seed);
    }
};


}  // namespace cm

#endif
