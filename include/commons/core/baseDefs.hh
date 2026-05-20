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

#include <commons/config.hh>
#include <stdarg.h>
#include <cstddef>
#include <new>
#include <commons/core/typeTraits.hh>


#ifdef _MSC_VER
#include <intrin.h>
#endif

// GCC/Clang define these on linux which is against the C standard (linux and unix should not be reserved keywords)
// instead, __linux__ and __unix__ is equivalent
#ifdef linux
#undef linux
#ifndef __linux__
#define __linux__ 1
#endif
#endif

#ifdef unix
#undef unix
#ifndef __unix__
#define __unix__ 1
#endif
#endif

#if !defined(__clang__) && defined(_MSC_VER)
using __UINT8_TYPE__ = unsigned __int8;
using __INT8_TYPE__ = signed char;  // because some dumb *** defined __int8 as char
using __UINT16_TYPE__ = unsigned __int16;
using __INT16_TYPE__ = __int16;
using __UINT32_TYPE__ = unsigned __int32;
using __INT32_TYPE__ = __int32;
using __UINT64_TYPE__ = unsigned __int64;
using __INT64_TYPE__ = __int64;
using __UINTMAX_TYPE__ = unsigned long long;
using __INTMAX_TYPE__ = long long;
#if _M_X64
using __SIZE_TYPE__ = unsigned __int64;
using __INTPTR_TYPE__ = __int64;
#else
using __SIZE_TYPE__ = unsigned __int32;
using __INTPTR_TYPE__ = __int32;
#endif
#endif

/// A "zero-bit" integer just for giggles. Good luck using it!
using u0 = void;
using u1 = unsigned _BitInt(1);

// Note: An "i1" would be useless because it would literally be nothing but a sign bit:
// It's either positive or negative nothing. Clang won't even let you use signed _BitInt(1).

/// {0, 1, 2, 3}
using u2 = unsigned _BitInt(2);
/// {0, 1, -2, -1}
using i2 = signed _BitInt(2);
using u3 = unsigned _BitInt(3);
using i3 = signed _BitInt(3);
using u4 = unsigned _BitInt(4);
using i4 = signed _BitInt(4);
using u8 = __UINT8_TYPE__;
using i8 = __INT8_TYPE__;
using u16 = __UINT16_TYPE__;
using i16 = __INT16_TYPE__;
using u24 = unsigned _BitInt(24);
using i24 = signed _BitInt(24);
using u32 = __UINT32_TYPE__;
using i32 = __INT32_TYPE__;
using u48 = unsigned _BitInt(48);
using i48 = signed _BitInt(48);
using u64 = __UINT64_TYPE__;
using i64 = __INT64_TYPE__;

#if defined(__aarch64__) || defined(__x86_64__)
using u128 = __uint128_t;
#else
using u128 = unsigned _BitInt(128);
#endif

#if defined(__aarch64__) || defined(__x86_64__)
using i128 = __int128;
#else
using i128 = _BitInt(128);
#endif

using u256 = unsigned _BitInt(256);
using i256 = _BitInt(256);
using usize = __SIZE_TYPE__;
using isize = __INTPTR_TYPE__;

static_assert(sizeof(u8) == 1 && sizeof(i8) == 1);
static_assert(sizeof(u16) == 2 && sizeof(i16) == 2);
static_assert(sizeof(u32) == 4 && sizeof(i32) == 4);
static_assert(sizeof(u64) == 8 && sizeof(i64) == 8);
static_assert(sizeof(u128) == 16 && sizeof(i128) == 16);
static_assert(sizeof(u256) == 32 && sizeof(i256) == 32);
static_assert(sizeof(usize) == sizeof(void*) && sizeof(isize) == sizeof(void*));

///
/// Most compilers have the __has_builtin function to check for the existence of a builtin function
///
#ifndef __has_builtin
#define __has_builtin() 0
#endif

///
/// Function annotation macros
///

#define $ExternC extern "C"
#define $NoReturn [[noreturn]]

