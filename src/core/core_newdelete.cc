#include <commons/core.hh>
// #include <commons/gc/GC.hh>
#include <commons/core.hh>
#include <stdlib.h>
using namespace cm;

namespace std {
struct nothrow_t
{};
}

constexpr std::align_val_t DEFAULT_ALIGNMENT = std::align_val_t(8);


void* newImpl(std::size_t size, std::align_val_t alignment) noexcept
{
    void* ptr;
    if (alignment == DEFAULT_ALIGNMENT) {
        ptr = malloc(size);
    } else {
        ptr = aligned_alloc(static_cast<size_t>(alignment), size);
    }
    Assert(ptr, ASMS_NO_MEMORY);
    return ptr;
}

void* newImplNothrow(std::size_t size, std::align_val_t alignment) noexcept
{
    return aligned_alloc(static_cast<size_t>(alignment), size);
    // return GC_alloc(size, size_t(alignment));
}

void deleteImpl(void* ptr)
{
    // GC_free(ptr);
    free(ptr);
    (void)ptr;
}


///
///
///

void* operator new(std::size_t size) { return newImpl(size, DEFAULT_ALIGNMENT); }

void* operator new[](std::size_t size) { return newImpl(size, DEFAULT_ALIGNMENT); }

void* operator new(std::size_t size, std::align_val_t al) { return newImpl(size, al); }

void* operator new[](std::size_t size, std::align_val_t al) { return newImpl(size, al); }


///
///
///
///

void* operator new(std::size_t size, std::nothrow_t const&) noexcept { return newImplNothrow(size, DEFAULT_ALIGNMENT); }

void* operator new[](std::size_t size, std::nothrow_t const&) noexcept
{
    return newImplNothrow(size, DEFAULT_ALIGNMENT);
}

void* operator new(std::size_t size, std::align_val_t al, std::nothrow_t const&) noexcept
{
    return newImplNothrow(size, al);
}

void* operator new[](std::size_t size, std::align_val_t al, std::nothrow_t const&) noexcept
{
    return newImplNothrow(size, al);
}

///
///
///
///

void operator delete(void* ptr) noexcept { return deleteImpl(ptr); }

void operator delete[](void* ptr) noexcept { return deleteImpl(ptr); }

void operator delete(void* ptr, __SIZE_TYPE__) noexcept { return deleteImpl(ptr); }

void operator delete[](void* ptr, __SIZE_TYPE__) noexcept { return deleteImpl(ptr); }

extern "C" [[noreturn]]
void __cxa_pure_virtual()
{
    __builtin_trap();
}
