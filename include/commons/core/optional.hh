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
#ifdef __inline_core_header__


namespace cm {

struct NoneType
{};

constexpr auto None = NoneType{};

}


constexpr void* operator new(__SIZE_TYPE__, void* ptr, cm::NoneType) noexcept { return ptr; }

namespace cm {

///
/// A wrapper class around a type T that does the following:
/// If T is a reference type "U&", stores it as a pointer "U*" instead.
/// Otherwise, it stores it as a value.
///
template<typename T>
struct ValueOrWrappedRef
{
    constexpr ValueOrWrappedRef(T const& value)
        : _value(value)
    {}
    constexpr T& get() const { return const_cast<T&>(_value); }

private:
    T _value;
};

template<typename T>
struct ValueOrWrappedRef<T&>
{
    constexpr ValueOrWrappedRef(T& value)
        : _value(&value)
    {}

    constexpr T& get() const { return *_value; }

private:
    T* _value;
};

template<typename T>
struct ValueOrWrappedRef<T volatile&>
{
    constexpr ValueOrWrappedRef(T volatile& value)
        : _value(&value)
    {}

    constexpr T volatile& get() { return *_value; }

private:
    T volatile* _value;
};

template<typename T>
struct ValueOrWrappedRef<T const&>
{
    constexpr ValueOrWrappedRef(T const& value)
        : _value(&value)
    {}

    constexpr T const& get() { return *_value; }

private:
    T const* _value;
};


template<typename T>
struct ValueOrWrappedRef<T const volatile&>
{
    constexpr ValueOrWrappedRef(T const volatile& value)
        : _value(&value)
    {}

    constexpr T const volatile& get() { return *_value; }

private:
    T const volatile* _value;
};


///
/// The class template Optional manages an optional contained value, i.e. a value that may or may
/// not be present. A common use case for optional is the return value of a function that may fail.
/// Identical in function to https://en.cppreference.com/w/cpp/utility/optional
///
template<typename T>
requires (!__is_same(T, NoneType))
struct Optional
{
    ///
    /// Constructs an Option that does not contain a value.
    ///
    constexpr inline Optional() noexcept
        : _bit(false), _u{._dummy = {}}
    {}

    ///
    /// A more explicit way of constructing an Option that does not contain a value.
    ///
    constexpr inline Optional(NoneType const&) noexcept
        : Optional()
    {}

    ///
    /// Constructs an Option that does contain a value.
    ///
    constexpr inline Optional(T const& value)
        : _bit(true), _u{._value{value}}
    {}

    ///
    /// Copy constructor.
    ///
    constexpr inline Optional(Optional const& other)
        : _bit(other._bit), _u([&]() {
              if (other._bit)
                  return _storedValue{._value = other._u._value};
              else
                  return _storedValue{._dummy = {}};
          })
    {}

    ///
    /// Destructor.
    ///
    constexpr inline ~Optional()
    {
        if constexpr (!TriviallyDestructible<T>) {
            if (this->hasValue())
                this->_u._value.~T();
        }
    }

    ///
    /// Returns true if the Option contains a value.
    ///
    constexpr inline operator bool() const noexcept { return _bit; }

    ///
    /// Returns true if the Option contains a value.
    ///
    constexpr inline bool hasValue() const noexcept { return this->operator bool(); }

    ///
    /// Yet another way to check if the option contains a value.
    ///
    constexpr inline bool operator==(NoneType const&) const noexcept { return !hasValue(); }

    ///
    /// Attempts to access the value. A trap occurs if there is no value.
    ///
    constexpr inline RefRemoved<T>* operator->() const noexcept
    {
        if (!this->hasValue()) {
            __builtin_trap();
        }
        return const_cast<RefRemoved<T>*>(&this->_u._value.get());
    }

    ///
    /// Attempts to access the value. A trap occurs if there is no value.
    ///
    constexpr inline T& value() const& noexcept
    {
        if (!this->hasValue()) {
            __builtin_trap();
        }
        return this->_u._value.get();
    }

    ///
    /// Returns the value if it is contained, otherwise a default value.
    ///
    template<class U>
    constexpr inline T valueOr(U&& x) const&
    {
        return this->_bit ? _u._value.get() : static_cast<T>(Forward<U>(x));
    }

    ///
    ///
    ///
    constexpr static void outputString(Optional const& self, auto const& out);

private:
    bool _bit;
    union _storedValue {
        // C++ doesn't allow directly storing references inside unions
        ValueOrWrappedRef<T> _value;
        unsigned char _dummy[sizeof(T)];
    } _u;
};

}  // namespace cm
#endif
