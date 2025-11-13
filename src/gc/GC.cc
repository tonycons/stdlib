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

#include <Commons/gc/GC.hh>
#include <Commons/core.hh>
#include <Commons/system/Profiler.hh>
#include <Commons/core.hh>
#include <sanitizer/dfsan_interface.h>
#include <pthread.h>
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"

static u8* gcFirstGenBuffer = {};
static size_t gcFirstGenSize = {};
static size_t gcFirstGenCap = {};
constexpr inline dfsan_label DF_MANAGED_POINTER = 1;


// #include <setjmp.h>
// #include <sanitizer/asan_interface.h>

//
//
///
///
///
///
//
//
//

/**
 * Uses the functionality of DFSAN to keep track of what is a pointer to objects allocated by the GC.
 * One obstacle of implementing a GC for C++ is that there is no way to distinguish between pointers and ordinary
 * integer values in memory, since both are represented as sequences of bits.
 * The DFSAN labels solve this issue by enabling any pointer obtained from GC_alloc to carry a "taint" that causes the
 * following effects:
 * Any pointer set equal to this pointer will be "tainted"
 * Any integer set equal to this pointer will be "tainted"
 * Any such "tainted" value is then considered an object reference.
 * However, any integer that happens to have the same bits as the pointer, but did not logically interact with the
 * pointer in any way, will not be considered as an object reference.
 */
void* makeTaggedPointer(void* ptr) noexcept
{
#if __has_feature(dataflow_sanitizer)
    // dfsan_set_label(DF_MANAGED_POINTER, &ptr, sizeof(ptr));
#else
#warning "No tagged pointers without dfsan"
#endif
    return ptr;
}

/**
 * Scans the specified memory region for managed pointers. It detects any bytes that are labeled as managed pointers,
 * and if so, interprets that as a valid reference to some object allocated by the GC.
 */
void scanForPointers(void const* mem, long sizeBytes, void (*callback)(void const*))
{
    auto* UNSAFE(bytes) = static_cast<u8 const*>(mem);
    if (sizeBytes <= 0) {
        return;
    } else {
#if __has_feature(dataflow_sanitizer)
        dfsan_label const label = 0;  // dfsan_read_label(bytes, 1);
#else
#warning "No tagged pointers without dfsan"
        dfsan_label const label = 0;
#endif
        if (label) {
            callback(bytes);
            [[clang::musttail]] return scanForPointers(
                bytes + sizeof(void*), sizeBytes - long(sizeof(void*)), callback);
        } else {
            [[clang::musttail]] return scanForPointers(bytes + 1, sizeBytes - 1, callback);
        }
    }
}

/**
 * Allocates some data from the first generation mempry pool
 */
void* gcAllocFromFirstGen(size_t size, size_t alignment)
{
    auto const baseAddr = &gcFirstGenBuffer[gcFirstGenSize];
    auto const remainder = size_t(baseAddr) % alignment;
    auto const retAddr = baseAddr + remainder;
    auto const sizePlusAlignment = size + remainder;

    if (retAddr + sizePlusAlignment > gcFirstGenBuffer + gcFirstGenCap) {
        return nullptr;
    } else {
        //__asan_unpoison_memory_region(baseAddr, sizePlusAlignment);
        gcFirstGenSize += sizePlusAlignment;
        return retAddr;
    }
}

/**
 * Resets the first generation memory pool
 */
void gcResetFirstGenPool() { gcFirstGenSize = 0; }

/**
 *
 */
void getStackStartAndSize(void** stackaddr, size_t* stacksize)
{
    pthread_attr_t attr;
    pthread_getattr_np(pthread_self(), &attr);
    pthread_attr_getstack(&attr, stackaddr, stacksize);
}

/**
 *
 */
void gcFirstGenPass()
{
    void* stackaddr;
    size_t stacksize;
    getStackStartAndSize(&stackaddr, &stacksize);

    scanForPointers(
        stackaddr, long(stacksize), [](void const* ptr) -> void { __builtin_printf("Found pointer %p\n", ptr); });
}


/**
 * Called to initialize the garbage collector.
 */
extern "C" void ::GC_init(size_t gen0_buffer_size)
{
    gcFirstGenCap = gen0_buffer_size;
    gcFirstGenBuffer = static_cast<u8*>(__builtin_malloc(gcFirstGenCap));
    gcResetFirstGenPool();
}

/**
 * Called when the program exits, frees all memory arenas
 */
__attribute__((destructor)) extern "C"  //
    void
    GC_deinit()
{
    if (gcFirstGenBuffer) {
        __builtin_free(gcFirstGenBuffer);
    }
}

extern "C" void* ::GC_alloc(size_t size, size_t alignment)
{
    void* ptr = gcAllocFromFirstGen(size, alignment);
    Assert(ptr, ASMS_NO_MEMORY);
    __builtin_printf("\t\t\t\t\t\t\tAllocation %p\n", ptr);
    return ptr;
}

extern "C" void ::GC_collect()
{
    // gcFirstGenPass();
    gcResetFirstGenPool();
}
