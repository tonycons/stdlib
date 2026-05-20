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
#include <commons/core/baseDefs.hh>


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


/// Determines how an integer is to be parsed from a string
enum class IntegerParsingScheme {

    /// Parses an integer according to the specification for a valid JSON integer literal.
    /// A sequence of digits, optionally prefixed with a minus sign for negative numbers.
    /// The string may not contain:
    /// -- Any leading zeros, except for the number 0 itself
    /// -- No '+' sign before the number
    /// -- Any prefixes such as 0x, 0b
    /// -- Any other non-digit character
    JSON,

    /// Parses an integer according to the specification for a valid YAML integer literal.
    /// The string must consist of a sequence of digits, with the following additions.
    /// -- One* '+' or '-' sign.
    /// -- The prefix '0x' denoting a hex number, and the number may contain the hex digits A-F, lower or upper case.
    /// -- The prefix '0b' denoting a binary number, and the number may only contain the digits '0' or '1'.
    /// -- The prefix '0o', or a leading '0', denoting an octal number, and the number may only contain the
    /// digits 0-7.
    /// -- There may be underscores separating digits, which are ignored.
    YAML,

    /// The default parsing scheme
    DEFAULT = JSON
};

///
/// Determines how an integer is to be represented as a string
///
enum class IntBaseFmt {

    B0 = 0,
    B1 = 1,
    B2 = 2,
    B3 = 3,
    B4 = 4,
    B5 = 5,
    B6 = 6,
    B7 = 7,
    B8 = 8,
    B9 = 9,
    B10 = 10,
    B16 = 16,

    /// Base 64 according to RFC 4648
    B64,
    /// Modified version of RFC 4648 where the '+' and '/' characters are replaced by '-' and '_', so that using URL
    /// encoders/decoders is no longer necessary and has no effect on the length of the encoded value.
    B64_URL,
    B64_crypt,
    B64_bcrypt,
    B64_Bash,
};
// namespace cm

///
/// Lookup tables/alphabets for particular base-N representations
///
constexpr inline auto intBaseTables = []() {
    constexpr static struct T
    {
        char const* charSet;
    } charSets[128] = {
        [int(IntBaseFmt::B2)] = {"0123456789abcdef"},                                                          //
        [int(IntBaseFmt::B3)] = {"0123456789abcdef"},                                                          //
        [int(IntBaseFmt::B8)] = {"0123456789abcdef"},                                                          //
        [int(IntBaseFmt::B10)] = {"0123456789abcdef"},                                                         //
        [int(IntBaseFmt::B16)] = {"0123456789abcdef"},                                                         //
        [int(IntBaseFmt::B64_crypt)] = {"./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"},   //
        [int(IntBaseFmt::B64_bcrypt)] = {"./ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"},  //
        [int(IntBaseFmt::B64_Bash)] = {"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ@_"},
    };
    return charSets;
}();

// ArrayRef<int> x;


template<IsInteger T>
constexpr inline auto BITS = unsigned(sizeof(T)) * 8u;

template<IsInteger T>
constexpr inline auto MIN_VALUE = IsIntegerSigned<T> ? (T(1) << (BITS<T> - 1)) : T(0);

template<IsInteger T>
constexpr inline auto MAX_VALUE = IsIntegerSigned<T> ? (~T(0) ^ MIN_VALUE<T>) : (~T(0));

// ///
// /// Defines math and helper functions on integers
// ///
// template<IsInteger T>
// struct IntHelper
// {
//     /// Constants
//     constexpr static auto BITS = unsigned(sizeof(T)) * 8u;
//     constexpr static auto MIN_VALUE = IsIntegerSigned<T> ? (T(1) << (BITS - 1)) : T(0);
//     constexpr static auto MAX_VALUE = IsIntegerSigned<T> ? (~T(0) ^ MIN_VALUE) : (~T(0));


// FORCEINLINE constexpr static UintN<BITS> assertPositive(T const& x) requires (IsIntegerSigned<T>)
// {
//     Assert(x >= 0, ASMS_PARAMETER);
//     return static_cast<UintN<BITS>>(x);
// }

