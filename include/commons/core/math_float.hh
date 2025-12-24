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


    /*
        convert a float to fractional and integral components like the C standard library function frexp.
        This is a specialization that assumes *value* is not inf, nan, or 0.

        The function produces efficient, completely branchless assembly code under clang 21:

        mov     rax, rdi
        vmovd   ecx, xmm0
        mov     edx, ecx
        shr     edx, 23
        and     edx, 252
        add     edx, -126
        and     ecx, -2139095041
        or      ecx, 1056964608
        mov     dword ptr [rdi], edx
        mov     dword ptr [rdi + 4], ecx
        ret
    */
    constexpr static Tuple<float, int> frexpFiniteNonzero(float value)
    {
        u32 const bits = __builtin_bit_cast(u32, value);
        int const expon = ((bits & 0x7e000000) >> 23) - 126;
        u32 const final_bits = (bits & 0x807fffff) | 0x3f000000;
        return {__builtin_bit_cast(float, final_bits), expon};
    }

    /**
     Like frexpFiniteNonzero, but just returns the fraction.
    */
    constexpr static float fractionFiniteNonzero(float value)
    {
        u32 const bits = __builtin_bit_cast(u32, value);
        u32 const final_bits = (bits & 0x807fffff) | 0x3f000000;
        return __builtin_bit_cast(float, final_bits);
    }

    /*
        Convert a float to fractional and integral components like the C standard library function frexp.
        The function is equally correct, but more efficient than the C standard library implementation.
        (It assumes, of course, the standard IEEE-754 float32 representation).
        The function produces efficient, completely branchless assembly code under clang 21:

        vmovd   eax, xmm0
        lea     ecx, [rax - 1]
        mov     edx, eax
        shr     edx, 23
        and     edx, 252
        add     edx, -126
        mov     esi, eax
        and     esi, -2139095041
        or      esi, 1056964608
        xor     r8d, r8d
        cmp     ecx, 2139095039
        cmovb   r8d, edx
        cmovae  esi, eax
        mov     rax, rdi
        mov     dword ptr [rdi], r8d
        mov     dword ptr [rdi + 4], esi
        ret
    */
    constexpr static Tuple<float, int> frexp(float value)
    {
        // this part is the same logic as frexp_finite_nonzero
        u32 const bits = __builtin_bit_cast(u32, value);
        bool isNotFiniteOrIsZero = (bits >= 0x7f800000u || bits == 0);
        // make exponent 0 if value is inf,nan,or 0.
        int const expon = (((bits & 0x7e000000) >> 23) - 126) * !isNotFiniteOrIsZero;
        u32 const finalBits = (bits & 0x807fffff) | 0x3f000000;
        // the reasoning behind this is if value is NaN, inf, or 0, the returned "fraction" is essentially the same
        // value. otherwise, it should be the fraction we computed earlier. so we want to select (without branching)
        // either the original value or the fraction. We do this by making a 64-bit integer where the upper 32 bits
        // contain the original float, the lower 32 bits contain the fraction float then we use clever bitmasking to
        // select either the lower or upper 32 bits depending on whether value is nan, inf, or 0.
        u64 const choice = (u64(bits) << 32) | u64(finalBits);
        // then use bitmasking to select either lower or upper 32 bits. note if isNotFiniteOrIsZero == 0, the shifts do
        // nothing and just get the lower 32 bits shift is either 0 or 32.
        u8 const shift = u8(u8(isNotFiniteOrIsZero) << 5);
        u32 const finalFinalBits = u32((choice & (u64(0xffffffff) << shift)) >> shift);

        float const result = __builtin_bit_cast(float, finalFinalBits);
        return {result, expon};
    }

    ///
    /// Calculates the base 2 logarithm, accurate to at least 36 bits.
    /// From https://github.com/emjay2k/Approximations/blob/master/logapprox.h#L82
    ///
    [[gnu::flatten]]
    constexpr static float log2(float value)
    {
        constexpr float a = 0.59329970349044314f, b = 2.3979646338966889f, c = -0.96358966800238843f,
                        d = -1.8439274267589987f, e = -0.18374724264449727f, f = 0.1068562844523792f,
                        g = 1.2392957064266512f, h = 2.0062979261642901f, i = 0.63680961689938775f,
                        j = 0.028211791264274255f;

        u32 mask = (__builtin_bit_cast(u32, value) & 0x7fffffffu);
        if (mask >= 0x7f800000u || value <= 0.0f) [[unlikely]] {
            if (value < 0.0f) {
                return -QNAN;
            } else if (mask == 0) {
                return NEG_INF;
            } else {
                return value;
            }
        }

        auto [dM, iExp] = frexpFiniteNonzero(value);
        auto dM2 = dM * dM;
        auto dM3 = dM * dM2;
        auto dM4 = dM2 * dM2;
        auto x = 1.0f / (((f * dM4 + h * dM2) + (g * dM3 + i * dM)) + j);
        return float(iExp) + x * (((a * dM4 + c * dM2) + (b * dM3 + d * dM)) + e);
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
