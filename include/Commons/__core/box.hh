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

///
/// Returns the wrapper class for a given primitive value 'x'. If 'x' is not a primitive value, it just returns 'x'.
/// @param x The value
///
constexpr auto const& Box(auto const& x)
{
    using T = CVRefRemoved<decltype(x)>;
    if constexpr (IsSame<T, bool>) {
        return Bool;
    } else if constexpr (IsSame<T, i8>) {
        return I8;
    } else if constexpr (IsSame<T, u8>) {
        return U8;
    } else if constexpr (IsSame<T, i16>) {
        return I16;
    } else if constexpr (IsSame<T, u16>) {
        return U16;
    } else if constexpr (IsSame<T, i32>) {
        return I32;
    } else if constexpr (IsSame<T, u32>) {
        return U32;
    } else if constexpr (IsSame<T, i64>) {
        return I64;
    } else if constexpr (IsSame<T, u64>) {
        return U64;
    } else if constexpr (IsSame<T, i128>) {
        return I128;
    } else if constexpr (IsSame<T, u128>) {
        return U128;
    } else if constexpr (IsSame<T, i256>) {
        return I256;
    } else if constexpr (IsSame<T, u256>) {
        return U256;
    } else if constexpr (IsFloatingPoint<T>) {
        return Double;
    } else if constexpr (IsClass<T>) {
        return x;
    }
}

///
/// Represents a reference to anything, like a void*
///
struct AnyRef
{
    virtual ~AnyRef() = default;
    virtual void outputString(Function<void(char)> const& receiver) const = 0;
};

///
/// Subclass of AnyRef for a specific type
///
template<typename T>
struct AnyRefT final : AnyRef
{
    AnyRefT(T const& ref)
        : _ptr(&const_cast<T&>(ref))
    {}

    void outputString(Function<void(char)> const& receiver) const override
    {
        auto const& v = *_ptr;
        if constexpr (IsChar<T>) {
            receiver(char(v));
        } else {
            Box(v).outputString(v, receiver);
        }
    }

private:
    T* _ptr;
};

template<typename T>
AnyRefT(T const&) -> AnyRefT<T>;


template<typename T>
constexpr void ArrayRef<T>::outputString(ArrayRef<T> const& a, auto const& out)
{
    out('[');
    if (a.length() != 0) {
        usize i = 0;
        for (; i < a.length() - 1; i++) {
            auto const& value = a[i];
            Box(value).outputString(value, out);
            out(',');
            out(' ');
        }
        for (; i < a.length(); i++) {
            auto const& value = a[i];
            Box(value).outputString(value, out);
        }
    }
    out(']');
}

}  // namespace cm
#endif
