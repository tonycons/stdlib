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
#include <Commons/core.hh>


namespace cm {
extern "C" {

void GC_init(size_t gen0_buffer_size = 16_MB);
void* GC_alloc(size_t size, size_t alignment);
void GC_collect();
}
}

// namespace cm {

// using GCObjectFlags = int;


// class GCObject {
// private:
//     friend class GCInstance;
//     friend class GCAllocation;
//     GCObject() = default;
//     void* ptr;
//     GCObjectFlags flags;
//     size_t size;
//     size_t hash;
//     void (*dtor)(void*);
// };

// /**
//  * This class wraps a pointer to memory allocated by GC::alloc,GC::calloc,GC::realloc,etc.
//  * The design of this garbage collector requires that a pointer to the START of each memory
//  * allocation be reachable. Thus, doing arithmetic on the pointer value returned by GC::alloc will
//  * deem the memory unreachable. This class ensures a pointer to the START is maintained as long as
//  * the user keeps the variable in scope. See:
//  * https://github.com/orangeduck/tgc?tab=readme-ov-file#tgc-isnt-working-when-i-increment-pointers
//  * GC::alloc is not a plug-in replacement for malloc, it's not good to trick users into thinking
//  * that is true and have them deal with the complications.
//  */
// class GCAllocation {
// public:
//     GCAllocation(GCAllocation const&) = default;

// bool invalid() const;
// void* data();
// void const* data() const;
// size_t size() const;

// GCObjectFlags getFlags() const;
// void setFlags(GCObjectFlags flags);

// decltype(GCObject::dtor) getDestructor() const;
// void setDestructor(void (*dtor)(void*));

// private:
//     friend class GCInstance;
//     GCAllocation(void* ptr)
//         : ptr(ptr)
//     {}
//     void* ptr;
// };

// /**
//  * The garbage collector.
//  */
// class GCInstance {
// public:
//     consteval GCInstance() = default;

// /**
//  * Initializes the garbage collector.
//  */
// void init(int stackref = 0);

// /**
//  * Stop the garbage collector and free its internal memory.
//  */
// void stop();

// /**
//  * Run an iteration of the garbage collector, freeing any unreachable memory.
//  */
// void run();

// /**
//  * Pause or resume the garbage collector. While paused the garbage collector will not run during any allocations.
//  */
// void pause();

// /**
//  * Resume the garbage collector.
//  */
// void resume();

// /**
//  * Manually free an allocation made by the garbage collector. Runs any destructor if registered.
//  */
// void free(GCAllocation& allocation);

// /**
//  * Manually free an allocation made by the garbage collector. Runs any destructor if registered.
//  */
// void free(void* ptr);

// /**
//  * Allocate memory via the garbage collector without initializing it to zero.
//  */
// GCAllocation alloc(size_t size_bytes, int flags, void (*dtor)(void*));

// /**
//  * Allocate memory via the garbage collector and initalise it to zero.
//  */
// GCAllocation calloc(size_t num_blocks, size_t block_size, int flags, void (*dtor)(void*));

// /**
//  * Allocate memory via the garbage collector without initializing it to zero.
//  */
// inline auto alloc(size_t size_bytes) { return this->alloc(size_bytes, 0, nullptr); }

// /**
//  * Allocate memory via the garbage collector and initalise it to zero.
//  */
// inline auto calloc(size_t num_blocks, size_t block_size)
// {
//     return this->calloc(num_blocks, block_size, 0, nullptr);
// }

// /**
//  * Attemps to change the size of an allocated memory block. It may be slightly more performant than a free/alloc.
//  * @param allocation The previous allocation. Will be updated to point to the resized memory block.
//  * @param new_size new_size
//  */
// void realloc(GCAllocation& allocation, size_t new_size);

// private:
//     friend class GCAllocation;
//     void* bottom = nullptr;
//     bool paused = false;
//     size_t minptr = size_t(usize::MAX_VALUE);
//     size_t maxptr = {};
//     GCObject* items = nullptr;
//     GCObject* frees = nullptr;
//     double loadfactor = 0.9;
//     double sweepfactor = 0.5;
//     size_t nitems = {};
//     size_t nslots = {};
//     size_t mitems = {};
//     size_t nfrees = {};
//     bool trace = false;

// GCInstance(void* stack_address);
// static auto hash(void* ptr) -> size_t;

// auto probe(size_t i, size_t h) -> size_t;
// auto getObject(void* ptr) -> GCObject*;
// void addPtr(void* ptr, size_t size, int flags, void (*dtor)(void*));
// void* add(void* ptr, size_t size, int flags, void (*dtor)(void*));
// void removePtr(void* ptr);
// void remove(void* ptr);
// size_t calcIdealSize(size_t size);
// int rehash(size_t new_size);
// int expand();
// int shrink();
// void markPtr(void* ptr);
// void markStack();
// void mark();
// void sweep();
// void markTraced();
// void sweepTraced();
// };

// extern GCInstance GC;


// }  // namespace cm
