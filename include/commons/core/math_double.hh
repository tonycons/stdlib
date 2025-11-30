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
/// Utilities for a double-precision floating-point number.
///
struct Double
{
    constexpr static double MIN_VALUE = -1.7976931348623157E+308;

    constexpr static double MAX_VALUE = 1.7976931348623157E+308;

    constexpr static double ZERO = double(0);

    constexpr static double POS_INF = bit_cast<double>(0x7FF0000000000000);

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

    /// Returns true if double is NaN
    FORCEINLINE constexpr static bool isNaN(double x)
    {
        auto u = bit_cast<u64>(x);
        return (static_cast<u32>(u >> 32) & 0x7fffffff) + (static_cast<u32>(u) != 0) > 0x7ff00000;
    }

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

    //
    FORCEINLINE constexpr static double trunc(double x)
    {
        long long i = static_cast<long long>(x);
        if (x < 0) {
            return static_cast<double>(i + (i < static_cast<long long>(x)));
        }
        return static_cast<double>(i - (i > static_cast<long long>(x)));
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

} inline constexpr Double;


///
/// Converts a floating-point value into a String
///
template<IsFloatingPoint T>
void OutputString(T val, auto const& writer)
{
    i64 integer;
    int zero_threshold = 16;
    int zero_seq_len = 0;

    if (__builtin_isnan(val)) {
        writer('N'), writer('a'), writer('N');
        return;
    } else if (__builtin_isinf(val)) {
        if (__builtin_signbit(val)) {
            writer('-'), writer('i'), writer('n'), writer('f');
            return;
        } else {
            writer('i'), writer('n'), writer('f');
            return;
        }
    }
    if (val < 0) {
        writer('-');
        val = -val;
    }
    integer = i64(val);
    if (integer == 0) {
        writer('0');
        return;
    }
    OutputString(integer, writer);
    writer('.');

    while (true) {
        val = val - static_cast<double>(i64(val));
        val *= 10;
        integer = i64(val);
        if (integer == 0) {
            zero_seq_len++;
            writer('0');
            if (zero_seq_len >= zero_threshold) {
                return;
            }
        } else {
            zero_seq_len = 0;
            OutputString(integer, writer);
        }
        zero_threshold--;
    }
}

}  // namespace cm
#endif
