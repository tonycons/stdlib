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

    template<typename U>
    requires (!IsSame<U, T>)
    constexpr inline U val() const = delete;

    template<typename U>
    requires (!IsSame<U, T>)
    constexpr inline U const& ref() const = delete;

    template<typename U>
    requires (!IsSame<U, T>)
    constexpr inline U& ref() = delete;


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
    constexpr inline bool hasValue() const noexcept { return Base::template is<T>(); }
    constexpr inline bool operator==(NoneType const&) const noexcept { return !hasValue(); }

    ///
    /// Attempts to access a reference to the value. A trap occurs if there is no value.
    ///
    constexpr inline T const& ref() const { return Base::template ref<T>(); }
    constexpr inline RefRemoved<T>* operator->() const noexcept { return const_cast<RefRemoved<T>*>(&this->ref()); }


    ///
    /// Attemps to access a copy of the value. A trap occurs if there is no value.
    ///
    constexpr inline T val() const { return Base::template val<T>(); }

    ///
    /// Returns the value if it is contained, otherwise a default value.
    ///
    template<class U>
    constexpr inline T valueOr(U&& x) const&
    {
        return this->hasValue() ? this->val() : static_cast<T>(Forward<U>(x));
    }

    constexpr static void outputString(Optional const& self, auto const& out)
    {
        if (self.hasValue()) {
            OutputString(self.ref(), out);
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