///
/// Checks for overflow in addition.
/// @param x First operand
/// @param y Second operand
/// @param res Destination to store result
/// @return If overflow occurred
///
template<IsInteger T>
constexpr inline bool addOverflow(T const& x, T const& y, T& res)
{
    return __builtin_add_overflow(x, y, &res);
}

///
/// Checks for overflow in addition.
/// @param x First operand
/// @param y Second operand
/// @return If overflow occurred
///
template<IsInteger T>
constexpr inline static bool addOverflow(T const& x, T const& y)
{
    T res{};
    return addOverflow(x, y, res);
}

///
/// Checks for overflow in multiplication.
/// @param x First operand
/// @param y Second operand
/// @param res Destination to store result
/// @return If overflow occurred
///
template<IsInteger T>
constexpr inline static bool mulOverflow(T const& x, T const& y, T& res)
{
    if constexpr (IsSame<T, u256>) {
        using u512 = _BitInt(512);
        auto r = u512(x) * u512(y);
        res = u256(r);
        return (r > (~u256(0)));
    } else if constexpr (IsSame<T, i256>) {
        using i512 = _BitInt(512);
        auto r = i512(x) * i512(y);
        res = i256(r);
        return (r < MIN_VALUE<i256> || r > MAX_VALUE<i256>);
    } else {
        return __builtin_mul_overflow(x, y, &res);
    }
}

///
/// Checks for overflow in multiplication.
/// @param x First operand
/// @param y Second operand
/// @return If overflow occurred
///
template<IsInteger T>
constexpr inline static bool mulOverflow(T const& x, T const& y)
{
    T res{};
    return mulOverflow(x, y, res);
}


// ///
// /// Converts a string to an integer according to an IntegerParsingScheme
// /// Returns a Result containing the integer or if the conversion fails:
// /// - Overflow: The magnitude of the number inside the string cannot be represented by this integer type.
// /// - BadFormat: The string is not a valid integer according to the IntegerParsingScheme
// ///   * Note: If the string is allowed to represent a negative integer, but this integer type is unsigned, then
// ///   BadFormat is returned.
// ///
// template<IntegerParsingScheme S = IntegerParsingScheme::JSON>
// constexpr static Result<T, Union<Errors::Overflow, Errors::BadFormat>> tryParse(char const* str)
// {
//     if constexpr (S == IntegerParsingScheme::JSON) {
//         return _tryParseJSON(str);
//     } else if constexpr (S == IntegerParsingScheme::YAML) {
//         return _tryParseYAML(str);
//     } else {
//         static_assert(false, "Invalid IntegerParsingScheme");
//     }
// }

// ///
// /// "Parsing" a non-string value is assumed to be the same as casting it to the integer type.
// ///
// template<IntegerParsingScheme S = IntegerParsingScheme::DEFAULT>
// FORCEINLINE constexpr static T parse(auto x)
// {
//     return T(x);
// }

// ///
// /// Same as tryParse for a string, but expects that the given string is a valid integer.
// ///
// template<IntegerParsingScheme S = IntegerParsingScheme::DEFAULT>
// FORCEINLINE constexpr static T parse(char const* str)
// {
//     return tryParse<S>(str).unwrap();
// }

// ///
// /// Consteval version (for forcing clang to create a constant)
// ///
// template<IntegerParsingScheme S = IntegerParsingScheme::DEFAULT>
// FORCEINLINE consteval static u128 parse_cv(auto x)
// {
//     return parse(x);
// }

// ///
// /// Consteval version (for forcing clang to create a constant)
// ///
// template<IntegerParsingScheme S = IntegerParsingScheme::DEFAULT>
// FORCEINLINE consteval static auto tryParse_cv(auto x)
// {
//     return tryParse(x);
// }

// private:
//     NODISCARD constexpr static Result<T, Union<Errors::Overflow, Errors::BadFormat>>
//     _tryParseJSON(char const* str) noexcept
//     {
//         using namespace Errors;
//         auto result = T{};
//         auto negate = false;
//         auto UNSAFE(s) = str;

