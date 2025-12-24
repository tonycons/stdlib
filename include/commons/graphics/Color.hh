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
#include "../core.hh"


namespace cm {

/**
 * @brief A color component represented by 8-bit red, green, blue, and alpha values.
 */
struct ColorRGBA8 : decltype([] {
    struct Impl
    {
        uint8_t r, g, b, a;

        [[maybe_unused]]
        constexpr static auto fromHex(u32 hex)
        {
            return Impl{
                .r = u8((hex & 0xff000000) >> 24),
                .g = u8((hex & 0x00ff0000) >> 16),
                .b = u8((hex & 0x0000ff00) >> 8),
                .a = u8(hex & 0x000000ff)};
        }

        [[maybe_unused]]
        constexpr bool operator==(Impl const& x) const
        {
            return r == x.r && g == x.g && b == x.b;
        }

        [[maybe_unused]]
        constexpr explicit operator u64() const
        {
            return (u64(r) << 16) | (u64(g) << 8) | (u64(b));
        }

        [[maybe_unused]]
        constexpr operator ColorRGBA8 const&() const;
    } _;
    return _;
}())
{

    constexpr static auto WHITE = fromHex(0xff'ff'ff'ff);
    constexpr static auto BLACK = fromHex(0x00'00'00'ff);


    constexpr ColorRGBA8() = default;

    constexpr ColorRGBA8(u8 r_, u8 g_, u8 b_, u8 a_)
        : Impl{r_, g_, b_, a_}
    {}

    constexpr ColorRGBA8(u32 rgba_)
        : Impl(fromHex(rgba_))
    {}

    constexpr bool operator==(ColorRGBA8 const&) const = default;
};

static_assert(ColorRGBA8::WHITE == ColorRGBA8::WHITE);
static_assert(ColorRGBA8{} == ColorRGBA8{});


constexpr ColorRGBA8::Impl::operator ColorRGBA8 const&() const { return *static_cast<ColorRGBA8 const*>(this); }

// constexpr auto ColorRGBA8::fromHex() -> ColorRGBA8 const& {}

}  // namespace cm
