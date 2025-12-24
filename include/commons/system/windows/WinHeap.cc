#include "WindowsNT.hh"  // IWYU pragma: keep
#include "Win32Error.hh"
#include "../core.hh"
#include <commons/system/Profiler.hh>


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wlanguage-extension-token"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage-in-libc-call"


// Address sanitizer injects its own memory allocation functions even with a freestanding build
#if !__has_feature(address_sanitizer)

extern "C" void* malloc(__SIZE_TYPE__ size)
{
    void* mem;

    __try {
        mem = HeapAlloc(GetProcessHeap(), HEAP_GENERATE_EXCEPTIONS, size);
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        return nullptr;
    }
    return mem;
}

extern "C" void free(void* mem) { ::operator delete(mem); }

//
///----------------------------------------------------------------------------------------------------------------------
/// edit 5/8/2025
///
//
void* operator new(__SIZE_TYPE__ size)
{
    void* mem;

    __try {
        mem = HeapAlloc(GetProcessHeap(), HEAP_GENERATE_EXCEPTIONS, size);
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        DWORD _;
        if (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION) {
            WriteFile(GetStdHandle(STD_ERROR_HANDLE), "Heap corruption\n", 15, &_, nullptr);
        } else if (GetExceptionCode() == STATUS_NO_MEMORY) {
            WriteFile(GetStdHandle(STD_ERROR_HANDLE), "Out of memory\n", 14, &_, nullptr);
        } else {
            __builtin_trap();
        }

#if defined(__cpp_exceptions) && __cpp_exceptions
        throw 0xdeadbeef;
#else
        // argc = size;
        WriteFile(GetStdHandle(STD_ERROR_HANDLE), "Memory allocation failed!\n", 27, &_, nullptr);
        __builtin_trap();
#endif
    }
    return mem;
}

void* operator new[](__SIZE_TYPE__ size) { return ::operator new(size); }

//
///----------------------------------------------------------------------------------------------------------------------
/// edit 2024
///
//
void operator delete(void* ptr)
{
    DWORD _;
    HANDLE heap;
    char const* msg;

    if (!ptr) {
        msg = "deleting null pointer\n";
        goto errorCondition;
    }

    heap = GetProcessHeap();
    if (heap == nullptr) {
        msg = "GetProcessHeap() failed\n";
        goto errorCondition;
    }

    if (HeapValidate(heap, 0, ptr) == 0) {
        msg = "deleting invalid pointer\n";
        goto errorCondition;
    }

    if (HeapFree(heap, 0, ptr) == 0) {
        msg = getErrorEnumName(GetLastError());
        goto errorCondition;
    }
    return;

errorCondition:

#if defined(__cpp_exceptions) && __cpp_exceptions
    throw 0xdedede;
#else
    cm::Assert(false, msg);
#endif
}

void operator delete[](void* ptr) { ::operator delete(ptr); }

void operator delete(void* ptr, __SIZE_TYPE__ sz)
{
    (void)sz;
    ::operator delete(ptr);
}

void operator delete[](void* ptr, __SIZE_TYPE__ sz)
{
    (void)sz;
    ::operator delete(ptr);
}

#endif
