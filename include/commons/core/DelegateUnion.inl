/*
   Copyright 2025 Anthony A. Constantinescu.

   Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
   in compliance with the License. You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software distributed under the License
   is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
   or implied. See the License for the specific language governing permissions and limitations under
   the License.

   File: core/union
   Description:
    Implements a Union type that can hold one of several types at a time.
    The Union type uses a strong, medium, and weak matching rule to determine which type to initialize the union with,
    based on the type of the value passed to the constructor.

    The functionality is similar to that of Rust Enum.
    It follows the exact same memory layout to the Rust Enums-
    Essentially "struct { u8 tag; union { .... } u; };"
    Except while Rust uses a 16-bit integer as a tag, this can use a byte as the tag if there are less than 255 types
    See https://patshaughnessy.net/2018/3/15/how-rust-implements-tagged-unions
*/

#pragma once
#ifdef __inline_core_header__

namespace cm {

template<typename DelegateType, typename... Types>
class DelegateUnion : public Union<Types...> {
public:
    using Base = Union<Types...>;
    using Union<Types...>::Union;

    constexpr DelegateUnion& operator=(auto&& t)
    {
        Base::operator=(Forward<decltype(t)>(t));
        return *this;
    }

    template<typename T>
    requires (IsSameAsOneOf<T, Types...>)
    constexpr void store(DelegateType&& d)
    {
        using Init = decltype(Base::template TryInit<0, T, Types...>::next(Base::_data, declval<T const&>()));
        constexpr auto tag = Init::Tag;
        __builtin_printf("tag is %d\n", tag);
        _delegates[tag] = d;
    }

    constexpr Optional<DelegateType> const& load() const
    {
        __builtin_printf("tag access %d\n", this->tag());
        return _delegates[this->tag()];
    }

private:
    CVArray<Optional<DelegateType>, sizeof...(Types)> _delegates;
};

}  // namespace cm

#endif
