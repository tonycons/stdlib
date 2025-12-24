/*
   Copyright 2025 Anthony A. Constantinescu.

   Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
   in compliance with the License. You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software distributed under the License
   is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
   or implied. See the License for the specific language governing permissions and limitations under
   the License.


    File: win32lite.hh
    Description: A lightweight alternative to the Windows.h header that only defines what is needed for this project.
    All type and function declarations are taken from the Microsoft documentation
    The NTAPI undocumented functions are sourced from http://undocumented.ntinternals.net/
*/

#pragma once

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define nominmax
#define NOMINMAX
#include "../core.hh"


extern "C" {

using BOOL = u32;  // Yes, this is correct
using DWORD = u32;
using size_t = usize;
using HANDLE = void*;
#define INVALID_HANDLE_VALUE (reinterpret_cast<void*>(-1LL))


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      KERNEL32 FUNCTIONS
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///
/// https://learn.microsoft.com/en-us/windows/win32/api/winnt/ns-winnt-memory_basic_information
///
struct MEMORY_BASIC_INFORMATION
{
    void* BaseAddress;
    void* AllocationBase;
    u32 AllocationProtect;
    u16 PartitionId;
    usize RegionSize;
    u32 State;
    u32 Protect;
    u32 Type;
};
using PMEMORY_BASIC_INFORMATION = MEMORY_BASIC_INFORMATION*;


enum : u32 {
    // https://learn.microsoft.com/en-us/windows/win32/memory/memory-protection-constants
    PAGE_EXECUTE = 0x10,
    PAGE_EXECUTE_READ = 0x20,
    PAGE_EXECUTE_READWRITE = 0x40,
    PAGE_EXECUTE_WRITECOPY = 0x80,
    PAGE_NOACCESS = 0x01,
    PAGE_READONLY = 0x02,
    PAGE_READWRITE = 0x04,
    PAGE_WRITECOPY = 0x08,
    PAGE_TARGETS_INVALID = 0x40000000,
    PAGE_TARGETS_NO_UPDATE = 0x40000000,
    PAGE_GUARD = 0x100,
    PAGE_NOCACHE = 0x200,
    PAGE_WRITECOMBINE = 0x400,

    // https://learn.microsoft.com/en-us/windows/win32/api/heapapi/nf-heapapi-heapalloc
    HEAP_ZERO_MEMORY = 0x00000008,
    HEAP_NO_SERIALIZE = 0x00000001,
    HEAP_GENERATE_EXCEPTIONS = 0x00000004,
};


[[gnu::dllimport, gnu::stdcall]]
HANDLE GetStdHandle(DWORD);

[[gnu::dllimport, gnu::stdcall]]
BOOL WriteFile(HANDLE hFile, void const* lpBuffer, DWORD nBytesToWrite, DWORD* lpBytesWritten, void* lpOverlapped);

[[gnu::dllimport, gnu::stdcall]]
void ExitProcess(int);

[[gnu::dllimport, gnu::stdcall]]
char* GetCommandLineA();

[[gnu::dllimport, gnu::stdcall]]
HANDLE GetProcessHeap();

[[gnu::dllimport, gnu::stdcall]]
void* HeapAlloc(HANDLE hHeap, DWORD dwFlags, size_t dwBytes);

[[gnu::dllimport, gnu::stdcall]]
BOOL HeapFree(HANDLE hHeap, DWORD dwFlags, void* lpMem);

[[gnu::dllimport, gnu::stdcall]]
void* HeapReAlloc(HANDLE hHeap, DWORD dwFlags, void* lpMem, size_t dwBytes);

///
/// https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualquery
///
[[gnu::dllimport, gnu::stdcall]]
usize VirtualQuery(void const* lpAddress, PMEMORY_BASIC_INFORMATION lpBuffer, usize dwLength);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      UNDOCUMENTED NT FUNCTIONS
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


[[gnu::dllimport, gnu::stdcall]]
void* RtlAllocateHeap(void* HeapHandle, unsigned long Flags, size_t Size);

///
/// NtTerminateProcess
/// \param pHandle If not specified, caller process is killed.
///
[[gnu::dllimport, gnu::stdcall]]
void NtTerminateProcess(void* pHandle, int exit_status);


///
/// Returns the number of 100-nanosecond intervals since January 1, 1601 (UTC).
///
[[gnu::dllimport, gnu::stdcall]]
int NtQuerySystemTime(long long*);

///
/// \param Alertable If set, execution can break in a result of NtAlertThread call.
/// \param pDelayInterval Delay in 100-ns units. Negative value means delay relative to current.
///
[[gnu::dllimport, gnu::stdcall]]
void NtDelayExecution(bool Alertable, long long* pDelayInterval);
}

#endif
