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


namespace cm {

///
/// Utilities for a double-precision floating-point number.
///
struct Double
{
    constexpr static double MIN_VALUE = -1.7976931348623157E+308;
    constexpr static double MAX_VALUE = 1.7976931348623157E+308;

    constexpr static double ZERO = double(0);
    /// Positive infinity
    constexpr static double POS_INF = bit_cast<double>(0x7FF0000000000000);
    /// Negative infinity
    constexpr static double NEG_INF = bit_cast<double>(0xFFF0000000000000);

    /// Quiet NaN
    /// The most commonly cited hexadecimal value for a quiet NaN has the smallest possible mantissa.
    constexpr static double QNAN = bit_cast<double>(0x7ff8000000000000);

    /// Signaling NaN
    constexpr static double SNAN = bit_cast<double>(0x7ff0000000000001);

    /// Represents the smallest positive Double value that is greater than zero.
    constexpr static double MIN_EPSILON = 4.94065645841247E-324;

    /// Represents the natural logarithmic base, specified by the constant, e.
    constexpr static double E = 2.7182818284590452354;

    /// Represents PI
    constexpr static double PI = 3.1415926535897932385;

    /// Represents Tau, or PI / 2
    constexpr static double TAU = 6.2831853071795864769;


    FORCEINLINE constexpr static int signbit(double x) { return (bit_cast<u64>(x) >> 63u) == 1; }

    /// Returns the sign of a double
    FORCEINLINE constexpr static int signum(double x) { return signbit(x) == 1 ? -1 : 1; }


    ///
    /// Returns true if double is NaN
    ///
    FORCEINLINE constexpr static bool isNaN(double x)
    {
        auto u = bit_cast<u64>(x);
        return (static_cast<u32>(u >> 32) & 0x7fffffff) + (static_cast<u32>(u) != 0) > 0x7ff00000;
    }

    ///
    /// Returns true if a double is positive or negative infinity
    ///
    FORCEINLINE constexpr static int isInf(double x)
    {
        auto u = bit_cast<u64>(x);
        return (static_cast<u32>(u >> 32) & 0x7fffffff) == 0x7ff00000 && static_cast<u32>(u) == 0;
    }

    //
    FORCEINLINE constexpr static double fma(double x, double y, double z)
    {
        _Pragma("clang fp contract(fast)");
        return (x * y) + z;
    }

    [[gnu::flatten]]
    constexpr inline static double floor(double value)
    {
        if consteval {
            return __builtin_floor(value);
        } else {
#if __x86_64__
            return __builtin_ia32_roundsd({value, value}, {value, value}, 9)[0];
#else
            return __builtin_floor(value);
#endif
        }
    }

    [[gnu::flatten]]
    constexpr inline static double ceil(double value)
    {
        if consteval {
            return __builtin_ceil(value);
        } else {
#if __x86_64__
            return __builtin_ia32_roundsd({value, value}, {value, value}, 10)[0];
#else
            return __builtin_ceil(value);
#endif
        }
    }

    [[gnu::flatten]]
    constexpr inline static double trunc(double value)
    {
        if consteval {
            return __builtin_trunc(value);
        } else {
#if __x86_64__
            return __builtin_ia32_roundsd({value, value}, {value, value}, 11)[0];
#else
            return __builtin_trunc(value);
#endif
        }
    }


    constexpr static double pow(double x, int n)
    {
        double r = 1.0;
        decltype(n) y;

        if (x == 1.0)
            return 1.0;

        if (bit_cast<i64>(x) == bit_cast<i64>(-1.0))
            return !(n & 1) ? 1.0 : -1.0;

        if (n == ::cm::MAX_VALUE<int>)
            return (x < 1.0 && x > -1.0) ? 0.0 : (1.0 / 0.0);

        if (n == ::cm::MIN_VALUE<int>)
            return 0.0;

        y = n;
        if (n < 0)
            n = -n;

        for (; n > 0; n--, r *= x)
            ;
        return y < 0 ? 1.0 / r : r;
    }


    constexpr static auto frexp_v2_finite_nonzero(double value) -> Tuple<double, int>
    {
        u64 const bits = __builtin_bit_cast(u64, value);
        int const expon = ((bits >> 52) & 0x7FF) - 1022;
        u64 const final_bits = (bits & 0x800fffff'ffffffff) | 0x3fe0000000000000;
        return {__builtin_bit_cast(double, final_bits), expon};
    }

    constexpr static auto frexp_v2(double value) -> Tuple<double, int>
    {
        u64 const bits = __builtin_bit_cast(u64, value);
        bool const isNotFiniteOrIsZero = (bits >= 0x7FF0000000000000 || bits == 0);

        int const expon = (((bits >> 52) & 0x7FF) - 1022) * !isNotFiniteOrIsZero;
        u64 const finalBits = (bits & 0x800fffff'ffffffff) | 0x3fe0000000000000;

        u128 const choice = (u128(bits) << 64) | u128(finalBits);
        u8 const shift = u8(u8(isNotFiniteOrIsZero) << 6);
        u64 const finalFinalBits = u64((choice & (u128(~0ULL) << shift)) >> shift);

        double result = __builtin_bit_cast(double, finalFinalBits);
        return {result, expon};
    }

    ///
    /// Calculates the base 2 logarithm, accurate to at least 50 bits.
    /// From https://github.com/emjay2k/Approximations/blob/master/logapprox.h#L195
    ///
    constexpr static double log2(double value)
    {
        using T = double;
        constexpr double a = 1.000000000000000000000e+00L, b = 1.251649209001242901707e+01L,
                         c = 2.046583854860732643033e+01L, d = -1.097536826489419503616e+01L,
                         e = -1.881965876655596403566e+01L, f = -4.046684236630626152476e+00L,
                         g = -1.406193705389736370304e-01L, h = 1.518692933639071429575e-01L,
                         i = 3.897795033656223484542e+00L, j = 1.728188727732104723600e+01L,
                         k = 2.168720176727976678421e+01L, l = 8.568477446142038544963e+00L,
                         m = 9.581795852523320444760e-01L, n = 1.851054408942580734032e-02L;

        u64 mask = (__builtin_bit_cast(u64, value));
        if (mask >= 0x7FF0000000000000 || value <= 0.0) [[unlikely]] {
            if (value < 0.0) {
                return -QNAN;
            } else if (mask == 0) {
                return NEG_INF;
            } else {
                return value;
            }
        }
        auto [dM, iExp] = frexp_v2_finite_nonzero(value);
        T dM2 = dM * dM;
        T dM3 = dM * dM2;
        T dM4 = dM2 * dM2;
        T dM5 = dM2 * dM3;
        T dM6 = dM3 * dM3;
        T x = 1.0 / ((((h * dM6 + n) + (i * dM5 + m * dM)) + (j * dM4 + l * dM2)) + k * dM3);
        return iExp + x * (((((a * dM6 + g) + (c * dM4 + d * dM3)) + b * dM5) + f * dM) + e * dM2);
    }

    ///
    /// Calculates the base 10 logarithm, accurate to at least 50 bits
    ///
    constexpr static double log10(double x)
    {
        return double(0.301029995663981195213738894724493026768189881462108541310L) * log2(x);
    }

    ///
    /// Calculates the base e logarithm, accurate to at least 50 bits
    ///
    constexpr static double ln(double x)
    {
        return double(0.693147180559945309417232121458176568075500134360255254120L) * log2(x);
    }

} inline constexpr Double;

}  // namespace cm
#endif