// if (*s == '-') {
//     if constexpr (IsIntegerSigned<T>) {
//         negate = true;
//         ++s;
//     } else {
//         return Err(BadFormat());
//     }
// }
// if (*s == '\0') {
//     return Err(BadFormat());
// }
// do {
//     if (*s < '0' || *s > '9') {
//         return Err(BadFormat());
//     }
//     if (mulOverflow(result, 10, result) || addOverflow(result, -('0' - *s), result)) {
//         return Err(Overflow());
//     }
//     if constexpr (IsIntegerSigned<T>) {
//         if (negate) {
//             result = -result;
//         }
//     }
//     ++s;
// } while (*s != '\0');

// return Ok(result);
// }

// NODISCARD constexpr static Result<T, Union<Errors::Overflow, Errors::BadFormat>>
// _tryParseYAML(char const* str) noexcept
// {
//     using namespace Errors;
//     auto result = T{};
//     auto negate = false;
//     auto base = 10;
//     auto UNSAFE(s) = str;

// if (*s == '-') {
//     if (IsIntegerSigned<T>) {
//         negate = true;
//         ++s;
//     } else {
//         return Err(BadFormat());
//     }
// } else if (*s == '+') {
//     ++s;
// } else if (*s == '0') {
//     ++s;
//     if (*s == 'x' || *s == 'X') {
//         base = 16;
//         ++s;
//     } else if (*s >= '0' && *s <= '7') {
//         base = 8;
//     } else if (*s == 'o') {
//         base = 8;
//         ++s;
//     } else if (*s == 'b' || *s == 'B') {
//         base = 2;
//         ++s;
//     }
// }
// if (*s == '\0') {
//     return Err(BadFormat());
// }
// do {
//     [[assume(base == 2 || base == 8 || base == 10 || base == 16)]];
//     switch (base) {
//     case 2:
//         if (*s != '0' && *s != '1') {
//             return Err(BadFormat());
//         }
//         if (mulOverflow(result, 2, result) || addOverflow(result, -('0' - *s), result)) {
//             return Err(Overflow());
//         }
//     case 8:
//         if (*s < '0' && *s > '7') {
//             return Err(BadFormat());
//         }
//         if (mulOverflow(result, 8, result) || addOverflow(result, -('0' - *s), result)) {
//             return Err(Overflow());
//         }
//     case 10:
//         if (*s < '0' || *s > '9') {
//             return Err(BadFormat());
//         }
//         if (mulOverflow(result, 10, result) || addOverflow(result, -('0' - *s), result)) {
//             return Err(Overflow());
//         }
//     case 16:
//         if (*s >= '0' && *s <= '9') {
//             if (mulOverflow(result, 16, result) || addOverflow(result, -('0' - *s), result)) {
//                 return Err(Overflow());
//             }
//         } else if (*s >= 'A' && *s <= 'F') {
//             if (mulOverflow(result, 16, result) || addOverflow(result, -('A' - *s), result)) {
//                 return Err(Overflow());
//             }
//         } else if (*s >= 'a' && *s <= 'f') {
//             if (mulOverflow(result, 16, result) || addOverflow(result, -('a' - *s), result)) {
//                 return Err(Overflow());
//             }
//         } else {
//             return Err(BadFormat());
//         }
//     default: __builtin_unreachable();
//     }

// if constexpr (IsIntegerSigned<T>) {
//     if (negate) {
//         result = -result;
//     }
// }
// s++;
// } while (*s != '\0');

// return Ok(result);
// }

// };  // namespace IntUtil

///
/// Computes a lookup table for a base N raised to positive integer powers k
/// @tparam T The integer type
/// @tparam Base The base N
///
template<IsInteger T, T Base>
struct PowerLookupTable
{
    T table[([] {
        auto k = T(1);
        auto size = 1u;
        for (; !mulOverflow(k, Base, k); size++)
            ;
        return size;
    }())];

