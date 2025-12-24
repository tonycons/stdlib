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


#pragma once
#ifndef __inline_core_header__
#warning Do not include this file directly; include "core.hh" instead
#else
UNSAFE_BEGIN;

extern "C" {

constexpr void* memset(void* dst, int c, usize n)
{
    constexpr auto alignment = alignof(long long);
    auto* p = reinterpret_cast<u8*>(dst);
    auto* end = p + n;

    for (; (reinterpret_cast<usize>(p) & (alignment - 1)) != 0 && p < end; p++) {
        *p = static_cast<u8>(c);
    }
    for (; p < end; p += alignment) {
        __builtin_memset_inline(p, c, alignment);
    }
    return dst;
}

constexpr void* memmove(void* pdst, void const* psrc, usize n)
{
    auto dst = reinterpret_cast<u8*>(pdst);
    auto src = reinterpret_cast<u8 const*>(psrc);
    // TODO: Optimize

    if (src < dst)
        for (dst += n, src += n; n--;)
            *--dst = *--src;
    else
        while (n--)
            *dst++ = *src++;
    return pdst;
}

[[clang::always_inline]] static void* PtrAdd(void const* ptr, usize offset)
{
    return reinterpret_cast<u8*>(const_cast<void*>(ptr)) + offset;
}

static void memcpy512(void* dst, void const* src, usize length)
{
    while (length != 0) {
        __builtin_memcpy_inline(dst, src, 64);
        dst = PtrAdd(dst, 64);
        src = PtrAdd(src, 64);
        length--;
    }
}

static void memcpy256(void* dst, void const* src, usize length)
{
    if (length % 2 != 0) {
        __builtin_memcpy_inline(dst, src, 32);
        dst = PtrAdd(dst, 32);
        src = PtrAdd(src, 32);
        length--;
    }
    memcpy512(dst, src, length / 2);
}

static void memcpy128(void* dst, void const* src, usize length)
{
    if (length % 2 != 0) {
        __builtin_memcpy_inline(dst, src, 16);
        dst = PtrAdd(dst, 16);
        src = PtrAdd(src, 16);
        length--;
    }
    memcpy256(dst, src, length / 2);
}

static void memcpy64(void* dst, void const* src, usize length)
{
    if (length % 2 != 0) {
        __builtin_memcpy_inline(dst, src, 8);
        dst = PtrAdd(dst, 8);
        src = PtrAdd(src, 8);
        length--;
    }
    memcpy128(dst, src, length / 2);
}

static void memcpy32(void* dst, void const* src, usize length)
{
    if (length % 2 != 0) {
        __builtin_memcpy_inline(dst, src, 4);
        dst = PtrAdd(dst, 4);
        src = PtrAdd(src, 4);
        length--;
    }
    memcpy64(dst, src, length / 2);
}

static void memcpy16(void* dst, void const* src, usize length)
{
    if (length % 2 != 0) {
        __builtin_memcpy_inline(dst, src, 2);
        dst = PtrAdd(dst, 2);
        src = PtrAdd(src, 2);
        length--;
    }
    memcpy32(dst, src, length / 2);
}

static void memcpy8(void* dst, void const* src, usize length)
{
    if (length % 2 != 0) {
        __builtin_memcpy_inline(dst, src, 1);
        dst = PtrAdd(dst, 1);
        src = PtrAdd(src, 1);
        length--;
    }
    memcpy16(dst, src, length / 2);
}

constexpr void* memcpy(void* dst, void const* src, usize length)
{
    if consteval {
        auto dstBytes = reinterpret_cast<u8*>(dst);
        auto srcBytes = reinterpret_cast<u8 const*>(src);
        auto tmp = dst;
        while (length-- != 0) {
            *dstBytes++ = *srcBytes++;
        }
        return tmp;
    } else {
        memcpy8(dst, src, length);
        return dst;
    }
}


///
///---------------------------------------------------------------------------------------------------------------------------
/// C STRING FUNCTIONS
///---------------------------------------------------------------------------------------------------------------------------
///

constexpr usize strlen(char const* s)
{
    usize len = 0;
    while (*s++ != '\0') {
        len++;
    }
    return len;
}

constexpr int strcmp(char const* s1, char const* s2)
{
    for (; (*s1 != '\0') && (*s1 == *s2); s1++, s2++)
        ;
    return *s1 - *s2;
}

//
//
//
//
constexpr char* strcat(char* __restrict__ dest, char const* __restrict__ src)
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
constexpr char* strncat(char* __restrict__ dest, char const* __restrict__ src, usize n)
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
constexpr char* strcpy(char* __restrict__ dst, char const* __restrict__ src)
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
constexpr char* strncpy(char* __restrict__ dest, char const* __restrict__ src, usize n)
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
constexpr usize wcslen(wchar_t const* start)
{
    // NB: start is not checked for nullptr!
    wchar_t const* end = start;
    while (*end != L'\0')
        ++end;

    [[assume(end > start)]];
    return usize(end - start);
}

constexpr usize wcsnlen(wchar_t const* start, usize n)
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
constexpr usize strnlen(char const* s, __SIZE_TYPE__ len)
{
    usize i = 0;
    for (; i < len && s[i] != '\0'; ++i)
        ;
    return i;
}


//
//
//
//
/*
constexpr int strncmp(char const* s1, char const* s2, __SIZE_TYPE__ n)
{
    while (n--)
        if (*s1++ != *s2++) {

        }
            return *reinterpret_cast<u8 const * (s1 - 1) - *u8 const * (s2 - 1);
    return 0;
}
*/

/*
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

    return char*(__builtin_memcpy(copyS, s, len));
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
                    return void*(&y[j]);
                j += l;
            }
        }
    } else
        do {
            if (*y == *x)
                return void*(y);
            y++;
        } while (--n);

    return nullptr;
}

char* strstr(char const* haystack, char const* needle)
{
    return char*(memmem(haystack, strlen(haystack), needle, strlen(needle)));
}

char* strnstr(char const* haystack, char const* needle, size_t len)
{
    return char*(memmem(haystack, strlen(haystack), needle, len));
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
*/

/*
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
*/
}

UNSAFE_END;
#endif
