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

// IWYU pragma: private

#pragma once


#ifdef _MSC_VER
#include <intrin.h>
#endif


#ifndef __has_builtin
#define __has_builtin() 0
#endif

///
/// Function annotation macros
///

#define $ExternC extern "C"
#define $NoReturn [[noreturn]]

#define NODISCARD [[nodiscard]]

#define NOINSTRUMENT [[gnu::no_instrument_function, gnu::no_sanitize_address]]

#ifdef _MSC_VER
#define $DLLExport [[gnu::dllexport]]
#else
#define $DLLExport [[gnu::dllexport]]
#endif

#if __clang__
#define _noinline __attribute__((noinline))
#elif __GNUC__
#define _noinline __attribute__((noinline))
#else
#define _noinline maybe_unused
#endif

#if defined(__GNUC__) || defined(__clang__)
#define FORCEINLINE [[gnu::always_inline]] inline
#elif _MSC_VER
#define FORCEINLINE __FORCEINLINE
#else
#define FORCEINLINE inline
#endif

#ifdef _MSC_VER
#define _no_unique_address msvc::no_unique_address
#endif

///
/// Definition of macro for 'unsafe' code blocks
///

#if defined(__GNUC__) || defined(__clang__)
#ifdef _WIN32
#define UNSAFE(...)                                                                                                    \
    _Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wunsafe-buffer-usage\"")                         \
        _Pragma("GCC diagnostic ignored \"-Wunsafe-buffer-usage-in-libc-call\"")                                       \
            __VA_ARGS__ _Pragma("GCC diagnostic pop")
#else
#define UNSAFE(...)                                                                                                    \
    _Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wunsafe-buffer-usage\"")                         \
        __VA_ARGS__ _Pragma("GCC diagnostic pop")
#endif

#define UNSAFE_BEGIN _Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wunsafe-buffer-usage\"")
#define UNSAFE_END _Pragma("GCC diagnostic pop")
#else
#define UNSAFE(...) __VA_ARGS__
#endif

///
/// Misc macros
///

#if !__has_builtin(__builtin_expect)
#define __builtin_expect(...) __VA_ARGS__
#endif
#ifdef _MSC_VER
#define CONSTANT_EVALUATED (__builtin_is_constant_evaluated())
#else
#define CONSTANT_EVALUATED consteval
#endif
#define MACRO_CAT_IMPL(a, b) a##b
#define MACRO_CAT(a, b) MACRO_CAT_IMPL(a, b)
#define UNIQUE_VAR_NAME(prefix) MACRO_CAT(prefix, __COUNTER__)

///
/// Additional "builtin" functions that don't have a place anywhere..
///

namespace cm {

struct CPU
{
    constexpr static void trap()
    {
        if CONSTANT_EVALUATED {
            while (true)
                ;
        } else {
#if __has_builtin(__builtin_trap)
            __builtin_trap();
#elif _MSC_VER
            __debugbreak();
#else
#error "Not implemented"
#endif
        }
    }

    consteval bool isBigEndian()
    {
        auto c = 0x01020304;
        struct dummy
        {
            unsigned char b[4];
        };
        return __builtin_bit_cast(dummy, c).b[0] == 1;
    }

} inline constexpr CPU;


template<typename A, typename B>
struct Pair
{
    A first;
    B second;
    Pair(A const& first, B const& second)
        : first(first), second(second)
    {}
};

template<typename A, typename B>
Pair(A const&, B const&) -> Pair<A, B>;


template<typename T>
struct Cast
{

    ///
    /// A function that takes in a variable number of arguments, and returns the Nth one cast to T.
    /// example:
    ///
    /// constexpr auto x = Cast<int>::FromNth<1>;
    /// constexpr auto i = x(1.0f, 2.0f);
    /// static_assert(i == 2);
    ///
    template<long N>
    struct FromNthT
    {
        constexpr T operator()(auto const&... args) const
            requires ((N - 1) <= long(sizeof...(args)) && (__is_convertible(decltype(args), T) && ...))
        {
            long i = 0;
            T result = {};
            (
                [&]() {
                    if (i >= N) {
                        result = T(args);
                    }
                    i++;
                }(),
                ...);
            return result;
        }
    };
    template<unsigned N>
    constexpr static FromNthT<N> FromNth = {};
};


}  // namespace cm

namespace std {
using size_t = __SIZE_TYPE__;
struct nothrow_t;
enum class align_val_t : std::size_t {
};
}

constexpr void* operator new(std::size_t, void* ptr) noexcept { return ptr; }
constexpr void* operator new[](std::size_t, void* ptr) noexcept { return ptr; }

/**
 */
void* operator new(std::size_t size);

void* operator new[](std::size_t size);

void* operator new(std::size_t size, std::align_val_t al);

void* operator new[](std::size_t size, std::align_val_t al);

/// non-throwing allocation functions

void* operator new(std::size_t size, std::nothrow_t const&) noexcept;

void* operator new[](std::size_t size, std::nothrow_t const&) noexcept;

void* operator new(std::size_t size, std::align_val_t al, std::nothrow_t const&) noexcept;

void* operator new[](std::size_t size, std::align_val_t al, std::nothrow_t const&) noexcept;

void operator delete(void* ptr) noexcept;

void operator delete[](void* ptr) noexcept;

void operator delete(void* ptr, std::size_t sz) noexcept;

void operator delete[](void* ptr, std::size_t sz) noexcept;


template<typename T, typename... Args>
constexpr inline T* ConstructInPlace(void* ptr, Args&&... args)
{
    return new (ptr) T(Forward<Args>(args)...);
}

template<unsigned long long Size>
struct BytePadding
{
    __UINT8_TYPE__ pad[Size];
};


/*
 Equatable interface.
 Derived must implement: bool equals(...)
*/
template<typename Derived>
struct IEquatable
{
    constexpr inline bool equalsAny(auto const&... args) const
    {
        return ((static_cast<Derived const*>(this)->equals(args)) || ...);
    }
    constexpr inline bool equalsAll(auto const&... args) const
    {
        return ((static_cast<Derived const*>(this)->equals(args)) && ...);
    }
    constexpr inline bool operator==(auto const& x) const { return static_cast<Derived const*>(this)->equals(x); }
    constexpr inline bool operator!=(auto const& x) const { return !static_cast<Derived const*>(this)->equals(x); }
};