    consteval PowerLookupTable()
    {
        auto k = T(1);
        auto i = T(0);
        (*this)[i++] = Base;
        while (!mulOverflow(k, Base, k)) {
            (*this)[i++] = k;
        }
    }

    constexpr auto length() const { return sizeof(table) / sizeof(T); }
    constexpr T& operator[](T const& i) UNSAFE({ return table[i]; });
    constexpr T const& operator[](T const& i) const UNSAFE({ return table[i]; });
};

///
/// Returns Base raised to the power k
/// @param k The power k
/// @return An optional containing Base raised to the power k, or None if power is too large
///
template<IsInteger auto Base>
[[maybe_unused]] constexpr static auto pow(IsInteger auto k)
{
    using T = CVRefRemoved<decltype(k)>;
    constexpr static PowerLookupTable<T, Base> table;
    if constexpr (IsIntegerSigned<T>) {
        if (k < 0) {
            // Since negative powers would result in a fractional value, truncating it to an integer is effectively 0
            return static_cast<T>(0);
        }
    }
    [[assume(k >= 0)]];
    // if the power is too large: Return the maximum possible value for the integer type
    if (UintN<BITS<T>>(k) >= table.length()) {
        return static_cast<T>(MAX_VALUE<T>);
    }
    return static_cast<T>(table[k]);
}


///
/// Returns the number of leading zeros in the binary representation of x.
/// @param x the value
///
[[maybe_unused]]
constexpr auto clz(IsInteger auto x) -> UintRanged<BITS<decltype(x)>>
{
    using R = UintRanged<BITS<decltype(x)>>;
    if constexpr (sizeof(x) <= sizeof(int)) {
        if constexpr (sizeof(x) == sizeof(int)) {
            return R(__builtin_clz(__builtin_bit_cast(unsigned int, x)));
        } else {
            if constexpr (IsIntegerSigned<decltype(x)>) {
                return R(__builtin_clz(__builtin_bit_cast(unsigned int, int(x))));
            } else {
                return R(__builtin_clz(x));
            }
        }
    } else if constexpr (sizeof(x) <= sizeof(long)) {
        return R(__builtin_clzl(__builtin_bit_cast(unsigned long, x)));
    } else if constexpr (sizeof(x) <= sizeof(long long)) {
        return R(__builtin_clzll(__builtin_bit_cast(unsigned long long, x)));
    } else {
        constexpr auto bits = R(sizeof(x)) * 8u;
        R count = 0;
        if (!x) {
            return bits;
        }
        for (auto i = R(bits) - 1; i >= 0; --i) {
            auto mask = static_cast<decltype(x)>(1) << i;
            if ((x & mask) != 0) {
                break;
            }
            ++count;
        }
        return R(count);
    }
}

///
/// Returns the base-logarithm of a given value.
/// Since this is integer math, this is equivalent to floor(log(x)) for floating-point.
/// The result is able to fit in a byte.
/// This works because the fastest increasing logarithm is log2, and so the maximum possible value which can be
/// obtained from this function, U256.log<2>(U256.MAX_VALUE), is exactly 255.
///
template<IsInteger auto Base = 10>
[[maybe_unused]]
constexpr auto log(IsInteger auto x)
{
    if constexpr (Base == 2) {
        return static_cast<u8>(((sizeof(x) * 8) - 1) - static_cast<unsigned>(clz(x)));
    } else if constexpr (Base == 16) {
        return log<2>(x) / 4;
    } else {
        constexpr static PowerLookupTable<decltype(x), Base> table;
        for (auto i = Base - 1; i >= 0; --i) {
            if (x >= table[i]) {
                return i;
            }
        }
        return Base;
    }
}

///
/// Rounds an integer to the next highest power of 2
///
constexpr auto roundup2 = [](usize x) -> usize {
    auto const L2 = log<2uz>(x);
    if (pow<2uz>(L2) != x) {
        return pow<2uz>(L2 + 1uz);
    } else {
        return x;
    }
};

