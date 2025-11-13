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
#include "GC.hh"


namespace cm {

/**
 * Provides customized allocation functions for objects managed by the garbage collector.
 * After allocating the object, it's still possible to delete the object as in plain old C++.
 * This reduces the work that has to be done by the garbage collector.
 * The garbage collector is there as a "safety net" in case it isn't deleted manually.
 *
 * Why didn't Java, for instance, let you delete objects manually?
 * Although that could solve "lag spikes" by allowing memory to be freed right away.
 * Probably they thought not because programmers would screw up and delete objects still in use.
 * So use delete sparingly.
 */
template<typename Derived>
class GCManaged {
public:
    virtual ~GCManaged() = default;

    inline static void* operator new(size_t size, void* ptr)
    {
        (void)size;
        return ptr;
    }

    inline static void* operator new(size_t size)
    {
        using PtrDerived = Derived const*;
        return GC.alloc(size, 0, [](void* ptr) { PtrDerived(ptr)->~Derived(); }).data();
    }

    inline static void operator delete(void* ptr) { GC.free(ptr); }
};

}  // namespace cm
