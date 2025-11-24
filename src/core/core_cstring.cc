/*
   Copyright 2024 Anthony A. Constantinescu.

   Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
   in compliance with the License. You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software distributed under the License
   is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
   or implied. See the License for the specific language governing permissions and limitations under
   the License.
*/

// implementation of C string functions necessary for compilation

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
#pragma GCC diagnostic ignored "-Wmissing-prototypes"
#pragma GCC diagnostic ignored "-Wreserved-identifier"

#include <commons/core.hh>


using Uint = unsigned int;


#if !__STDC_HOSTED__

extern "C" {


//
//
//
//
void* memset(void* dst, int c, __SIZE_TYPE__ n)
{
    constexpr auto alignment = alignof(long long);
    auto* p = reinterpret_cast<__Uint8_TYPE__*>(dst);
    auto* end = p + n;

    for (; (reinterpret_cast<__SIZE_TYPE__>(p) & (alignment - 1)) != 0 && p < end; p++)
        *p = static_cast<__Uint8_TYPE__>(c);

    for (; p < end; p += alignment)
        __builtin_memset_inline(p, c, alignment);

    return dst;
}

//
//
//
//
void* memmove(void* pdst, void const* psrc, __SIZE_TYPE__ n)
{
    auto dst = reinterpret_cast<__Uint8_TYPE__*>(pdst);
    auto src = reinterpret_cast<__Uint8_TYPE__ const*>(psrc);
    // TODO: Optimize

    if (src < dst)
        for (dst += n, src += n; n--;)
            *--dst = *--src;
    else
        while (n--)
            *dst++ = *src++;
    return pdst;
}

//
//
//
//
int strcmp(char const* s1, char const* s2)
{
    for (; (*s1 != '\0') && (*s1 == *s2); s1++, s2++)
        ;
    return *s1 - *s2;
}

//
//
//
//
__SIZE_TYPE__ strlen(char const* s)
{
    __SIZE_TYPE__ len = 0;
    while (*s++ != '\0')
        len++;

    return len;
    // // Runtime version: found at https://www.strchr.com/optimized_strlen_function
    // // Added improvements to original function based on a reader's comment.
    // __SIZE_TYPE__ len = 0;

    // // If string is not aligned, check the first few bytes until pointer is aligned.
    // for (; ((__SIZE_TYPE__)s & 3) != 0 && *s++ != '\0'; len++)
    //     ;

    // // Scan 4 bytes at once:
    // // If we didn't align the pointer previously, a crash could occur on some platforms.
    // for (;;) {
    //     __Uint32_TYPE__ x = *(__Uint32_TYPE__*)s;
    //     if ((x & 0xFF) == 0)
    //         return len;
    //     if ((x & 0xFF00) == 0)
    //         return len + 1;
    //     if ((x & 0xFF0000) == 0)
    //         return len + 2;
    //     if ((x & 0xFF000000) == 0)
    //         return len + 3;
    //     s += 4, len += 4;
    // }
}


/*
 implementation of runtime memcpy
 Takes advantage that any number can be expressed as a sum of powers of 2, where the powers of 2 do
 not repeat unless there is an upper bound on the powers of 2.
 This means that a copy of 44 bytes (352 bits) is

    32 bit copy + 64 bit copy + 256 bit copy.

 Rather than copying 44 bytes, one at a time, this operation can be reduced to 3 copies.
 The upper bound on the number of bits copied at a time depends on the processor.
 Modern x64 CPUs can copy up to 512 bits at once with a single instruction.
 A larger copy of 45369 bytes (362,952 bits) is then

    64 bit copy + 512 bit copy + 512 bit copy + ...

 Use of clang's __builtin_memcpy_inline function generates an N-byte copy instruction.
 According to the docs, it is meant to be a "building block" for memcpy implementations.
 If the CPU cannot copy that many bytes in a single instruction,
 then it simply generates multiple of the largest copy instruction.
 (unless the number is much, much higher, then it will break)
 But up to 64 bytes it works fine on x86.

 While standard libc isn't using the naive approach,
 this runs about 6.8 times faster than libc memcpy under Compiler Explorer.
*/

using u8 = __Uint8_TYPE__;
using usize = __SIZE_TYPE__;

__attribute__((always_inline)) static void* PtrAdd(void const* ptr, usize offset)
{
    return reinterpret_cast<u8*>(const_cast<void*>(ptr)) + offset;
}

/*
 -
 -
*/
static void MemcpyU512(void* dst, void const* src, usize length)
{
    while (length != 0) {
        __builtin_memcpy_inline(dst, src, 64);
        dst = PtrAdd(dst, 64);
        src = PtrAdd(src, 64);
        length--;
    }
}

static void MemcpyU256(void* dst, void const* src, usize length)
{
    if (length % 2 != 0) {
        __builtin_memcpy_inline(dst, src, 32);
        dst = PtrAdd(dst, 32);
        src = PtrAdd(src, 32);
        length--;
    }
    MemcpyU512(dst, src, length / 2);
}

static void MemcpyU128(void* dst, void const* src, usize length)
{
    if (length % 2 != 0) {
        __builtin_memcpy_inline(dst, src, 16);
        dst = PtrAdd(dst, 16);
        src = PtrAdd(src, 16);
        length--;
    }
    MemcpyU256(dst, src, length / 2);
}

static void MemcpyU64(void* dst, void const* src, usize length)
{
    if (length % 2 != 0) {
        __builtin_memcpy_inline(dst, src, 8);
        dst = PtrAdd(dst, 8);
        src = PtrAdd(src, 8);
        length--;
    }
    MemcpyU128(dst, src, length / 2);
}

static void MemcpyU32(void* dst, void const* src, usize length)
{
    if (length % 2 != 0) {
        __builtin_memcpy_inline(dst, src, 4);
        dst = PtrAdd(dst, 4);
        src = PtrAdd(src, 4);
        length--;
    }
    MemcpyU64(dst, src, length / 2);
}

static void MemcpyU16(void* dst, void const* src, usize length)
{
    if (length % 2 != 0) {
        __builtin_memcpy_inline(dst, src, 2);
        dst = PtrAdd(dst, 2);
        src = PtrAdd(src, 2);
        length--;
    }
    MemcpyU32(dst, src, length / 2);
}

static void MemcpyU8(void* dst, void const* src, usize length)
{
    if (length % 2 != 0) {
        __builtin_memcpy_inline(dst, src, 1);
        dst = PtrAdd(dst, 1);
        src = PtrAdd(src, 1);
        length--;
    }
    MemcpyU16(dst, src, length / 2);
}

void* memcpy(void* dst, void const* src, usize length)
{
    MemcpyU8(dst, src, length);
    return dst;
}


//
//
//
//
void* memchr(void const* s, int c, __SIZE_TYPE__ n)
{
    auto p = u8 const * (s);

    while ((n--) != 0)
        if (*p != U8(c))
            p++;
        else
            return void * (p);
    return nullptr;
}

//
//
//
//
int memcmp(void const* s1, void const* s2, usize n)
{
    auto p1 = u8 const * (s1);
    auto p2 = u8 const * (s2);

    while (n-- > 0) {
        int r = int(Uint(*p1)) - int(Uint(*p2));
        if (r)
            return r;
        p1++;
        p2++;
    }
    return 0;
}

//
//
//
//

char* strcat(char* __restrict__ dest, char const* __restrict__ src)
{
    char* ret = dest;
    while (*dest) {
        dest++;
    }
    while (true) {
        if (!(*dest++ = *src++)) {
            return ret;
        }
    }
}

//
//
//
//
char* strncat(char* __restrict__ dest, char const* __restrict__ src, __SIZE_TYPE__ n)
{
    char* ret = dest;
    while (*dest) {
        dest++;
    }
    while (n--) {
        if (!(*dest++ = *src++)) {
            return ret;
        }
    }
    *dest = 0;
    return ret;
}

//
//
//
//
char* strcpy(char* __restrict__ dst, char const* __restrict__ src)
{
    char* tmp = dst;
    while ((*dst++ = *src++) != '\0')
        ;
    return tmp;
}

//
//
//
//
char* strncpy(char* __restrict__ dest, char const* __restrict__ src, __SIZE_TYPE__ n)
{
    char* ret = dest;
    do {
        if (!n--)
            return ret;
    } while ((*dest++ = *src++));
    while (n--)
        *dest++ = 0;
    return ret;
}


// //
// //
// //
// //
usize wcslen(wchar_t const* start)
{
    // NB: start is not checked for nullptr!
    wchar_t const* end = start;
    while (*end != L'\0')
        ++end;

    [[assume(end > start)]];
    return usize(end - start);
}

usize wcsnlen(wchar_t const* start, usize n)
{

    wchar_t const* end = start;

    while (*end != L'\0' && n != 0) {
        ++end;
        n--;
    }

    [[assume(end > start)]];
    return usize(end - start);
}

//
//
//
//
__SIZE_TYPE__ strnlen(char const* s, __SIZE_TYPE__ len)
{
    __SIZE_TYPE__ i = 0;
    for (; i < len && s[i] != '\0'; ++i)
        ;
    return i;
}


//
//
//
//
int strncmp(char const* s1, char const* s2, __SIZE_TYPE__ n)
{
    while (n--)
        if (*s1++ != *s2++)
            return *u8 const * (s1 - 1) - *u8 const * (s2 - 1);
    return 0;
}

char* strchr(char const* s, int c)
{
    do {
        if (*s == c) {
            return const_cast<char*>(s);
        }
    } while (*s++);
    return nullptr;
}

usize strcspn(char const* s1, char const* s2)
{
    char const* s = s1;
    char const* c;

    while (*s1) {
        for (c = s2; *c; c++) {
            if (*s1 == *c)
                break;
        }
        if (*c)
            break;
        s1++;
    }

    return usize(s1 - s);
}

char* _strdup(char const* s)
{
    usize len = strlen(s) + 1;
    void* copyS = __builtin_malloc(len);

    if (copyS == nullptr)
        return nullptr;

    return char * (__builtin_memcpy(copyS, s, len));
}

char* strpbrk(char const* s1, char const* s2)
{
    char const* c = s2;

    if (!*s1)
        return nullptr;

    while (*s1) {
        for (c = s2; *c; c++) {
            if (*s1 == *c)
                break;
        }
        if (*c)
            break;
        s1++;
    }

    if (*c == '\0')
        s1 = nullptr;

    return const_cast<char*>(s1);
}

usize strspn(char const* s1, char const* s2)
{
    char const* s = s1;
    char const* c;

    while (*s1) {
        for (c = s2; *c; c++) {
            if (*s1 == *c)
                break;
        }
        if (*c == '\0')
            break;
        s1++;
    }

    return usize(s1 - s);
}

///
/// https://github.com/apache/mynewt-core/blob/master/libc/baselibc/src/memmem.c
///
void* memmem(void const* haystack, size_t n, void const* needle, size_t m)
{
    auto y = u8 const * (haystack);
    auto x = u8 const * (needle);

    size_t j, k, l;

    if (m > n || !m || !n)
        return nullptr;

    if (1 != m) {
        if (x[0] == x[1]) {
            k = 2;
            l = 1;
        } else {
            k = 1;
            l = 2;
        }

        j = 0;
        while (j <= n - m) {
            if (x[1] != y[j + 1]) {
                j += k;
            } else {
                if (!memcmp(x + 2, y + j + 2, m - 2) && x[0] == y[j])
                    return void * (&y[j]);
                j += l;
            }
        }
    } else
        do {
            if (*y == *x)
                return void * (y);
            y++;
        } while (--n);

    return nullptr;
}

char* strstr(char const* haystack, char const* needle)
{
    return char * (memmem(haystack, strlen(haystack), needle, strlen(needle)));
}

char* strnstr(char const* haystack, char const* needle, size_t len)
{
    return char * (memmem(haystack, strlen(haystack), needle, len));
}

char* strsep(char** stringp, char const* delim)
{
    char* s = *stringp;
    char* e;

    if (!s)
        return nullptr;

    e = strpbrk(s, delim);
    if (e)
        *e++ = '\0';

    *stringp = e;
    return s;
}

char* strtok_r(char* s, char const* delim, char** holder)
{
    if (s)
        *holder = s;

    do {
        s = strsep(holder, delim);
    } while (s && !*s);

    return s;
}

char* strtok(char* s, char const* delim)
{
    static char* holder;

    return strtok_r(s, delim, &holder);
}

///
/// Note: implementation details:
/// Whitespace: Ignore any leading whitespace (" ")
/// Signedness: + or - signs are allowed before the first digit.
/// Conversion: Reads the integer by skipping leading zeros until a non-digit character is
/// encountered or the end of the string is reached. If no digits were read, then the result is 0.
/// If the integer is out of the 32-bit signed integer range [-231, 231 - 1], then round the integer
/// to remain in the range.
///
int atoi(char const* s)
{
    int result = 0;
    int negate = 0;

    while (*s == ' ' || *s == '\t')
        s++;
    if (*s == '+' && *(s + 1) != '-')
        s++;
    if (*s == '-') {
        negate = 1;
        s++;
    }
    for (; *s != '\0'; s++) {
        switch (*s) {
        case '0' ... '9':
            if (__builtin_mul_overflow(result, 10, &result) || __builtin_add_overflow(result, -('0' - *s), &result))
                return negate ? int_MIN : int_MAX;
            continue;
        default:
        }
        break;
    }
    return negate ? -result : result;
}

long atol(char const* s)
{
    long result = 0;
    int negate = 0;

    while (*s == ' ' || *s == '\t')
        s++;
    if (*s == '+' && *(s + 1) != '-')
        s++;
    if (*s == '-') {
        negate = 1;
        s++;
    }
    for (; *s != '\0'; s++) {
        switch (*s) {
        case '0' ... '9':
            if (__builtin_mul_overflow(result, 10, &result) || __builtin_add_overflow(result, -('0' - *s), &result))
                return negate ? LONG_MIN : LONG_MAX;
            continue;
        default:
        }
        break;
    }
    return negate ? -result : result;
}

long long atoll(char const* s)
{
    long long result = 0;
    int negate = 0;

    while (*s == ' ' || *s == '\t')
        s++;
    if (*s == '+' && *(s + 1) != '-')
        s++;
    if (*s == '-') {
        negate = 1;
        s++;
    }
    for (; *s != '\0'; s++) {
        switch (*s) {
        case '0' ... '9':
            if (__builtin_mul_overflow(result, 10, &result) || __builtin_add_overflow(result, -('0' - *s), &result))
                return negate ? LLONG_MIN : LLONG_MAX;
            continue;
        default:
        }
        break;
    }
    return negate ? -result : result;
}


// FIXME
[[noreturn]]
void longjmp()
{
    __builtin_trap();
}


// https://github.com/openmv/tensorflow-lib/blob/master/libm/frexp.c

double frexp(double x, int* e)
{
    union {
        double d;
        U64 i;
    } y = {x};
    int ee = y.i >> 52 & 0x7ff;

    if (!ee) {
        if (bool(x)) {
            x = frexp(x * 0x1p64, e);
            *e -= 64;
        } else
            *e = 0;
        return x;
    } else if (ee == 0x7ff) {
        return x;
    }

    *e = ee - 0x3fe;
    y.i &= 0x800fffffffffffffull;
    y.i |= 0x3fe0000000000000ull;
    return y.d;
}
}

#endif
