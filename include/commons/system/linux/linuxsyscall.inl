/*
   Copyright 2025 Anthony A. Constantinescu.

   Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
   in compliance with the License. You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software distributed under the License
   is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
   or implied. See the License for the specific language governing permissions and limitations under
   the License.

   File: commons/system/syscall_linux_x64_numbers.inl
   Description: Functionality for making direct syscalls on Linux
*/

#pragma once
#ifdef __inline_sys_header__


struct
{
#define __inline_system_header__
#if __x86_64__
#include "syscall_linux_x64.inl"
#else
#error "Unimplemented"
#endif
#undef __inline_system_header__

    [[maybe_unused]] FORCEINLINE static u64 operator()(int num)
    {  //
        return Syscall(num);
    }
    [[maybe_unused]] FORCEINLINE static u64 operator()(int num, u64 p1)
    {  //
        return Syscall(num, p1);
    }
    [[maybe_unused]] FORCEINLINE static u64 operator()(int num, u64 p1, u64 p2)
    {  //
        return Syscall(num, p1, p2);
    }
    [[maybe_unused]] FORCEINLINE static u64 operator()(int num, u64 p1, u64 p2, u64 p3)
    {
        return Syscall(num, p1, p2, p3);
    }
    [[maybe_unused]] FORCEINLINE static u64 operator()(int num, u64 p1, u64 p2, u64 p3, u64 p4)
    {
        return Syscall(num, p1, p2, p3, p4);
    }
    [[maybe_unused]] FORCEINLINE static u64 operator()(int num, u64 p1, u64 p2, u64 p3, u64 p4, u64 p5)
    {
        return Syscall(num, p1, p2, p3, p4, p5);
    }
    [[maybe_unused]] FORCEINLINE static u64 operator()(int num, u64 p1, u64 p2, u64 p3, u64 p4, u64 p5, u64 p6)
    {
        return Syscall(num, p1, p2, p3, p4, p5, p6);
    }
} inline constexpr LinuxSyscall;


