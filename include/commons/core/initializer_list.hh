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


namespace std {

///
/// My implementation of std::initializer_list
///
template<class E>
class initializer_list {
public:
    typedef E value_type;
    typedef E const& reference;
    typedef E const& const_reference;
    typedef __SIZE_TYPE__ usizeype;
    typedef E const* iterator;
    typedef E const* const_iterator;

private:
    E const* _array;
    __SIZE_TYPE__ _len;

    // The compiler can call a private constructor.
    constexpr initializer_list(E const* a, __SIZE_TYPE__ l)
        : _array(a), _len(l)
    {}

public:
    constexpr initializer_list() noexcept
        : _array(nullptr), _len(0)
    {}

    // Number of elements.
    constexpr auto size() const noexcept { return _len; }

    // First element.
    constexpr E const* begin() const noexcept { return _array; }

    // One past the last element.
    constexpr E const* end() const noexcept { return UNSAFE(begin() + size()); }
};

///
/// Return an iterator pointing to the first element of the initializer_list.
/// @param a Initializer list.
///
template<class T>
constexpr T const* begin(initializer_list<T> a) noexcept
{
    return a.begin();
}

///
/// Return an iterator pointing to one past the last element of the initializer_list.
/// @param a Initializer list.
///
template<class T>
constexpr T const* end(initializer_list<T> a) noexcept
{
    return a.end();
}

}  // namespace std


namespace cm {


///
/// Just a workaround for some stuff.
/// Using C++ references plainly prevents some things,
/// like changing what the reference points to without calling the copy constructor.
///
/// I want my collections to be able to store references, unlike std. Why?
/// Why not? There are many times you might want to store a reference to something. Maybe I don't need to use a
/// "std::shared_ptr" because the references are to static data. There's a number of reasons why I shouldn't have to
/// result to a pointer in that case.
///
///  SO BASICALLY ...
///
///  In my library, when you have a collection of references (Queue<T&>, for example), you get something in-between a
///  collection of values and a collection of pointers.
///
///  When you INSERT a value into the collection (through initialization, or methods like push/insert/etc)-- it
///  stores a "pointer" to that value in the collection.
///
///  When you REMOVE a value from the collection, it removes the pointer to that value. It does not destroy the value
///  itself.
///
template<typename T>
struct RefWrapper
{
    // clang-format off
    FORCEINLINE constexpr RefWrapper(T& x) : ptr(&x) {}
    FORCEINLINE constexpr operator T&() { return *ptr; }
    FORCEINLINE constexpr operator T const&() const { return *ptr; }
    FORCEINLINE constexpr T* operator->() { return ptr; }
    FORCEINLINE constexpr T const* operator->() const { return ptr; }
    FORCEINLINE constexpr RefWrapper& operator=(T& ref) { ptr = &ref; return *this; }



    FORCEINLINE constexpr T operator+(auto& val) const requires (IsAddable<T, decltype(val)>) { return *ptr + val; }
    FORCEINLINE constexpr T operator-(auto& val) const requires (IsSubtractable<T, decltype(val)>) { return *ptr - val; }
    FORCEINLINE constexpr T operator*(auto& val) const requires (IsMultipliable<T, decltype(val)>) { return *ptr * val; }
    FORCEINLINE constexpr T operator/(auto& val) const requires (IsDivideable<T, decltype(val)>) { return *ptr / val; }
    FORCEINLINE constexpr T operator%(auto& val) const requires (IsMODAble<T, decltype(val)>) { return *ptr % val; }
    FORCEINLINE constexpr T operator&(auto& val) const requires (IsANDAble<T, decltype(val)>) { return *ptr & val; }
    FORCEINLINE constexpr T operator|(auto& val) const requires (IsORAble<T, decltype(val)>) { return *ptr | val; }
    FORCEINLINE constexpr T operator^(auto& val) const requires (IsXORAble<T, decltype(val)>) { return *ptr ^ val; }

    FORCEINLINE constexpr auto& operator++() requires (!IsConst<T> && IsPrefixIncrementable<T>) { ++(*ptr); return *this; }
    FORCEINLINE constexpr T operator++(int) requires (!IsConst<T> && IsPostfixIncrementable<T>) { T tmp = *ptr; ++(*ptr); return move(tmp); }
    FORCEINLINE constexpr auto& operator--() requires (!IsConst<T> && IsPrefixDecrementable<T>) { --(*ptr); return *this; }
    FORCEINLINE constexpr T operator--(int) requires (!IsConst<T> && IsPostfixDecrementable<T>) { T tmp = *ptr; --(*ptr); return move(tmp); }
   
    FORCEINLINE constexpr auto& operator+=(auto& val) requires (!IsConst<T> && IsAddable<T, decltype(val)>) { *ptr += val; return *this; }
    FORCEINLINE constexpr auto& operator-=(auto& val) requires (!IsConst<T> && IsSubtractable<T, decltype(val)>) { *ptr - val; return *this; }
    FORCEINLINE constexpr auto& operator*=(auto& val) requires (!IsConst<T> && IsMultipliable<T, decltype(val)>) { *ptr * val; return *this; }
    FORCEINLINE constexpr auto& operator/=(auto& val) requires (!IsConst<T> && IsDivideable<T, decltype(val)>) { *ptr /= val; return *this; }
    FORCEINLINE constexpr auto& operator%=(auto& val) requires (!IsConst<T> && IsMODAble<T, decltype(val)>) { *ptr %= val; return *this; }
    FORCEINLINE constexpr auto& operator&=(auto& val) requires (!IsConst<T> && IsANDAble<T, decltype(val)>) { *ptr &= val; return *this; }
    FORCEINLINE constexpr auto& operator|=(auto& val) requires (!IsConst<T> && IsORAble<T, decltype(val)>) { *ptr |= val; return *this; }
    FORCEINLINE constexpr auto& operator^=(auto& val) requires (!IsConst<T> && IsXORAble<T, decltype(val)>) { *ptr ^= val; return *this; }