#define NODISCARD

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

// clang-format off
#define $private(type, name) private: type _##name; public:

#define $private_with_const_value_getter(type, name) private: type _##name; public: constexpr T name() const { return _##name; }
#define $private_with_const_ref_getter(type, name) private: type _##name; public: constexpr T const& name() const { return _##name; }
#define $private_with_const_value_getter_override(type, name) private: type _##name; public: constexpr T name() const override { return _##name; }
#define $private_with_const_ref_getter_override(type, name) private: type _##name; public: constexpr T const& name() const override { return _##name; }

#define $getter_value(name) public: __attribute__((always_inline)) constexpr auto name() const { return _##name; }
#define $getter_value_override(name) public: __attribute__((always_inline)) constexpr auto name() const override { return _##name; }

// clang-format on

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
/// Definition of macro for validation sanity checks.
/// Any sanity check that fails will abort the process.
/// Currently it only validates pointers.
/// For a const pointer, it validates that it is readable by checking if a segfault occurs. if a segfault happens, it
/// will print it nicely instead of showing the horrid addresssanitizer crash dump.
/// For a non-const pointer, validates that it is readable and writable.
///
#define $validate(obj)                                                                                                 \
    do {                                                                                                               \
        if !consteval {                                                                                                \
            ::cm::validator::check<sizeof(#obj)>(obj, #obj);                                                           \
        }                                                                                                              \
    } while (0)

///
/// Same as validate, but allows checking a pointer of variable length.
///
#define $validate_sized(obj, count)                                                                                    \
    do {                                                                                                               \
        if !consteval {                                                                                                \
            ::cm::validator::checkBytes<sizeof(#obj)>(obj, count * sizeof(*obj), #obj);                                \
        }                                                                                                              \
    } while (0)

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

struct NoneType
{};

constexpr auto None = NoneType{};


/**
 @brief Defer is used to execute a statement upon exiting the current block, by return or throw.
 They are useful to ensure that resources are cleaned up when they are no longer needed.
 Inspired by the Zig language construct: https://zig.guide/language-basics/defer/
 Unlike Zig, this Defer construct can execute multiple statements at once.
 An example of usage is:
 \code{.cpp}
    int main() {
        DEFER { printf("second deferred\n"); };
        DEFER noexcept { printf("first deferred\n"); };

        printf("stuff\n");
        return 0;
    }
 \endcode
  When there are multiple defers in a single block, they are executed in reverse order.
  \attention Do not use return inside a defer statement. Since Defer is implemented with a lambda
 function, return is going to return from that lambda function instead of returning from the
 function using defer.
*/
#define DEFER DeferredOperation _unique_name_(__defer) = [&]()

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreserved-macro-identifier"
#define _concat_for_defer(a, b) _concat_for_defer_inner(a, b)
#define _concat_for_defer_inner(a, b) a##b
#define _unique_name_(base) _concat_for_defer(base, __LINE__)
#pragma GCC diagnostic pop


template<typename F>
class DeferredOperation {
    F f;

public:
    FORCEINLINE DeferredOperation(F f) noexcept
        : f(f)
    {}

    FORCEINLINE ~DeferredOperation() noexcept(noexcept(f())) { f(); }
};

template<typename F>
DeferredOperation(F) -> DeferredOperation<F>;

///
/// CPU related utilities
///
struct CPU
{
    ///
    /// Causes a trap in both compile-time and run-time evaluation.
    ///
    [[noreturn]]
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

    ///
    /// Returns true if the CPU is big-endian.
    ///
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


struct PairBase
{};

///
/// A basic pair structure.
///
template<typename A, typename B>
struct Pair : PairBase
{
    A first;
    B second;

    constexpr Pair() = default;
    constexpr Pair(A const& a, B const& b)
        : first(a),
          second(b)
    {}
};

template<typename A, typename B>
Pair(A const&, B const&) -> Pair<A, B>;


}  // namespace cm


[[gnu::alloc_size(1)]]
constexpr inline void* operator new(std::size_t, void* ptr, int) noexcept
{
    return ptr;
}

[[gnu::alloc_size(1)]]
constexpr inline void* operator new[](std::size_t, void* ptr, int) noexcept
{
    return ptr;
}

/**
 */
[[gnu::alloc_size(1)]]
void* operator new(std::size_t size);

[[gnu::alloc_size(1)]]
void* operator new[](std::size_t size);

[[gnu::alloc_size(1)]]  //
[[gnu::alloc_align(2)]]
void* operator new(std::size_t size, std::align_val_t al);

[[gnu::alloc_size(1)]]  //
[[gnu::alloc_align(2)]]
void* operator new[](std::size_t size, std::align_val_t al);

/// non-throwing allocation functions

[[gnu::alloc_size(1)]]
void* operator new(std::size_t size, std::nothrow_t const&) noexcept;

[[gnu::alloc_size(1)]]
void* operator new[](std::size_t size, std::nothrow_t const&) noexcept;

[[gnu::alloc_size(1)]]  //
[[gnu::alloc_align(2)]]
void* operator new(std::size_t size, std::align_val_t al, std::nothrow_t const&) noexcept;

[[gnu::alloc_size(1)]]  //
[[gnu::alloc_align(2)]]
void* operator new[](std::size_t size, std::align_val_t al, std::nothrow_t const&) noexcept;

void operator delete(void* ptr) noexcept;
void operator delete[](void* ptr) noexcept;
void operator delete(void* ptr, std::size_t sz) noexcept;
void operator delete[](void* ptr, std::size_t sz) noexcept;


// Random stuff

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


namespace cm {

template<typename T>
concept HasOutputStringMethod = requires (T value) {
    {
        T::outputString(value, [](char) {})
    };
};

void setIfNull(auto*& ptr, auto val)
{
    if (ptr == nullptr) {
        ptr = val;
    }
}

auto getIfNull(auto* ptr, auto val) { return ptr != nullptr ? ptr : val; }

namespace impl {
constexpr void outputStringForPrimitiveType(auto const&, auto const&);
}

template<typename T>
constexpr void OutputString(T const& value, auto const& out)
{
    if constexpr (HasOutputStringMethod<T>) {
        T::outputString(value, out);
    } else {
        impl::outputStringForPrimitiveType(value, out);
    }
}

///
/// Pre-initialization condition: Used for testing conditions to be true when the program starts running but before any
/// program logic is executed.
///
struct PreInitAssertion
{
    bool (*test)();
    char const* name;
    char const* file;
    int line;
};

///
/// Adds a new pre-initialization condition.
///
void addPreInitAssertion(PreInitAssertion const& assertion);

///
/// Adds a new pre-initialization condition.
/// @param cond The condition to test
///
#define runtime_assert(...)                                                                                            \
    __attribute__((constructor(LibraryConfig::GLOBAL_CTOR_ADD_PRECONDITION_PRIO))) void _unique_name_(_precheck_)()    \
    {                                                                                                                  \
        addPreInitAssertion(                                                                                           \
            PreInitAssertion{                                                                                          \
                .test = [] -> bool { return (__VA_ARGS__); },                                                          \
                .name = #__VA_ARGS__,                                                                                  \
                .file = __FILE__,                                                                                      \
                .line = __LINE__});                                                                                    \
    }

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

///
/// Typeless predicate for comparing.
/// Should return a negative integer if the first element is less than the second;
/// A positive integer if the first element is greater than the second;
/// Zero if the elements are equal.
///
using Comparator = int (*)(void const*, void const*);

///
/// Comparable interface. Automatically implements all of the comparison operators for a derived class, provided one
/// function is implemented in Derived. int compare(Derived)- which returns 0 if equal, 1 if greater, -1 if less.
///
template<typename Derived>
struct IComparable
{

    constexpr static Comparator comparator = [](void const* e1, void const* e2) -> int {
        auto t1 = reinterpret_cast<Derived const*>(e1);
        auto t2 = reinterpret_cast<Derived const*>(e2);
        return t1->compare(*t2);
    };

    constexpr inline bool operator<(auto const& x) const { return static_cast<Derived const*>(this)->compare(x) < 0; }

    constexpr inline bool operator>(auto const& x) const { return static_cast<Derived const*>(this)->compare(x) > 0; }

    constexpr inline bool operator<=(auto const& x) const { return static_cast<Derived const*>(this)->compare(x) <= 0; }

    constexpr inline bool operator>=(auto const& x) const { return static_cast<Derived const*>(this)->compare(x) >= 0; }

    constexpr inline bool lessThanAny(auto... args) const { return ((this->operator<(args)) || ...); }

    constexpr inline bool lessThanAll(auto... args) const { return ((this->operator<(args)) && ...); }

    constexpr inline bool greaterThanAny(auto... args) const { return ((this->operator>(args)) || ...); }

    constexpr inline bool greaterThanAll(auto... args) const { return ((this->operator>(args)) && ...); }

    constexpr inline bool lessEqualThanAny(auto... args) const { return ((this->operator<=(args)) || ...); }

    constexpr inline bool lessEqualThanAll(auto... args) const { return ((this->operator<=(args)) && ...); }

    constexpr inline bool greaterEqualThanAny(auto... args) const { return ((this->operator>=(args)) || ...); }

    constexpr inline bool greaterEqualThanAll(auto... args) const { return ((this->operator>=(args)) && ...); }
};

///
/// Defines whether a type has every comparison operator.
///
template<typename A, typename B = A>
concept IsFullyComparable = requires (A a, B b) {
    { a == b } -> IsSame<bool>;
    { a != b } -> IsSame<bool>;
    { a < b } -> IsSame<bool>;
    { a <= b } -> IsSame<bool>;
    { a > b } -> IsSame<bool>;
    { a >= b } -> IsSame<bool>;
};

///
/// Defines whether a type has the .compare member function.
///
template<typename A>
concept HasCompareMemberFunction = requires (A a1, A a2) {
    { a1.compare(a2) } -> IsSame<int>;
};

///
/// Generic comparison function.
/// Returns 0 if two given objects are equal, -1 if the first object is less than the second object, and 1 otherwise.
/// @param a The first object
/// @param b The second object
///
constexpr int Compare(auto const& a, auto const& b)
{
    if constexpr (HasCompareMemberFunction<decltype(a)>) {
        // If these are objects, this might be more efficient than comparing them using the operators
        return a.compare(b);
    } else {
        if (a == b) {
            return 0;
        } else if (a < b) {
            return -1;
        } else {
            return 1;
        }
    }
}

///
/// Compare() when given pointers would compare the memory addresses themselves.
/// ComparePointed instead dereferences two pointer arguments then compares them. otherwise it acts like Compare.
/// @param a The first object
/// @param b The second object
///
constexpr int ComparePointed(auto const& a, auto const& b)
{
    if constexpr (IsPointer<decltype(a)> || IsPointer<decltype(b)>) {
        return Compare(*a, *b);
    } else {
        return Compare(a, b);
    }
}


template<typename Container_>
struct ComputedProperty
{
protected:
    friend Container_;

    ComputedProperty() = default;

    template<typename T1>
    inline auto findOffset(T1 Container_::* member) const noexcept
    {
        signed char data[sizeof(Container_)];
        Container_ const& object = *reinterpret_cast<Container_*>(data);
        return reinterpret_cast<isize>(&(object.*member)) - reinterpret_cast<isize>(&object);
    }

    template<typename T1>
    inline Container_* container(T1 Container_::* member) noexcept
    {
        return reinterpret_cast<Container_*>(reinterpret_cast<isize>(this) - findOffset(member));
    }

    template<typename T1>
    inline Container_ const* container(T1 Container_::* member) const noexcept
    {
        return reinterpret_cast<Container_ const*>(reinterpret_cast<isize>(this) - findOffset(member));
    }
};

}  // namespace cm
