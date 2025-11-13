
#pragma once
#ifdef __inline_core_header__
#include "hashers/crc32_hasher.hh"

namespace cm {


///
/// Defines basic hashing functions given a hasher implementation
/// - Integers are not simply returned, but are hashed by the HashFunction.
/// - Floating point values are hashed as if they were integers.
/// - All pointers except char pointers are hashed as if they were integers.
/// - All char pointers are a composite hash of each character.
/// - All structs must be hashed according to their own hash function.
///
template<typename Hasher>
struct Hash
{
    constexpr static auto DEFAULT_SEED = Hasher::DEFAULT_SEED;
    using HashResult = Hasher::HashResult;
    using SeedType = Hasher::SeedType;

    template<typename T>
    requires (!IsClass<T> && (!IsUnderlyingTypeOneOf<T, char*, wchar_t*, char8_t*, char16_t*, char32_t*>))
    constexpr inline static HashResult hash(T in, SeedType seed = DEFAULT_SEED)
    {
        if constexpr (IsUnderlyingTypeOneOf<T, char*, wchar_t*, char8_t*, char16_t*, char32_t*>) {
            return _hashCharPtr(in);
        } else if constexpr (!IsClass<T>) {
            if constexpr (sizeof(in) == 1) {
                return Hasher::hash8(bit_cast<u8>(in), seed);
            } else if constexpr (sizeof(in) == 2) {
                return Hasher::hash16(bit_cast<u16>(in), seed);
            } else if constexpr (sizeof(in) == 4) {
                return Hasher::hash32(bit_cast<u32>(in), seed);
            } else if constexpr (sizeof(in) == 8) {
                return Hasher::hash64(bit_cast<u64>(in), seed);
            } else {
                static_assert(false, "Unable to hash primitive type");
                return HashResult{};
            }
        } else {
            if constexpr (IsPointer<T>) {
                return hash(reinterpret_cast<usize>(in), seed);
            } else {
                return in.template hash<Hasher>(seed);
            }
        }
    }

    constexpr static auto hashData(void const* mem, usize sizeBytes, SeedType seed = DEFAULT_SEED) UNSAFE({
        auto* bytes = static_cast<u8 const*>(mem);
        for (usize i = {}; i < sizeBytes; i += usize(1)) {
            seed = hash(bytes[size_t(i)], seed);
        };
        return seed;
    });

private:
    template<typename T>
    constexpr static auto _hashCharPtr(T in, SeedType seed = DEFAULT_SEED) UNSAFE({
        do {
            seed = hash(*in, seed);
        } while (*in++);
        return seed;
    });
};

using DefaultHasher = Hash<Crc32>;

}  // namespace cm
#endif
