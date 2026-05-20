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

// struct HeapAllocator final : Arena
// {
//     void*
//     void* _alloc(usize size_bytes) override { return __builtin_malloc(size_bytes); }

// bool _dealloc(void* ptr, usize size_bytes, bool set_zero) override
// {
//     (void)size_bytes;
//     __builtin_free(ptr);
//     if (set_zero) {
//         memset(ptr, 0, size_bytes);
//     }
//     return true;
// }
// };

}  // namespace cm
