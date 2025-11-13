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


/**
 * Returns true if a pointer is pointing to data in program ROM (.text or .data section)
 * If true, we know that the data has infinite lifetime, and won't change throughout the program's runtime.
 * This can enable some optimizations.
 */
inline static bool commons_isRomData(void const* address)
{

#if __linux__
    /* Holy shit this is such a nice, clean hack and I love it.
     */
    extern char const etext, edata, end;
    return address >= &etext && address < &end;
#else
#error "Not implemented yet!"
#endif
}


namespace cm {

enum Access : unsigned int {
    READ_BIT = 1,
    WRITE_BIT = 1 << 1,
    EXECUTE_BIT = 1 << 2,
    READ_WRITE_BITS = READ_BIT | WRITE_BIT
};


struct Ptr
{
    Ptr(void* base, usize n_bytes);

    /**
     * Returns true if a pointer is pointing to data in program ROM (.text or .data section)
     * If true, we know that the data has infinite lifetime. This can enable some optimizations (see ResizableArray)
     */
    inline static bool isRomData(void const* address) { return commons_isRomData(address); }

    /**
     * Returns the access permissions for a range of memory.
     * @note In the case that some areas of the range have different permissions, it will return the lowest set of
     * permissions (that is, the level of access that is common to all areas)
     */
    static unsigned int getAccessBits(void* base, usize n_bytes) noexcept;

    static unsigned int leastPermissiveAccess(unsigned int access0, unsigned int access1) noexcept
    {
        return access0 & access1;
    }

    inline static bool canRead(void* base, usize n_bytes) noexcept
    {
        return getAccessBits(base, n_bytes) & Access::READ_BIT;
    }

    inline static bool canWrite(void* base, usize n_bytes) noexcept
    {
        return getAccessBits(base, n_bytes) & Access::WRITE_BIT;
    }

    inline static bool canReadWrite(void* base, usize n_bytes) noexcept
    {
        return getAccessBits(base, n_bytes) & Access::READ_WRITE_BITS;
    }

    // [[no_unique_address]] struct ReadableProperty : public ComputedProperty<MemorySegment> {
    //     operator bool() const noexcept {
    //         return container(&MemorySegment::r)->
    //     }
    // } r;

    template<typename T>
    UNSAFE(constexpr static T const* findSubstring(T const* str, T const* substring) {
        T const* a;
        T const* b = substring;

        if (*b == 0)
            return str;

        for (; *str != 0; str += 1) {
            if (*str != *b) {
                continue;
            }

            a = str;
            while (1) {
                if (*b == 0) {
                    return str;
                }
                if (*a++ != *b++) {
                    break;
                }
            }
            b = substring;
        }
        return nullptr;
    });
};

}  // namespace cm
#endif
