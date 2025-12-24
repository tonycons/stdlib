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

// #ifdef _WIN32
// #include "../system/win32lite.hh"
// #endif


inline static bool __ptr_is_rodata(void const* address)  // NOLINT
{
#if __linux__
    extern char const etext, edata, end;
    return address >= &etext && address < &end;
#else
#warning "Not implemented yet!"
    return false;
#endif
}

namespace cm {


enum Access : u8 {
    READ_BIT = 1,
    WRITE_BIT = 1 << 1,
    EXECUTE_BIT = 1 << 2,
    READ_WRITE_BITS = READ_BIT | WRITE_BIT
};


struct Ptr
{
    Ptr(void* base, usize n_bytes);

    ///
    /// Returns true if a pointer is an address to program ROM (.text or .data section)
    /// If true, we know that the data has infinite lifetime, and won't change throughout the program's runtime.
    /// This can enable some optimizations.
    /// @param address The address to check
    ///
    inline static bool isRomData(void const* address) { return __ptr_is_rodata(address); }


    static u8 leastPermissiveAccess(u8 access0, u8 access1) noexcept { return access0 & access1; }

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

    /**
     * Returns the access permissions for a range of memory.
     * @note In the case that some areas of the range have different permissions, it will return the lowest set of
     * permissions (that is, the level of access that is common to all areas)
     */
    static u8 getAccessBits(void* base, usize n_bytes) noexcept
    {
#if __linux__
        // TODO
        if (base == nullptr)
            return 0;

        (void)n_bytes;
        return Access::READ_WRITE_BITS;

#elif _WIN32
        auto result = 0u;
        MEMORY_BASIC_INFORMATION mbi = {};

        if (base == nullptr)
            return 0;

        if (::VirtualQuery(base, &mbi, sizeof(mbi)) == 0)
            // VirtualQuery will just fail if the address is above the highest memory address accessible to the process.
            return 0;

        if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS))
            return 0;

        if (mbi.Protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE))
            result |= Access::READ_BIT;

        if (mbi.Protect & (PAGE_WRITECOPY | PAGE_READWRITE | PAGE_EXECUTE_WRITECOPY | PAGE_EXECUTE_READWRITE))
            result |= Access::WRITE_BIT;

        if (mbi.Protect & (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY))
            result |= Access::EXECUTE_BIT;

        // How many bytes 'base' is, ahead of the page it belongs to;
        usize base_offset = reinterpret_cast<usize>(base) - reinterpret_cast<usize>(mbi.BaseAddress);

        // Determines if VirtualAlloc has checked n_bytes (because it returns if it reaches a page with different
        // attributes than the previous)
        usize n_bytes_checked = mbi.RegionSize - base_offset;
        if (n_bytes_checked < n_bytes) {
            void* next_base = reinterpret_cast<void*>(reinterpret_cast<usize>(base) + n_bytes_checked);
            result &= getAccessBits(next_base, n_bytes - n_bytes_checked);
        }

        return result;
#else
#warning "Unimplemented"
        if (base == nullptr)
            return 0;

        (void)n_bytes;
        return Access::READ_WRITE_BITS;
#endif
    }
};

}  // namespace cm
#endif