    FORCEINLINE constexpr static void outputString(RefWrapper const& self, auto const& out) {
        OutputString(*self.ptr, out);
    }
private:
    T* ptr;
};


// clang-format off
template<typename T>
struct TWrapIfReference { using Type = T; };
template<typename T>
struct TWrapIfReference<T&> { using Type = RefWrapper<T>; };
template<typename T>
struct TWrapIfReference<T volatile&> { using Type = RefWrapper<T volatile>; };
template<typename T>
struct TWrapIfReference<T const&> { using Type = RefWrapper<T const>; };
template<typename T>
struct TWrapIfReference<T const volatile&> { using Type = RefWrapper<T const volatile>; };

// clang-format on


template<typename T>
using WrapIfReference = SelectType<IsReference<T>, T, typename TWrapIfReference<T>::Type>;

///
/// Use Array instead.
/// Just a dummy struct for code that cannot depend on the real Array
///
template<typename T, unsigned N>
struct CVArray
{
    T _data[N];

    constexpr CVArray() = default;

    constexpr CVArray(auto... values) requires (sizeof...(values) <= N)
        : _data{static_cast<T>(values)...}
    {}

    constexpr CVArray(T const (&values)[N])
        : _data(values)
    {}

    FORCEINLINE constexpr T& operator[](auto index)
    {
        UNSAFE({ return _data[index]; });
    }

    FORCEINLINE constexpr T const& operator[](auto index) const
    {
        UNSAFE({ return _data[index]; });
    }
};

template<typename T, unsigned N>
CVArray(T const (&)[N]) -> CVArray<T, N>;


namespace CArrays {

/**
 * @brief Moves data (which may be overlapping) to a new memory location, mostly used in ResizableArray

 * If the data "safe to move" (i.e. is trivially copyable or inherits from SafeBinaryMoveable), it is copied via
 * memmove. Otherwise, it is copied by using copy constructors.

 * "Safe to move" Means that for a given instance of type T, changing the instance's location in memory would not mess
 * up its internal state.

 * Node structures or structures using self-reference are generally not safe to move. Consider the
 * struct Foo { Foo() : ptr(&x) {}; int x; int *ptr; }
 * Moving an instance of Foo in memory would invalidate ptr.

 * @param dst Destination pointer to a series of uninitialized T.
 * @param src Source pointer to a series of instances of T.
 * @param n The number of elements to copy
 */
UNSAFE_BEGIN
template<typename T>
constexpr void moveDataToNewRegion(T* dst, T const* src, usize n)
{
    if consteval {
        for (__SIZE_TYPE__ i = 0; i < n; i++) {
            new (dst + i) T(src[i]);
        }
    } else {
        if constexpr (TriviallyCopyConstructible<T> || IsDerivedFrom<SafeBinaryMoveable, T>) {
            __builtin_memmove(dst, src, sizeof(T) * n);
        } else {
            for (__SIZE_TYPE__ i = 0; i < n; i++) {
                new (dst + i) T(src[i]);
            }
        }
    }
}

template<typename T>
constexpr bool equal(T const* a, T const* b, usize n)
{
    if consteval {
        for (usize i = 0; i < n; i++) {
            if (a[i] != b[i]) {
                return false;
            }
        }
    } else {
        if constexpr (IsPrimitiveData<T>) {
            return memcmp(a, b, n * sizeof(T)) == 0;
        } else {
            for (usize i = 0; i < n; i++) {
                if (a[i] != b[i]) {
                    return false;
                }
            }
        }
    }
    return true;
}

template<typename T>
constexpr void DefaultInitialize(T const* a, usize n)
{
    if consteval {
        for (usize i = 0; i < n; i++) {
            a[i] = T{};
        }
    } else {
        if constexpr (IsPrimitiveData<T>) {
            memset(a, 0, sizeof(T) * n);
        } else {
            for (auto i = 0uz; i < n; i++) {
                a[i] = T{};
            }
        }
    }
}

template<typename T>
constexpr auto stringLen(T const* str)
{
    if consteval {
        auto len = 0uz;
        while (*str++ != T{}) {
            len++;
        }
        return len;
    } else {
        if constexpr (IsUnderlyingTypeOneOf<T, char*, char8_t*>) {
            return strlen(str);
        } else {
            auto len = 0uz;
            while (*str++ != T{}) {
                len++;
            }
            return len;
        }
    }
}

constexpr auto For(Range<usize> const& range, auto* ptr, auto func)
{
    for (auto i : range) {
        func(ptr[i]);
    }
}

}  // namespace CArrays

UNSAFE_END

}  // namespace cm
#endif
