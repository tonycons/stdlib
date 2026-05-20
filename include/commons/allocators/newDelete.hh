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

// template<typename T>
// constexpr void* operator new(usize sizeBytes, cm::Arena& allocator)
// {
//     return allocator.alloc(sizeBytes / sizeof(T)).val();
// }

// template<typename T>
// constexpr void* operator new(usize sizeBytes, cm::Arena<T>* allocator)
// {
//     return allocator->alloc(sizeBytes / sizeof(T)).val();
// }

// template<typename T>
// constexpr void* operator new[](usize sizeBytes, cm::Arena<T>& allocator)
// {
//     return allocator.alloc(sizeBytes / sizeof(T)).val();
// }

// template<typename T>
// constexpr void* operator new[](usize sizeBytes, cm::Arena<T>* allocator)
// {
//     return allocator->alloc(sizeBytes / sizeof(T)).val();
// }

// template<typename T>
// constexpr void operator delete(void* ptr, cm::Arena<T>& allocator)
// {
//     allocator.dealloc(static_cast<T*>(ptr));
// }
// template<typename T>
// constexpr void operator delete(void* ptr, cm::Arena<T>* allocator)
// {
//     allocator->dealloc(static_cast<T*>(ptr));
// }
// template<typename T>
// constexpr void operator delete[](void* ptr, cm::Arena<T>& allocator)
// {
//     allocator.dealloc(static_cast<T*>(ptr));
// }
// template<typename T>
// constexpr void operator delete[](void* ptr, cm::Arena<T>* allocator)
// {
//     allocator->dealloc(static_cast<T*>(ptr));
// }
