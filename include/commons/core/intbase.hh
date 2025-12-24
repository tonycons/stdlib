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


/**
 * @brief Allows for interpreting the bits of one value as the bits of another type.
 * @tparam Dst The type to interpret the bits of 'src' as
 * @param src The value to interpret as bits of 'ConvertTo'
 * @return Value of type 'Dst' with exactly the same bits as value 'src'
 */
template<typename Dst>
constexpr inline auto bit_cast(auto src) noexcept
{
    static_assert(sizeof(Dst) == sizeof(decltype(src)), "bit_cast must be performed on types of equal size");
    return __builtin_bit_cast(Dst, src);
}


namespace cm {
namespace impl {
template<unsigned long long N>
consteval auto GetUintRanged()
{
    if constexpr (N > 281474976710656ull) {
        return u64{};
    } else if constexpr (N > 4294967296ull) {
        return u48{};
    } else if constexpr (N > 16777216ull) {
        return u32{};
    } else if constexpr (N > 65536ull) {
        return u24{};
    } else if constexpr (N > 256ull) {
        return u16{};
    } else if constexpr (N > 16ull) {
        return u8{};
    } else if constexpr (N > 8ull) {
        return u4{};
    } else if constexpr (N > 4ull) {
        return u3{};
    } else if constexpr (N > 2ull) {
        return u2{};
    } else {
        return u1{};
    }
}

template<unsigned long long N>
consteval auto GetIntRanged()
{
    if constexpr (N > 281474976710656ull) {
        return i64{};
    } else if constexpr (N > 4294967296ull) {
        return i48{};
    } else if constexpr (N > 16777216ull) {
        return i32{};
    } else if constexpr (N > 65536ull) {
        return i24{};
    } else if constexpr (N > 256ull) {
        return i16{};
    } else if constexpr (N > 16ull) {
        return i8{};
    } else if constexpr (N > 8ull) {
        return i4{};
    } else if constexpr (N > 4ull) {
        return i3{};
    } else {
        return i2{};
    }
}

template<unsigned Bits>
consteval auto GetUintN()
{
    if constexpr (Bits == 256) {
        return u256{};
    } else if constexpr (Bits == 128) {
        return u128{};
    } else if constexpr (Bits == 64) {
        return u64{};
    } else if constexpr (Bits == 32) {
        return u32{};
    } else if constexpr (Bits == 16) {
        return u16{};
    } else if constexpr (Bits == 8) {
        return u8{};
    } else {
        return static_cast<unsigned _BitInt(Bits)>(0);
    }
}

template<unsigned Bits>
consteval auto GetIntN()
{
    if constexpr (Bits == 256) {
        return i256{};
    } else if constexpr (Bits == 128) {
        return i128{};
    } else if constexpr (Bits == 64) {
        return i64{};
    } else if constexpr (Bits == 32) {
        return i32{};
    } else if constexpr (Bits == 16) {
        return i16{};
    } else if constexpr (Bits == 8) {
        return i8{};
    } else {
        return static_cast<_BitInt(Bits)>(0);
    }
}

}  // namespace impl


///
/// Type alias for an unsigned integer type which has enough bits to store N distinct states.
///
template<unsigned long long N>
using UintRanged = decltype(impl::GetUintRanged<N>());

///
/// Type alias for a signed integer type which has enough bits to store N distinct states.
///
template<unsigned long long N>
using IntRanged = decltype(impl::GetIntRanged<N>());

///
/// Type alias for an unsigned integer that has a certain amount of bits.
/// Useful if you want the number of bits to be determined via template metaprogramming.
///
template<unsigned Bits>
using UintN = decltype(impl::GetUintN<Bits>());

///
/// Type alias for a signed integer that has a certain amount of bits.
/// Useful if you want the number of bits to be determined via template metaprogramming.
///
template<unsigned Bits>
using IntN = decltype(impl::GetIntN<Bits>());

///
/// Defines an unsigned integer constant representing kilobytes.
///
constexpr auto operator""_KB(unsigned long long _) { return 1024ull * _; }

///
/// Defines an unsigned integer constant representing megabytes.
////
constexpr auto operator""_MB(unsigned long long _) { return 1024ULL * 1024ULL * _; }

using uint = unsigned int;

///
/// Kinda pointless?..
///
template<typename T>
constexpr inline usize SizeOf = usize(sizeof(T));


#if __clang__
template<typename T, unsigned int Size>
using Vector = __attribute__((__vector_size__(Size * sizeof(T)))) T;
#elif __GNUC__
template<typename T, unsigned int Size>
using Vector = __attribute__((__vector_size__(Size * sizeof(T)))) T;
#else
#error "Does this compiler support vector primitive types?"
#endif

template<unsigned Size>
using VectorU8x = Vector<u8, Size>;
template<unsigned Size>
using VectorU32x = Vector<u32, Size>;

static_assert(sizeof(Vector<int, 4>) == 16, "");


///
/// Returns the largest value in a variable-number of arguments
/// All arguments are coerced to be the same type as the first argument.
///
// Base case 1: single argument (simply return a reference to it)
constexpr decltype(auto) max(auto const& a) { return a; }

// Base case 2: two arguments
constexpr decltype(auto) max(auto const& a, auto const& b) { return a > b ? a : b; }

// Recursive case: three or more arguments
constexpr decltype(auto) max(auto const& a, auto const& b, auto const&... args) { return max(a, max(b, args...)); }

///
/// Returns a reference to the smallest value in a variable-number of arguments
/// All arguments are coerced to be the same type as the first argument.
///

// Base case 1: single argument (simply return a reference to it)
constexpr decltype(auto) min(auto const& a) { return a; }

// Base case 2: two arguments
constexpr decltype(auto) min(auto const& a, auto const& b) { return a < b ? a : b; }

// Recursive case: three or more arguments
constexpr decltype(auto) min(auto const& a, auto const& b, auto const&... args)
{
    // The inner min(b, args...) call now correctly forwards its reference type.
    // The outer call then returns a reference to either 'a' or the result
    // of the inner call.
    return min(a, min(b, args...));
}


static_assert(max(-1, 2, 3, -11, 5) == 5);
// static_assert(max(0, 1e23, 9999999999LL) == 1e23);
static_assert(min(-1, 2, 3, -11, 5) == -11);
static_assert(min(0, -999999999999LL) == -999999999999LL);


#ifdef _MSC_VER
#include <intrin.h>
#endif


using int8_t = __INT8_TYPE__;
using uint8_t = __UINT8_TYPE__;
using int16_t = __INT16_TYPE__;
using uint16_t = __UINT16_TYPE__;
using int32_t = __INT32_TYPE__;
using uint32_t = __UINT32_TYPE__;
using int64_t = __INT64_TYPE__;
using uint64_t = __UINT64_TYPE__;
using size_t = __SIZE_TYPE__;
using ssize_t = __INTPTR_TYPE__;


/*
 * Implementation of builtins checking for overflow in subtraction.
 */

#if !__has_builtin(__builtin_add_overflow) && _MSC_VER

__FORCEINLINE constexpr bool __builtin_add_overflow(uint8_t v1, uint8_t v2, uint8_t* pRes)
{
    if CONSTANT_EVALUATED {
        uint16_t s = uint16_t(v1) + uint16_t(v2);
        *pRes = uint8_t(s);
        return s > 255;
    } else {
        return _addcarry_u8(0, v1, v2, pRes);
    }
}

__FORCEINLINE constexpr bool __builtin_add_overflow(uint16_t v1, uint16_t v2, uint16_t* pRes)
{
    if CONSTANT_EVALUATED {
        uint32_t s = uint32_t(v1) + uint32_t(v2);
        *pRes = uint16_t(s);
        return s > 65535;
    } else {
        return _addcarry_u16(0, v1, v2, pRes);
    }
}

__FORCEINLINE constexpr bool __builtin_add_overflow(uint32_t v1, uint32_t v2, uint32_t* pRes)
{
    if CONSTANT_EVALUATED {
        uint64_t s = uint64_t(v1) + uint64_t(v2);
        *pRes = uint32_t(s);
        return s > 4294967295u;
    } else {
        return _addcarry_u32(0, v1, v2, pRes);
    }
}

__FORCEINLINE constexpr bool __builtin_add_overflow(uint64_t v1, uint64_t v2, uint64_t* pRes)
{
    if CONSTANT_EVALUATED {
        uint64_t Sum = (0 != 0) + v1 + v2;
        uint64_t CarryVector = (v1 & v2) ^ ((v1 ^ v2) & ~Sum);
        *pRes = Sum;
        return CarryVector >> 63;
    } else {
        return _addcarry_u64(0, v1, v2, pRes);
    }
}

__FORCEINLINE constexpr bool __builtin_add_overflow(int8_t v1, int8_t v2, int8_t* pRes)
{
    if CONSTANT_EVALUATED {
        int16_t c = int16_t(v1) + int16_t(v2);
        *pRes = int8_t(c);
        return c < -128 || c > 127;
    } else {
        return _add_overflow_i8(0, v1, v2, pRes);
    }
}

__FORCEINLINE constexpr bool __builtin_add_overflow(int16_t v1, int16_t v2, int16_t* pRes)
{
    if CONSTANT_EVALUATED {
        int32_t c = int32_t(v1) + int32_t(v2);
        *pRes = int16_t(c);
        return c < -32768 || c > 32767;
    } else {
        return _add_overflow_i16(0, v1, v2, pRes);
    }
}

__FORCEINLINE constexpr bool __builtin_add_overflow(int32_t v1, int32_t v2, int32_t* pRes)
{
    if CONSTANT_EVALUATED {
        int64_t c = int64_t(v1) + int64_t(v2);
        *pRes = int32_t(c);
        return c < -2147483648 || c > 2147483647;
    } else {
        return _add_overflow_i32(0, v1, v2, pRes);
    }
}

__FORCEINLINE constexpr bool __builtin_add_overflow(int64_t v1, int64_t v2, int64_t* pRes)
{
    if CONSTANT_EVALUATED {
        // https://blog.regehr.org/archives/1139
        int64_t s = int64_t(uint64_t(v1) + uint64_t(v2));
        *pRes = s;
        return (v1 < 0 && v2 < 0 && s >= 0) || (v1 >= 0 && v2 >= 0 && s < 0);
    } else {
        return _add_overflow_i64(0, v1, v2, pRes);
    }
}

#endif


/*
 * Implementation of builtins checking for overflow in subtraction.
 */

#if !__has_builtin(__builtin_sub_overflow)

__FORCEINLINE constexpr bool __builtin_sub_overflow(uint8_t v1, uint8_t v2, uint8_t* pRes)
{
    if CONSTANT_EVALUATED {
        auto Diff = uint8_t(v1 - v2);
        auto CarryVector = uint8_t((Diff & v2) ^ ((Diff ^ v2) & ~v1));
        *pRes = Diff;
        return (CarryVector >> 7);
    } else {
        return _subborrow_u8(0, v1, v2, pRes);
    }
}

__FORCEINLINE constexpr bool __builtin_sub_overflow(uint16_t v1, uint16_t v2, uint16_t* pRes)
{
    if CONSTANT_EVALUATED {
        auto Diff = uint16_t(v1 - v2);
        auto CarryVector = uint16_t((Diff & v2) ^ ((Diff ^ v2) & ~v1));
        *pRes = Diff;
        return (CarryVector >> 15);
    } else {
        return _subborrow_u16(0, v1, v2, pRes);
    }
}

__FORCEINLINE constexpr bool __builtin_sub_overflow(uint32_t v1, uint32_t v2, uint32_t* pRes)
{
    if CONSTANT_EVALUATED {
        auto Diff = uint32_t(v1 - v2);
        auto CarryVector = uint32_t((Diff & v2) ^ ((Diff ^ v2) & ~v1));
        *pRes = Diff;
        return bool(CarryVector >> 31);
    } else {
        return _subborrow_u32(0, v1, v2, pRes);
    }
}

__FORCEINLINE constexpr bool __builtin_sub_overflow(uint64_t v1, uint64_t v2, uint64_t* pRes)
{
    if CONSTANT_EVALUATED {
        auto Diff = uint64_t(v1 - v2);
        auto CarryVector = uint64_t((Diff & v2) ^ ((Diff ^ v2) & ~v1));
        *pRes = Diff;
        return bool(CarryVector >> 63);
    } else {
        return _subborrow_u64(0, v1, v2, pRes);
    }
}

__FORCEINLINE constexpr bool __builtin_sub_overflow(int8_t v1, int8_t v2, int8_t* pRes)
{
    if CONSTANT_EVALUATED {
        return checkAddOverflow(v1, -v2, pRes);
    } else {
        return _sub_overflow_i8(0, v1, v2, pRes);
    }
}

__FORCEINLINE constexpr bool __builtin_sub_overflow(int16_t v1, int16_t v2, int16_t* pRes)
{
    if CONSTANT_EVALUATED {
        return checkAddOverflow(v1, -v2, pRes);
    } else {
        return _sub_overflow_i16(0, v1, v2, pRes);
    }
}

__FORCEINLINE constexpr bool __builtin_sub_overflow(int32_t v1, int32_t v2, int32_t* pRes)
{
    if CONSTANT_EVALUATED {
        return checkAddOverflow(v1, -v2, pRes);
    } else {
        return _sub_overflow_i32(0, v1, v2, pRes);
    }
}

__FORCEINLINE constexpr bool __builtin_sub_overflow(int64_t v1, int64_t v2, int64_t* pRes)
{
    if CONSTANT_EVALUATED {
        return checkAddOverflow(v1, -v2, pRes);
    } else {
        return _sub_overflow_i64(0, v1, v2, pRes);
    }
}

#define __builtin_sub_overflow checkSubOverflow
#endif


#if !__has_builtin(__builtin_mul_overflow)

/*
 * @brief Checks for overflow in multiplication.
 * @tparam T_ Integer type
 * @param a First operand
 * @param b Second operand
 * @param pRes Pointer to store result into
 * @return If overflow occured.
 */
template<IsIntegerPrimitiveType T_>
constexpr inline bool __builtin_mul_overflow(T_ a, T_ b, T_* pRes)
{
    using T = CVRefRemoved<T_>;

    if CONSTANT_EVALUATED {
        constexpr T max = MAX_VALUE<T>;
        constexpr T min = MIN_VALUE<T>;
        // from
        // https://github.com/llvm/llvm-project/blob/34e63be925cc91d13f8c57c42664d9a678328f7c/libc/src/__support/memory_size.h
        bool overflow = (b > 0 && (a > max / b || a < min / b)) || (b < 0 && (a < max / b || a > min / b));
        if (!overflow)
            *pRes = a * b;
        return overflow;
    } else {
#if __has_builtin(__builtin_mul_overflow)
        return __builtin_mul_overflow(a, b, pRes);
#elif _MSC_VER
        if constexpr (IsSameAsOneOf<T, int8_t, char>) {
            int16_t p{};
            bool overflow = _mul_full_overflow_i8(a, b, &p);
            *pRes = int8_t(p);
            return overflow;
        } else if constexpr (IsSameAsOneOf<T, uint8_t, char8_t>) {
            uint16_t p{};
            bool overflow = _mul_full_overflow_u8(a, b, &p);
            *pRes = uint8_t(p);
            return overflow;
        } else if constexpr (IsSame<T, int16_t>) {
            return _mul_full_overflow_i16(a, b, pRes, pRes);
        } else if constexpr (IsSameAsOneOf<T, uint16_t, char16_t>) {
            return _mul_full_overflow_u16(a, b, pRes, pRes);
        } else if constexpr (
            IsSame<T, int32_t> || (IsSame<T, wchar_t> && IsIntegerSigned<wchar_t> && sizeof(wchar_t) == 4))
        {
            return _mul_full_overflow_i32(a, b, pRes, pRes);
        } else if constexpr (
            IsSameAsOneOf<T, uint32_t, char32_t> ||
            (IsSame<T, wchar_t> && !IsIntegerSigned<wchar_t> && sizeof(wchar_t) == 4))
        {
            return _mul_full_overflow_u32(a, b, pRes, pRes);
        } else if constexpr (IsSame<T, int64_t>) {
            return _mul_full_overflow_i64(a, b, pRes, pRes);
        } else if constexpr (IsSame<T, uint64_t>) {
            return _mul_full_overflow_u64(a, b, pRes, pRes);
        }
#else
#error "Not implemented"
#endif
    }
}
#endif


}  // namespace cm
#endif
