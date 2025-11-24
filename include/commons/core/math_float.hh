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
/// Utilities for a single-precision floating-point number.
///
struct Float
{
    /// Represents the smallest possible value of a float, other than negative infinity.
    constexpr static float MIN_VALUE = -3.40282347E+38f;

    /// Represents the largest possible value of a float, other than positive infinity.
    constexpr static float MAX_VALUE = 3.40282347E+38f;

    /// Represents zero
    constexpr static float ZERO = bit_cast<float>(0);

    /// Represents 'negative zero'
    constexpr static float NEG_ZERO = bit_cast<float>(0x80000000);

    /// Represents positive infinity
    constexpr static float POS_INF = bit_cast<float>(0x7F800000);

    /// Represents negative infinity
    constexpr static float NEG_INF = bit_cast<float>(0xFF800000);

    /// Quiet NaN
    constexpr static float QNAN = bit_cast<float>(0x7fc00000);

    /// Signaling NaN
    constexpr static float SNAN = bit_cast<float>(0x7F800001);

    /// Represents the smallest positive float value that is greater than zero.
    constexpr static float MIN_EPSILON = 1.401298E-45f;

    /// Represents the natural logarithmic base, specified by the constant, e.
    constexpr static float E = 2.7182818284590452354f;

    /// Represents PI
    constexpr static float PI = 3.1415926535897932385f;

    /// Represents Tau, or PI / 2
    constexpr static float TAU = 6.2831853071795864769f;

    ///
    /// Returns whether the sign bit is set
    ///
    FORCEINLINE constexpr static int signbit(float x) { return (bit_cast<u32>(x) >> 31u) == 1; }

    /// Returns the sign of the float
    FORCEINLINE constexpr static int signum(float x) { return signbit(x) == 1 ? -1 : 1; }

    /// Returns true if float is NaN
    FORCEINLINE constexpr static bool isNaN(float x)
    {  // Note sign bit: either 0 or 1, exponent: all 1, mantissa: anything except all 0
        return ((bit_cast<u32>(x) & 0x7fffffffu) > 0x7f800000u) != 0u;
    }

    /// Returns true if float is positive or negative infinity
    FORCEINLINE constexpr static bool isInf(float x) { return (bit_cast<u32>(x) & 0x7fffffffu) == 0x7f800000u; }

    /// Fused multiply-add intrinsic
    FORCEINLINE constexpr static float fma(float x, float y, float z)
    {
        // if compiled with -mfma, compiles to a single instruction: vfmadd213ss
#pragma clang fp contract(fast)
        return (x * y) + z;
    }

    ///
    /// Notes: if one argument is NaN, the other is returned. If both are NaN, then NaN is returned.
    /// BUG: if arguments are +0, -0, +0 should be returned.
    ///
    FORCEINLINE constexpr static float max(float x, float y)
    {
        if !consteval {
            return _maxIntrin(x, y);
        } else {
            if (isNaN(x)) {
                if (isNaN(y))
                    return QNAN;
                return y;
            }
            if (isNaN(y)) {
                return x;
            }
            return x > y ? x : y;
        }
    }

    //
public:
    NODISCARD constexpr static float min(float x, float y)
    {
        if !consteval {
            return _minIntrin(x, y);
        } else {
            if (isNaN(x)) {
                if (isNaN(y))
                    return __builtin_bit_cast(float, 0x7fc00000);
                return y;
            }
            if (isNaN(y)) {
                return x;
            }
            return x < y ? x : y;
        }
    }

    FORCEINLINE constexpr static float trunc(float x)
    {
#if __AVX__
        if !consteval {
            using _f32x4 = __attribute__((__vector_size__(16))) float;
            return __builtin_ia32_roundss(_f32x4{x, 0, 0, 0}, _f32x4{x, 0, 0, 0}, 11)[0];
        } else
#endif
        {
            auto i0 = bit_cast<i32>(x);
            auto sx = i0 & bit_cast<i32>(0x80000000);
            auto j0 = ((i0 >> 23) & 0xff) - 0x7f;

            if (j0 < 23) {
                if (j0 < 0)
                    /* The magnitude of the number is < 1 so the result is +-0.  */
                    x = bit_cast<float>(x);
                else
                    x = bit_cast<float>(sx | (i0 & ~(0x007fffff >> j0)));
            } else {
                if (j0 == 0x80)
                    /* x is inf or NaN.  */
                    return x + x;
            }
            return x;
        }
    }

    //
    FORCEINLINE constexpr static float round(float x)
    {
        auto i0 = bit_cast<i32>(x);
        auto j0 = ((i0 >> 23) & 0xff) - 0x7f;

        if (j0 < 23) {
            if (j0 < 0) {
                i0 &= 0x80000000;
                if (j0 == -1)
                    i0 |= 0x3f800000;
            } else {
                u32 i = 0x007fffff >> j0;
                if ((i0 & bit_cast<i32>(i)) == 0) {
                    // X is integral.
                    return x;
                }
                i0 += 0x00400000 >> j0;
                i0 &= ~i;
            }
        } else {
            // Inf or NaN
            return j0 == 0x80 ? x + x : x;
        }
        return bit_cast<float>(i0);
    }

    FORCEINLINE constexpr static float floor(float x)
    {
        if consteval {
            return _floor(x);
        } else {
            return _floorIntrin(x);
        }
    }


private:
    static float _maxIntrin(float x, float y);
    static float _minIntrin(float x, float y);
    static float _floorIntrin(float x);


    constexpr static float _floor(float x)
    {
        auto i0 = bit_cast<__INT32_TYPE__>(x);
        auto j0 = ((i0 >> 23) & 0xff) - 0x7f;

        if (j0 < 0) {
            if (i0 >= 0) {
                i0 = 0;
            } else if ((i0 & 0x7fffffff) != 0) {
                i0 = __builtin_bit_cast(int, 0xbf800000);
            }
        } else if (j0 < 23) {
            auto i = (0x007fffff) >> j0;
            if ((i0 & i) == 0) {
                return x;
            }
            if (i0 < 0) {
                i0 += (0x00800000) >> j0;
            }
            i0 &= (~i);
        } else {
            return j0 == 0x80 ? x + x : x;
        }
        return bit_cast<float>(i0);
    }

} inline constexpr Float;


//
//
//


//
//
//
//
constexpr long double fmal(long double x, long double y, long double z)
{
#pragma clang fp contract(fast)
    return (x * y) + z;
}


//
//
//
//

//
//
//
//
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wfloat-equal"


static_assert(Float.max(1.0f, 2.0f) == 2.0f);
static_assert(Float.max(0.001f, -0.2f) == 0.001f);
static_assert(Float.max(-1.0f, Float.QNAN) == -1.0f);
static_assert(Float.max(Float.QNAN, -1.0f) == -1.0f);
static_assert(Float.max(3.141f, Float.QNAN) == 3.141f);
static_assert(Float.max(Float.QNAN, 3.141f) == 3.141f);
static_assert(Float.max(-3.1f, 0.0f) == 0.0f);
static_assert(Float.max(0.0f, -3.1f) == 0.0f);
static_assert(Float.max(+0.0f, -0.0f) == +0.0f);
static_assert(Float.isNaN(Float.max(Float.QNAN, Float.QNAN)));
static_assert(Float.trunc(-3.49f) == -3.0f);
static_assert(Float.trunc(1.33f) == 1.0f);

#pragma clang diagnostic pop

}  // namespace cm

#include "math_double.hh"  // IWYU pragma: keep
#endif