///
///
///
FORCEINLINE
constexpr auto isPow2(auto const n) { return (n != 0) && ((n & (n - 1)) == 0); }

///
///
[[gnu::pure, clang::always_inline]]
inline size_t roundUpToMultiple(auto const m, auto const x)
{
    if (isPow2(m)) {
        return (x + (m - 1)) & ~(m - 1);
    } else {
        if ((x % m) != 0) {
            [[clang::musttail]] return roundUpToMultiple(m, x + 1);
        } else {
            return x;
        }
    }
}


///
/// Returns the most significant base-N digit of an integer.
///
template<unsigned base>
[[maybe_unused]]
constexpr static auto msd(IsInteger auto x)
{
    using T = decltype(x);
    if (x == 0) {
        return static_cast<T>(0);
    }
    if (x < 0) {
        if (x == MIN_VALUE<T>) [[unlikely]] {
            x += 1;
        }
        x = -x;
    }
    x /= pow<base>(log<base>(x));
    return x;
}


constexpr void ::cm::impl::outputStringForPrimitiveType(auto const& value, auto const& out)
{
    constexpr auto Base = IntBaseFmt::B10;
    constexpr auto S = IntegerParsingScheme::DEFAULT;
    using T = CVRefRemoved<decltype(value)>;

    if constexpr (IsUnderlyingTypeOneOf<T, char, char8_t>) {
        out(value);
    } else if constexpr (IsBool<T>) {
        if (value) {
            out('t');
            out('r');
            out('u');
            out('e');
        } else {
            out('f');
            out('a');
            out('l');
            out('s');
            out('e');
        }
    } else if constexpr (IsInteger<T>) {
        if constexpr (S == IntegerParsingScheme::JSON) {
            static_assert(Base == IntBaseFmt::B10, "JSON only supports base 10");
        }
        auto num = value;
        if constexpr (IsIntegerSigned<T>) {
            if (num < 0) {
                out('-');
                num = static_cast<T>(-value);
            }
        }
        // Handle prefixes
        if constexpr (Base == IntBaseFmt::B2) {
            if constexpr (S == IntegerParsingScheme::YAML) {
                out("0b");
            }
        } else if constexpr (Base == IntBaseFmt::B8) {
            if constexpr (S == IntegerParsingScheme::YAML) {
                out("0o");
            }
        } else if constexpr (Base == IntBaseFmt::B16) {
            if constexpr (S == IntegerParsingScheme::YAML) {
                out("0x");
            }
        }
        constexpr auto base = static_cast<T>(Base);
        if (num == 0) {
            out(intBaseTables[base].charSet[0]);
            return;
        }
        [&out, base](this auto&& self, T const num_) -> void {
            if (num_ < base) {
                out(intBaseTables[base].charSet[num_]);
            } else {
                self(num_ / 10);
                out(intBaseTables[base].charSet[num_ % base]);
            }
        }(num);

    } else if constexpr (IsFloatingPoint<T>) {

        // double or float
        auto val = double(value);
        i64 integer;
        int zero_threshold = 16;
        int zero_seq_len = 0;

        if (__builtin_isnan(val)) {
            out('N'), out('a'), out('N');
            return;
        } else if (__builtin_isinf(val)) {
            if (__builtin_signbit(val)) {
                out('-'), out('i'), out('n'), out('f');
                return;
            } else {
                out('i'), out('n'), out('f');
                return;
            }
        }
        if (val < 0) {
            out('-');
            val = -val;
        }
        integer = i64(val);
        if (integer == 0) {
            out('0');
            return;
        }
        OutputString(integer, out);
        out('.');

        while (true) {
            val = val - static_cast<double>(i64(val));
            val *= 10;
            integer = i64(val);
            if (integer == 0) {
                zero_seq_len++;
                out('0');
                if (zero_seq_len >= zero_threshold) {
                    return;
                }
            } else {
                zero_seq_len = 0;
                OutputString(integer, out);
            }
            zero_threshold--;
        }
    }
}

}  // namespace cm