namespace impl {

// disables register keyword deprecation warnings
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wregister"


// From
// https://chromium.googlesource.com/native_client/src/native_client/+/refs/heads/main/src/nonsfi/linux/linux_syscall_wrappers.h

#if defined(__i386__)
/*
 * Registers used for system call parameters on x86-32:
 *   %eax - system call number and return value
 *   %ebx - argument 1, preserved
 *   %ecx - argument 2, preserved
 *   %edx - argument 3, preserved
 *   %esi - argument 4, preserved
 *   %edi - argument 5, preserved
 *   %ebp - argument 6, preserved
 */
inline static u32 syscall0(int num)
{
    u32 result;
    __asm__ __volatile__("int $0x80\n" : "=a"(result) : "a"(num));
    return result;
}
inline static u32 syscall1(int num, u32 arg1)
{
    u32 result;
    __asm__ __volatile__("int $0x80\n" : "=a"(result) : "a"(num), "b"(arg1));
    return result;
}
inline static u32 syscall2(int num, u32 arg1, u32 arg2)
{
    u32 result;
    __asm__ __volatile__("int $0x80\n" : "=a"(result) : "a"(num), "b"(arg1), "c"(arg2));
    return result;
}
inline static u32 syscall3(int num, u32 arg1, u32 arg2, u32 arg3)
{
    u32 result;
    __asm__ __volatile__("int $0x80\n" : "=a"(result) : "a"(num), "b"(arg1), "c"(arg2), "d"(arg3));
    return result;
}
inline static u32 syscall4(int num, u32 arg1, u32 arg2, u32 arg3, u32 arg4)
{
    u32 result;
    __asm__ __volatile__("int $0x80\n" : "=a"(result) : "a"(num), "b"(arg1), "c"(arg2), "d"(arg3), "S"(arg4));
    return result;
}
inline static u32 syscall5(int num, u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5)
{
    u32 result;
    __asm__ __volatile__("int $0x80\n"
                         : "=a"(result)
                         : "a"(num), "b"(arg1), "c"(arg2), "d"(arg3), "S"(arg4), "D"(arg5));
    return result;
}
inline static u32 syscall6(int num, u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6)
{
    /*
     * Inline assembly doesn't let us use a constraint to set %ebp, which is
     * the 6th syscall argument on x86-32.  To set %ebp, we use the trick of
     * saving some registers on the stack.
     *
     * Note, however, that this means unwind info will be incorrect during
     * this syscall.
     */
    u32 args[2] = {arg1, arg6};
    u32 result;
    __asm__ __volatile__("push %%ebp\n"
                         "movl 4(%%ebx), %%ebp\n" /* arg6 */
                         "movl 0(%%ebx), %%ebx\n" /* arg1 */
                         "int $0x80\n"
                         "pop %%ebp\n"
                         : "=a"(result)
                         : "a"(num), "b"(&args), "c"(arg2), "d"(arg3), "S"(arg4), "D"(arg5)
                         : "ebx", "memory");
    return result;
}

#elif defined(__arm__)
/*
 * Registers used for system call parameters on ARM EABI:
 *   r7 - system call number
 *   r0 - argument 1 and return value
 *   r1 - argument 2, preserved
 *   r2 - argument 3, preserved
 *   r3 - argument 4, preserved
 *   r4 - argument 5, preserved
 *   r5 - argument 6, preserved
 */
inline static u32 syscall0(int num)
{
    register u32 sysno __asm__("r7") = num;
    register u32 result __asm__("r0");
    __asm__ __volatile__("svc #0\n" : "=r"(result) : "r"(sysno) : "memory");
    return result;
}
inline static u32 syscall1(int num, u32 arg1)
{
    register u32 sysno __asm__("r7") = num;
    register u32 a1 __asm__("r0") = arg1;
    register u32 result __asm__("r0");
    __asm__ __volatile__("svc #0\n" : "=r"(result) : "r"(sysno), "r"(a1) : "memory");
    return result;
}
inline static u32 syscall2(int num, u32 arg1, u32 arg2)
{
    register u32 sysno __asm__("r7") = num;
    register u32 a1 __asm__("r0") = arg1;
    register u32 a2 __asm__("r1") = arg2;
    register u32 result __asm__("r0");
    __asm__ __volatile__("svc #0\n" : "=r"(result) : "r"(sysno), "r"(a1), "r"(a2) : "memory");
    return result;
}
inline static u32 syscall3(int num, u32 arg1, u32 arg2, u32 arg3)
{
    register u32 sysno __asm__("r7") = num;
    register u32 a1 __asm__("r0") = arg1;
    register u32 a2 __asm__("r1") = arg2;
    register u32 a3 __asm__("r2") = arg3;
    register u32 result __asm__("r0");
    __asm__ __volatile__("svc #0\n" : "=r"(result) : "r"(sysno), "r"(a1), "r"(a2), "r"(a3) : "memory");
    return result;
}
inline static u32 syscall4(int num, u32 arg1, u32 arg2, u32 arg3, u32 arg4)
{
    register u32 sysno __asm__("r7") = num;
    register u32 a1 __asm__("r0") = arg1;
    register u32 a2 __asm__("r1") = arg2;
    register u32 a3 __asm__("r2") = arg3;
    register u32 a4 __asm__("r3") = arg4;
    register u32 result __asm__("r0");
    __asm__ __volatile__("svc #0\n" : "=r"(result) : "r"(sysno), "r"(a1), "r"(a2), "r"(a3), "r"(a4) : "memory");
    return result;
}
inline static u32 syscall5(int num, u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5)
{
    register u32 sysno __asm__("r7") = num;
    register u32 a1 __asm__("r0") = arg1;
    register u32 a2 __asm__("r1") = arg2;
    register u32 a3 __asm__("r2") = arg3;
    register u32 a4 __asm__("r3") = arg4;
    register u32 a5 __asm__("r4") = arg5;
    register u32 result __asm__("r0");
    __asm__ __volatile__("svc #0\n"
                         : "=r"(result)
                         : "r"(sysno), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5)
                         : "memory");
    return result;
}
inline static u32 syscall6(int num, u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6)
{
    register u32 sysno __asm__("r7") = num;
    register u32 a1 __asm__("r0") = arg1;
    register u32 a2 __asm__("r1") = arg2;
    register u32 a3 __asm__("r2") = arg3;
    register u32 a4 __asm__("r3") = arg4;
    register u32 a5 __asm__("r4") = arg5;
    register u32 a6 __asm__("r5") = arg6;
    register u32 result __asm__("r0");
    __asm__ __volatile__("svc #0\n"
                         : "=r"(result)
                         : "r"(sysno), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5), "r"(a6)
                         : "memory");
    return result;
}

#endif


static inline int linux_is_error_result(u32 result)
{
    /*
     * -0x1000 is the highest address that mmap() can return as a result.
     * Linux errno values are less than 0x1000.
     */
    return result > u32(-0x1000);
}


}  // namespace impl
#endif
