/*
   Copyright 2024 Anthony A. Constantinescu.

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

struct NoneType
{};

constexpr auto None = NoneType{};

///
/// @see docs/Option.md
///
template<typename T>
class Optional : public Union<T, NoneType> {
    using Base = Union<T, NoneType>;

public:
    using Union<T, NoneType>::Union;

    ///
    /// Default constructor, sets to None
    ///
    constexpr Optional()
        : Base(NoneType{})
    {}

    constexpr Optional(T const& val)
        : Base(val)
    {}
    constexpr Optional(NoneType const&)
        : Base(NoneType{})
    {}

    ///
    /// Returns true if the Option contains a value.
    ///
    constexpr inline operator bool() const noexcept { return Base::template is<T>(); }
    constexpr inline bool hasValue() const noexcept { return this->operator bool(); }
    constexpr inline bool operator==(NoneType const&) const noexcept { return !hasValue(); }

    ///
    /// Attempts to access the value. A trap occurs if there is no value.
    ///
    constexpr inline T const& value() const { return Base::template get<T>(); }
    constexpr inline RefRemoved<T>* operator->() const noexcept { return const_cast<RefRemoved<T>*>(&this->value()); }


    ///
    /// Returns the value if it is contained, otherwise a default value.
    ///
    template<class U>
    constexpr inline T valueOr(U&& x) const&
    {
        return this->hasValue() ? this->value() : static_cast<T>(Forward<U>(x));
    }

    constexpr static void outputString(Optional const& self, auto const& out)
    {
        if (self.hasValue()) {
            OutputString(self.value(), out);
        } else {
            out('N');
            out('o');
            out('n');
            out('e');
        }
    }
};

}  // namespace cm

#endif
