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

#include <commons/core.hh>

#ifdef _WIN32
#include <commons/system/win32lite.hh>
#endif


namespace cm {

UNSAFE_BEGIN;
u8 Ptr::getAccessBits(void* base, usize n_bytes) noexcept
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

    // Determines if VirtualAlloc has checked n_bytes (because it returns if it reaches a page with different attributes
    // than the previous)
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
UNSAFE_END;

}  // namespace cm
