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
        ArrayRef<char const> charSet;
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
    return ArrayRef<T>(const_cast<T*>(charSets), 128);
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
        (*this)[i++] = k;
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
constexpr static auto pow(IsInteger auto k) -> Optional<decltype(k)>
{
    using T = CVRefRemoved<decltype(k)>;
    constexpr static PowerLookupTable<T, Base> table;
    if constexpr (IsIntegerSigned<T>) {
        if (k < 0) {
            return None;
        }
    }
    [[assume(k >= 0)]];
    if (UintN<BITS<T>>(k) >= table.length()) {
        return None;
    }
    return table[k];
}


///
/// Returns the number of leading zeros in the binary representation of x.
/// @param x the value
///
constexpr static auto clz(IsInteger auto x) -> UintRanged<BITS<decltype(x)>>
{
    using R = UintRanged<BITS<decltype(x)>>;
    if constexpr (sizeof(x) <= sizeof(int)) {
        return R(__builtin_clz(__builtin_bit_cast(unsigned int, x)));
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
            count++;
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
constexpr static u8 log(IsInteger auto x)
{
    if constexpr (Base == 2) {
        return u8(((sizeof(x) * 8) - 1) - unsigned(clz(x)));
    } else if constexpr (Base == 16) {
        return log<2>(x) / 4;
    } else if constexpr (Base == 10) {
        constexpr u8 UNSAFE(guess[33]) = {0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4,
                                          5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9};
        u8 UNSAFE(digits) = guess[log<2>(x)];
        return u8(digits + ((x / 10) >= pow<10>(digits).value()));
    } else {
        static_assert(false, "Not implemented");
    }
}

///
/// Returns the most significant base-N digit of an integer.
///
template<unsigned BaseN>
constexpr static auto msd(IsInteger auto x) -> decltype(x)
{
    using T = decltype(x);
    return T(x / pow<BaseN>(log<BaseN>(x)).value());
}


// #if __clang__
// #define U128(str) \
//     ([]<bool C>(auto const& s) constexpr -> u128 { \
//         if constexpr (C) { \
//             return ::cm::_u128_c::parse_cv(s); \
//         } else { \
//             return ::cm::_u128_c::parse(s); \
//         } \
//     }.template operator()<__builtin_constant_p(str)>(str))
// #endif


}  // namespace cm

#endif
