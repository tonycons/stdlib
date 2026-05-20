/*
   Copyright 2026 Anthony A. Constantinescu.

   Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
   in compliance with the License. You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software distributed under the License
   is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
   or implied. See the License for the specific language governing permissions and limitations under
   the License.
*/

#pragma once

#include <commons/allocators/baseAllocator.hh>


namespace cm {

template<u32 Capacity, u32 Alignment = 8>
struct ScratchAllocator final : Arena
{
private:
    alignas(Alignment) u8 _buffer[Capacity]{};
    usize _offset{};

public:
    constexpr void* _base() override { return _buffer; }

    constexpr void* _alloc(usize size_bytes) override
    {
        if (_offset + (size_bytes) >= Capacity * size_bytes) {
            return nullptr;
        }
        void* ptr = (_buffer + _offset);
        _offset += size_bytes;
        return ptr;
    }

    constexpr bool _dealloc(void* ptr, usize size_bytes, bool set_zero) override
    {
        if (set_zero) {
            u8* addr = static_cast<u8*>(ptr);
            if (addr >= _buffer && addr < _buffer + Capacity) {
                memset(ptr, 0, size_bytes);
            }
        }
        return true;
    }

    void reset(bool wipe = false)
    {
        if (wipe) {
            memset(_buffer, 0, sizeof(_buffer));
        }
        _offset = 0;
    }
};

}  // namespace cm
