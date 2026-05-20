/*

   Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
   in compliance with the License. You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software distributed under the License
   is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
   or implied. See the License for the specific language governing permissions and limitations under
   the License.
*/

#pragma once

#include "mem.hh"

namespace cm {


struct Arena
{
public:
    constexpr Arena() = default;
    constexpr Arena(Arena const&) = default;
    constexpr Arena& operator=(Arena const&) = default;
    constexpr Arena(Arena&&) = default;
    constexpr Arena& operator=(Arena&&) = default;
    virtual constexpr ~Arena() = default;

    virtual constexpr void* _alloc(usize size_bytes) = 0;
    virtual constexpr bool _dealloc(void* ptr, usize size_bytes, bool set_zero) = 0;
    virtual constexpr void* _base() = 0;

    template<typename T>
    constexpr Mem<T> alloc(usize count)
    {
        return Mem<T>(count, *this);
    }

    constexpr void dealloc(void* ptr, usize size_bytes, bool set_zero)
    {
        if (_dealloc(ptr, size_bytes, set_zero) && set_zero) {
            if !consteval {
                memset(ptr, 0, size_bytes);
            }
        }
    }

    template<typename T>
    T* getPointer(u32 offset)
    {
        return static_cast<T*>(getPointer(offset, sizeof(T)));
    }

    void* getPointer(u32 offset, usize stride) { return static_cast<u8*>(_base()) + (stride * offset); }
};


}  // namespace cm


template<typename T>
constexpr T* cm::arenaAlloc(cm::Arena& arena, usize length)
{
    return static_cast<T*>(arena._alloc(sizeof(T) * length));
}

template<typename T>
constexpr void cm::arenaFree(cm::Arena& arena, T* ptr, usize length)
{
    arena._dealloc(ptr, sizeof(T) * length, false);
}
