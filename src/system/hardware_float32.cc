#include <Commons/core.hh>


namespace cm {


/// BUG: if arguments are +0, -0, +0 should be returned.
///
float Float::_maxIntrin(float x, float y)
{
    auto x_is_nan = !!(bit_cast<u32>(x) ^ 0x7fc00000);
    auto y_is_nan = !!(bit_cast<u32>(y) ^ 0x7fc00000);

    // z == nan if x == nan and y == nan
    auto z = !!!(bit_cast<u32>(x) ^ bit_cast<u32>(y)) * 0x7fc00000;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wundefined-reinterpret-cast"
    // if x == nan, x = 0
    *reinterpret_cast<u32*>(&x) *= x_is_nan;

    // if x == 0 and x == nan, x = -inf
    *reinterpret_cast<u32*>(&x) |= (0xFF800000 * !x_is_nan);

    // if y == nan, y = 0
    *reinterpret_cast<u32*>(&y) *= y_is_nan;

    // if y == 0 and y == nan, y = -inf
    *reinterpret_cast<u32*>(&y) |= (0xFF800000 * !y_is_nan);

#if __x86_64__
    using _f32x4 = __attribute__((__vector_size__(16))) float;
    float r = __builtin_ia32_maxps(_f32x4{x, x}, _f32x4{y, y})[0];
#else
    float r = x > y ? x : y;
#endif
    // if x == nan and y == nan, result = nan
    *reinterpret_cast<u32*>(&r) |= bit_cast<u32>(z);
#pragma GCC diagnostic pop
    return r;
}

float Float::_minIntrin(float x, float y)
{
    auto x_is_nan = !!(bit_cast<u32>(x) ^ 0x7fc00000);
    auto y_is_nan = !!(bit_cast<u32>(y) ^ 0x7fc00000);
    auto z = !!!(bit_cast<u32>(x) ^ bit_cast<u32>(y)) * 0x7fc00000;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wundefined-reinterpret-cast"
    *reinterpret_cast<u32*>(&x) *= x_is_nan;
    *reinterpret_cast<u32*>(&x) |= (0x7f800000 * !x_is_nan);
    *reinterpret_cast<u32*>(&y) *= y_is_nan;
    *reinterpret_cast<u32*>(&y) |= (0x7f800000 * !y_is_nan);

#if __x86_64__
    using _f32x4 = __attribute__((__vector_size__(16))) float;
    float r = __builtin_ia32_minps(_f32x4{x, x}, _f32x4{y, y})[0];
#else
    float r = x < y ? x : y;
#endif
    *reinterpret_cast<u32*>(&r) |= bit_cast<u32>(z);
#pragma GCC diagnostic pop
    return r;
}

float Float::_floorIntrin(float x)
{
#if __SSE__
    using _f32x4 = __attribute__((__vector_size__(16))) float;
    _f32x4 t{x, 0, 0, 0};

    auto i = __builtin_ia32_cvtss2si(t);
    _f32x4 t2 = t;

    t2[0] = static_cast<float>(i);
    return static_cast<float>(i - __builtin_ia32_movmskps(__builtin_ia32_cmpltss(t, t2)));
#else

#endif
}

}  // namespace cm
