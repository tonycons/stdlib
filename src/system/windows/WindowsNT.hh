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
#define WIN32_LEAN_AND_MEAN
#define nominmax
#define NOMINMAX
#include <Windows.h>
#include <commons/core.hh>


#define ATLAS_USE_WINDOWS_NT 0

// using BOOL = __UINT32_TYPE__;
// using DWORD = __UINT32_TYPE__;
// using size_t = __SIZE_TYPE__;
// using HANDLE = void*;
// #define INVALID_HANDLE_VALUE reinterpret_cast<void*>(-1LL)


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      KERNEL32 FUNCTIONS
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// typedef struct {
//     void* BaseAddress;
//     void* AllocationBase;
//     DWORD AllocationProtect;
//     short PartitionId;
//     size_t RegionSize;
//     DWORD State;
//     DWORD Protect;
//     DWORD Type;
// } MEMORY_BASIC_INFORMATION, *PMEMORY_BASIC_INFORMATION;

// #define HEAP_ZERO_MEMORY 0x00000008
// #define HEAP_NO_SERIALIZE 0x00000001
// #define HEAP_GENERATE_EXCEPTIONS 0x00000004

// extern "C" [[gnu::dllimport, gnu::stdcall]]
// HANDLE GetStdHandle(DWORD);

// extern "C" [[gnu::dllimport, gnu::stdcall]]
// BOOL WriteFile(HANDLE hFile, void const* lpBuffer, DWORD nBytesToWrite, DWORD* lpBytesWritten, void* lpOverlapped);

// extern "C" [[gnu::dllimport, gnu::stdcall]]
// void ExitProcess(int);

// extern "C" [[gnu::dllimport, gnu::stdcall]]
// char* GetCommandLineA();

// extern "C" [[gnu::dllimport, gnu::stdcall]]
// HANDLE GetProcessHeap();

// extern "C" [[gnu::dllimport, gnu::stdcall]]
// void* HeapAlloc(HANDLE hHeap, DWORD dwFlags, size_t dwBytes);

// extern "C" [[gnu::dllimport, gnu::stdcall]]
// BOOL HeapFree(HANDLE hHeap, DWORD dwFlags, void* lpMem);

// extern "C" [[gnu::dllimport, gnu::stdcall]]
// void* HeapReAlloc(HANDLE hHeap, DWORD dwFlags, void* lpMem, size_t dwBytes);

// extern "C" [[gnu::dllimport, gnu::stdcall]]
// size_t VirtualQuery(void const* lpAddress, PMEMORY_BASIC_INFORMATION lpBuffer, size_t dwLength);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      UNDOCUMENTED NT FUNCTIONS
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// extern "C" [[gnu::dllimport, gnu::stdcall]]
// void* RtlAllocateHeap(void* HeapHandle, unsigned long Flags, size_t Size);
//
/////
///// NtTerminateProcess
///// \param pHandle If not specified, caller process is killed.
/////
// extern "C" [[gnu::dllimport, gnu::stdcall]]
// void NtTerminateProcess(void* pHandle, int exit_status);
//
//
/////
///// Returns the number of 100-nanosecond intervals since January 1, 1601 (UTC).
/////
// extern "C" [[gnu::dllimport, gnu::stdcall]]
// int NtQuerySystemTime(long long*);
//
/////
///// \param Alertable If set, execution can break in a result of NtAlertThread call.
///// \param pDelayInterval Delay in 100-ns units. Negative value means delay relative to current.
/////
// extern "C" [[gnu::dllimport, gnu::stdcall]]
// void NtDelayExecution(bool Alertable, long long* pDelayInterval);
