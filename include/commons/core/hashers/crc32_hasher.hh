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


/**
 * CRC32 hasher
 */
struct Crc32
{
    constexpr static auto DEFAULT_SEED = 1;
    using SeedType = u32;
    using HashResult = u32;

    /**
     * @brief The table of constants used in the software version of the hasher.
     * Using the Castagnoli table allows the software version to match with the hardware.
     * https://docs.rs/crc/1.6.0/crc/crc32/constant.CASTAGNOLI_TABLE.html
     */
    constexpr static auto const& table()
    {
        constexpr static VectorU32x<256> tbl = {
        // clang-format off
#include HEADER(resources/crc32_castagnoli_table.csv)
            // clang-format on
        };
        return tbl;
    }

    ///
    /// Hashes an 8-bit input. Uses hardware features if possible.
    ///
    constexpr static u32 hash8(u8 value, u32 seed) noexcept
    {
#if (__x86_64__ || __aarch64__)
        if consteval
#endif
        {
            auto const& tbl = table();
            auto const hash = seed;
            return tbl[(hash ^ value) & 0xffu] ^ (hash >> 8u);
        }
#if (__x86_64__ || __aarch64__)
        else {
#if __x86_64__
            return static_cast<u32>(__builtin_ia32_crc32qi(seed, value));

#elif __aarch64__
            return static_cast<u32>(__crc32cb(seed, value));
#endif
        }
#endif
    }

    ///
    /// Hashes a 16-bit input. Uses hardware features if possible.
    ///
    constexpr static u32 hash16(u16 value, u32 seed) noexcept
    {
#if (__x86_64__ || __aarch64__)
        if consteval
#endif
        {
            auto const& tbl = table();
            seed = tbl[(seed ^ bit_cast<VectorU8x<2>>(value)[0]) & 0xffu] ^ (seed >> 8u);
            seed = tbl[(seed ^ bit_cast<VectorU8x<2>>(value)[1]) & 0xffu] ^ (seed >> 8u);
            return seed;
        }
#if (__x86_64__ || __aarch64__)
        else {
#if __x86_64__
            return static_cast<u32>(__builtin_ia32_crc32hi(seed, value));
#elif __aarch64__
            return static_cast<u32>(__crc32ch(seed, value));
#endif
        }
#endif
    }

    ///
    /// Hashes a 32-bit input. Uses hardware features if possible.
    ///
    constexpr static u32 hash32(u32 value, u32 seed) noexcept
    {
#if (__x86_64__ || __aarch64__)
        if consteval
#endif
        {
            auto const& tbl = table();
            seed = tbl[(seed ^ bit_cast<VectorU8x<4>>(value)[0]) & 0xffu] ^ (seed >> 8u);
            seed = tbl[(seed ^ bit_cast<VectorU8x<4>>(value)[1]) & 0xffu] ^ (seed >> 8u);
            seed = tbl[(seed ^ bit_cast<VectorU8x<4>>(value)[2]) & 0xffu] ^ (seed >> 8u);
            seed = tbl[(seed ^ bit_cast<VectorU8x<4>>(value)[3]) & 0xffu] ^ (seed >> 8u);
            return seed;
        }
#if (__x86_64__ || __aarch64__)
        else {
#if __x86_64__
            return static_cast<u32>(__builtin_ia32_crc32si(seed, value));

#elif __aarch64__
            return static_cast<u32>(__crc32cw(seed, value));
#endif
        }
#endif
    }

    ///
    /// Hashes a 64-bit input. Uses hardware features if possible.
    ///
    constexpr static u32 hash64(u64 value, u32 seed) noexcept
    {
#if (__x86_64__ || __aarch64__)
        if consteval
#endif
        {
            auto const& tbl = table();
            seed = tbl[(seed ^ bit_cast<VectorU8x<8>>(value)[0]) & 0xffu] ^ (seed >> 8u);
            seed = tbl[(seed ^ bit_cast<VectorU8x<8>>(value)[1]) & 0xffu] ^ (seed >> 8u);
            seed = tbl[(seed ^ bit_cast<VectorU8x<8>>(value)[2]) & 0xffu] ^ (seed >> 8u);
            seed = tbl[(seed ^ bit_cast<VectorU8x<8>>(value)[3]) & 0xffu] ^ (seed >> 8u);
            seed = tbl[(seed ^ bit_cast<VectorU8x<8>>(value)[4]) & 0xffu] ^ (seed >> 8u);
            seed = tbl[(seed ^ bit_cast<VectorU8x<8>>(value)[5]) & 0xffu] ^ (seed >> 8u);
            seed = tbl[(seed ^ bit_cast<VectorU8x<8>>(value)[6]) & 0xffu] ^ (seed >> 8u);
            seed = tbl[(seed ^ bit_cast<VectorU8x<8>>(value)[7]) & 0xffu] ^ (seed >> 8u);
            return seed;
        }
#if (__x86_64__ || __aarch64__)
        else {
#if __x86_64__
            return static_cast<u32>(__builtin_ia32_crc32di(seed, value));

#elif __aarch64__
            return static_cast<u32>(__crc32cd(seed, value));
#endif
        }
#endif
    }
};


}  // namespace cm
#endif
