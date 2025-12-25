# 1 "./test/blank.cc"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 475 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "./test/blank.cc" 2
# 1 "./include/commons/startup.hh" 1
# 29 "./include/commons/startup.hh"
# 1 "./include/commons/system.hh" 1
# 31 "./include/commons/system.hh"
# 1 "./include/commons/core.hh" 1
# 30 "./include/commons/core.hh"
# 1 "./include/commons/core/base_definitions.hh" 1
# 49 "./include/commons/core/base_definitions.hh"
using u0 = void;
using u1 = unsigned _BitInt(1);





using u2 = unsigned _BitInt(2);

using i2 = signed _BitInt(2);
using u3 = unsigned _BitInt(3);
using i3 = signed _BitInt(3);
using u4 = unsigned _BitInt(4);
using i4 = signed _BitInt(4);
using u8 = unsigned char;
using i8 = signed char;
using u16 = unsigned short;
using i16 = short;
using u24 = unsigned _BitInt(24);
using i24 = signed _BitInt(24);
using u32 = unsigned int;
using i32 = int;
using u48 = unsigned _BitInt(48);
using i48 = signed _BitInt(48);
using u64 = long unsigned int;
using i64 = long int;


using u128 = __uint128_t;





using i128 = __int128;




using u256 = unsigned _BitInt(256);
using i256 = _BitInt(256);
using usize = long unsigned int;
using isize = long int;

static_assert(sizeof(u8) == 1 && sizeof(i8) == 1);
static_assert(sizeof(u16) == 2 && sizeof(i16) == 2);
static_assert(sizeof(u32) == 4 && sizeof(i32) == 4);
static_assert(sizeof(u64) == 8 && sizeof(i64) == 8);
static_assert(sizeof(u128) == 16 && sizeof(i128) == 16);
static_assert(sizeof(u256) == 32 && sizeof(i256) == 32);
static_assert(sizeof(usize) == sizeof(void*) && sizeof(isize) == sizeof(void*));
# 187 "./include/commons/core/base_definitions.hh"
namespace cm {
# 212 "./include/commons/core/base_definitions.hh"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreserved-macro-identifier"



#pragma GCC diagnostic pop

template<typename F>
class DeferredOperation {
    F f;

public:
    [[gnu::always_inline]] inline DeferredOperation(F f) noexcept
        : f(f)
    {}

    [[gnu::always_inline]] inline ~DeferredOperation() noexcept(noexcept(f())) { f(); }
};

template<typename F>
DeferredOperation(F) -> DeferredOperation<F>;




struct CPU
{



    [[noreturn]]
    constexpr static void trap()
    {
        if consteval {
            while (true)
                ;
        } else {

            __builtin_trap();





        }
    }




    consteval bool isBigEndian()
    {
        auto c = 0x01020304;
        struct dummy
        {
            unsigned char b[4];
        };
        return __builtin_bit_cast(dummy, c).b[0] == 1;
    }

} inline constexpr CPU;




template<typename A, typename B>
struct Pair
{
    A first;
    B second;

    constexpr Pair() = default;
    constexpr Pair(A const& a, B const& b)
        : first(a), second(b)
    {}
};

template<typename A, typename B>
Pair(A const&, B const&) -> Pair<A, B>;


}

namespace std {
using size_t = long unsigned int;
struct nothrow_t;
enum class align_val_t : std::size_t {
};
}

[[gnu::alloc_size(1)]]
constexpr inline void* operator new(std::size_t, void* ptr) noexcept
{
    return ptr;
}

[[gnu::alloc_size(1)]]
constexpr inline void* operator new[](std::size_t, void* ptr) noexcept
{
    return ptr;
}



[[gnu::alloc_size(1)]]
void* operator new(std::size_t size);

[[gnu::alloc_size(1)]]
void* operator new[](std::size_t size);

[[gnu::alloc_size(1)]]
[[gnu::alloc_align(2)]]
void* operator new(std::size_t size, std::align_val_t al);

[[gnu::alloc_size(1)]]
[[gnu::alloc_align(2)]]
void* operator new[](std::size_t size, std::align_val_t al);



[[gnu::alloc_size(1)]]
void* operator new(std::size_t size, std::nothrow_t const&) noexcept;

[[gnu::alloc_size(1)]]
void* operator new[](std::size_t size, std::nothrow_t const&) noexcept;

[[gnu::alloc_size(1)]]
[[gnu::alloc_align(2)]]
void* operator new(std::size_t size, std::align_val_t al, std::nothrow_t const&) noexcept;

[[gnu::alloc_size(1)]]
[[gnu::alloc_align(2)]]
void* operator new[](std::size_t size, std::align_val_t al, std::nothrow_t const&) noexcept;

void operator delete(void* ptr) noexcept;
void operator delete[](void* ptr) noexcept;
void operator delete(void* ptr, std::size_t sz) noexcept;
void operator delete[](void* ptr, std::size_t sz) noexcept;


template<typename T, typename... Args>
constexpr inline T* ConstructInPlace(void* ptr, Args&&... args)
{
    return new (ptr) T(Forward<Args>(args)...);
}

template<unsigned long long Size>
struct BytePadding
{
    unsigned char pad[Size];
};


namespace cm {

template<typename T>
concept HasOutputStringMethod = requires (T value) {
    {
        T::outputString(value, [](char) {})
    };
};

constexpr void __outputString(auto const&, auto const&);


template<typename T>
constexpr void OutputString(T const& value, auto const& out)
{
    if constexpr (HasOutputStringMethod<T>) {
        T::outputString(value, out);
    } else {
        __outputString(value, out);
    }
}
}






template<typename Derived>
struct IEquatable
{
    constexpr inline bool equalsAny(auto const&... args) const
    {
        return ((static_cast<Derived const*>(this)->equals(args)) || ...);
    }
    constexpr inline bool equalsAll(auto const&... args) const
    {
        return ((static_cast<Derived const*>(this)->equals(args)) && ...);
    }
    constexpr inline bool operator==(auto const& x) const { return static_cast<Derived const*>(this)->equals(x); }
    constexpr inline bool operator!=(auto const& x) const { return !static_cast<Derived const*>(this)->equals(x); }
};
# 31 "./include/commons/core.hh" 2
# 1 "./include/commons/resources/OneCharStringTable.hh" 1



namespace cm::Data {




constexpr inline static auto oneCharStringTable(char c)
{
#pragma GCC diagnostic push
# 11 "./include/commons/resources/OneCharStringTable.hh"
                                  ;
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 12 "./include/commons/resources/OneCharStringTable.hh"
                                                               ;
    return &"\x00\x00\x01\x00\x02\x00\x03\x00\x04\x00\x05\x00\x06\x00\x07\x00\x08\x00\x09\x00\x0A\x00\x0B\x00\x0C\x00\x0D\x00\x0E\x00\x0F\x00\x10\x00\x11\x00\x12\x00\x13\x00\x14\x00\x15\x00\x16\x00\x17\x00\x18\x00\x19\x00\x1A\x00\x1B\x00\x1C\x00\x1D\x00\x1E\x00\x1F\x00\x20\x00\x21\x00\x22\x00\x23\x00\x24\x00\x25\x00\x26\x00\x27\x00\x28\x00\x29\x00\x2A\x00\x2B\x00\x2C\x00\x2D\x00\x2E\x00\x2F\x00\x30\x00\x31\x00\x32\x00\x33\x00\x34\x00\x35\x00\x36\x00\x37\x00\x38\x00\x39\x00\x3A\x00\x3B\x00\x3C\x00\x3D\x00\x3E\x00\x3F\x00\x40\x00\x41\x00\x42\x00\x43\x00\x44\x00\x45\x00\x46\x00\x47\x00\x48\x00\x49\x00\x4A\x00\x4B\x00\x4C\x00\x4D\x00\x4E\x00\x4F\x00\x50\x00\x51\x00\x52\x00\x53\x00\x54\x00\x55\x00\x56\x00\x57\x00\x58\x00\x59\x00\x5A\x00\x5B\x00\x5C\x00\x5D\x00\x5E\x00\x5F\x00\x60\x00\x61\x00\x62\x00\x63\x00\x64\x00\x65\x00\x66\x00\x67\x00\x68\x00\x69\x00\x6A\x00\x6B\x00\x6C\x00\x6D\x00\x6E\x00\x6F\x00\x70\x00\x71\x00\x72\x00\x73\x00\x74\x00\x75\x00\x76\x00\x77\x00\x78\x00\x79\x00\x7A\x00\x7B\x00\x7C\x00\x7D\x00\x7E\x00\x7F\x00\x80\x00\x81\x00\x82\x00\x83\x00\x84\x00\x85\x00\x86\x00\x87\x00\x88\x00\x89\x00\x8A\x00\x8B\x00\x8C\x00\x8D\x00\x8E\x00\x8F\x00\x90\x00\x91\x00\x92\x00\x93\x00\x94\x00\x95\x00\x96\x00\x97\x00\x98\x00\x99\x00\x9A\x00\x9B\x00\x9C\x00\x9D\x00\x9E\x00\x9F\x00\xA0\x00\xA1\x00\xA2\x00\xA3\x00\xA4\x00\xA5\x00\xA6\x00\xA7\x00\xA8\x00\xA9\x00\xAA\x00\xAB\x00\xAC\x00\xAD\x00\xAE\x00\xAF\x00\xB0\x00\xB1\x00\xB2\x00\xB3\x00\xB4\x00\xB5\x00\xB6\x00\xB7\x00\xB8\x00\xB9\x00\xBA\x00\xBB\x00\xBC\x00\xBD\x00\xBE\x00\xBF\x00\xC0\x00\xC1\x00\xC2\x00\xC3\x00\xC4\x00\xC5\x00\xC6\x00\xC7\x00\xC8\x00\xC9\x00\xCA\x00\xCB\x00\xCC\x00\xCD\x00\xCE\x00\xCF\x00\xD0\x00\xD1\x00\xD2\x00\xD3\x00\xD4\x00\xD5\x00\xD6\x00\xD7\x00\xD8\x00\xD9\x00\xDA\x00\xDB\x00\xDC\x00\xDD\x00\xDE\x00\xDF\x00\xE0\x00\xE1\x00\xE2\x00\xE3\x00\xE4\x00\xE5\x00\xE6\x00\xE7\x00\xE8\x00\xE9\x00\xEA\x00\xEB\x00\xEC\x00\xED\x00\xEE\x00\xEF\x00\xF0\x00\xF1\x00\xF2\x00\xF3\x00\xF4\x00\xF5\x00\xF6\x00\xF7\x00\xF8\x00\xF9\x00\xFA\x00\xFB\x00\xFC\x00\xFD\x00\xFE\x00\xFF\x00"[c * 2];
# 47 "./include/commons/resources/OneCharStringTable.hh"
}

}
# 32 "./include/commons/core.hh" 2
# 1 "./include/commons/core/cstring.hh" 1
# 20 "./include/commons/core/cstring.hh"
#pragma GCC diagnostic push
# 20 "./include/commons/core/cstring.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 20 "./include/commons/core/cstring.hh"
            ;

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
# 158 "./include/commons/core/cstring.hh"
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





constexpr char* strcpy(char* __restrict__ dst, char const* __restrict__ src)
{
    char* tmp = dst;
    while ((*dst++ = *src++) != '\0')
        ;
    return tmp;
}





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






constexpr usize wcslen(wchar_t const* start)
{

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





constexpr usize strnlen(char const* s, long unsigned int len)
{
    usize i = 0;
    for (; i < len && s[i] != '\0'; ++i)
        ;
    return i;
}
# 551 "./include/commons/core/cstring.hh"
}

#pragma GCC diagnostic pop
# 553 "./include/commons/core/cstring.hh"
          ;
# 33 "./include/commons/core.hh" 2
# 1 "./include/commons/core/range.hh" 1
# 20 "./include/commons/core/range.hh"
namespace cm {


template<typename T>
class Range {
public:
    struct DefaultStep
    {};

    constexpr Range(T const& last)
        : Range(T{}, last)
    {}

    constexpr Range(T const& first, T const& last, T step = {})
        : _first(first),
          _last(last),
          _step(step == T{} ? 1 : step),
          _step_function(first > last ? ([](T step_, T& iterator) { iterator -= step_; }) : ([](T step_, T& iterator) {
              iterator += step_;
          }))
    {}


    struct Iterator
    {
        constexpr Iterator(Range const& range, T const& position)
            : _range(range), _position(position)
        {}

        constexpr Iterator& operator++()
        {
            _range._step_function(_range._step, this->_position);
            return *this;
        }

        constexpr bool operator==(Iterator const& other) const noexcept
        {
            return &_range == &other._range && _position == other._position;
        }

        constexpr T operator*() const noexcept { return _position; }

    private:
        Range const& _range;
        T _position;
    };

    constexpr Iterator begin() const { return Iterator(*this, _first); }
    constexpr Iterator end() const { return Iterator(*this, _last); }


private:
    T const _first;
    T const _last;
    T const _step;
    void (*_step_function)(T step, T& value);
};

template<typename T>
Range(T const&) -> Range<T>;
template<typename T>
Range(T const&, T const&) -> Range<T>;



}
# 34 "./include/commons/core.hh" 2
# 1 "./include/commons/core/reflection_type.hh" 1
# 21 "./include/commons/core/reflection_type.hh"
struct Nothing
{};

struct SafeBinaryMoveable
{};
# 38 "./include/commons/core/reflection_type.hh"
template<typename T>
struct TConstRemoved { using Type = T; };
template<typename T>
struct TConstRemoved<T const> { using Type = T; };
template<typename T>
struct TConstRemoved<T const volatile> { using Type = T volatile; };
template<typename T>
struct TConstRemoved<T const*> { using Type = T*; };
template<typename T>
struct TConstRemoved<T const volatile*> { using Type = T volatile*; };
template<typename T>
struct TConstRemoved<T const&> { using Type = T&; };
template<typename T>
struct TConstRemoved<T const volatile&> { using Type = T volatile&; };
template<typename T>
struct TVolatileRemoved { using Type = T; };
template<typename T>
struct TVolatileRemoved<T const> { using Type = T const; };
template<typename T>
struct TVolatileRemoved<T volatile> { using Type = T; };
template<typename T>
struct TVolatileRemoved<T const volatile> { using Type = T const; };
template<typename T>
struct TVolatileRemoved<T const*> { using Type = T const*; };
template<typename T>
struct TVolatileRemoved<T volatile*> { using Type = T*; };
template<typename T>
struct TVolatileRemoved<T const volatile*> { using Type = T const*; };
template<typename T>
struct TVolatileRemoved<T const&> { using Type = T const&; };
template<typename T>
struct TVolatileRemoved<T volatile&> { using Type = T&; };
template<typename T>
struct TVolatileRemoved<T const volatile&> { using Type = T const&; };
template<typename T>
struct TPointerRemoved { using Type = T; };
template<typename T>
struct TPointerRemoved<T*> { using Type = T; };
template<typename T>
struct TRefRemoved { using Type = T; };
template<typename T>
struct TRefRemoved<T&> { using Type = T; };
template<typename T>
struct TRValueRefRemoved { using Type = T; };
template<typename T>
struct TRValueRefRemoved<T&&> { using Type = T; };
# 97 "./include/commons/core/reflection_type.hh"
template<typename T>
using ConstRemoved = typename TConstRemoved<T>::Type;

template<typename T>
using VolatileRemoved = typename TVolatileRemoved<T>::Type;

template<typename T>
using CVRemoved = typename TConstRemoved<typename TVolatileRemoved<T>::Type>::Type;

template<typename T>
using CRefRemoved = typename TRefRemoved<typename TConstRemoved<T>::Type>::Type;

template<typename T>
using CVRefRemoved = typename TRefRemoved<typename TConstRemoved<typename TVolatileRemoved<T>::Type>::Type>::Type;

template<typename T>
using PointerRemoved = typename TPointerRemoved<T>::Type;

template<typename T>
using RefRemoved = typename TRefRemoved<T>::Type;

template<typename T>
using RefOrPointerRemoved = typename TRefRemoved<typename TPointerRemoved<T>::Type>::Type;

template<typename T>
using RValueRefRemoved = typename TRValueRefRemoved<T>::Type;
# 136 "./include/commons/core/reflection_type.hh"
template<typename A, typename B>
concept IsSame = __is_same(A, B);
# 151 "./include/commons/core/reflection_type.hh"
template<typename T, typename... A>
concept IsSameAsOneOf = ((IsSame<T, A>) || ...);

static_assert(IsSame<int, int>);
static_assert(!IsSame<int, long>);
static_assert(IsSame<ConstRemoved<char const*>, char*>);
static_assert(IsSame<ConstRemoved<char const volatile*>, char volatile*>);
static_assert(IsSame<ConstRemoved<char const* const>, char const*>);
static_assert(IsSame<ConstRemoved<char const volatile* const>, char const volatile*>);
static_assert(IsSame<ConstRemoved<char const&>, char&>);
static_assert(IsSame<ConstRemoved<char const volatile*>, char volatile*>);
static_assert(IsSame<ConstRemoved<char const* const>, char const*>);
static_assert(IsSame<ConstRemoved<char const volatile* const>, char const volatile*>);
static_assert(IsSame<VolatileRemoved<char volatile>, char>);
static_assert(IsSame<VolatileRemoved<char volatile*>, char*>);
static_assert(IsSame<VolatileRemoved<char volatile&>, char&>);
static_assert(IsSame<VolatileRemoved<char volatile** volatile>, char volatile**>);
static_assert(IsSame<CVRemoved<char const* volatile>, char*>);


template<typename T>
concept IsConst = bool{!IsSame<T, ConstRemoved<T>>};

template<typename T>
concept IsVolatile = bool{!IsSame<T, VolatileRemoved<T>>};


template<typename T>
concept IsPointer = __is_pointer(T);
# 197 "./include/commons/core/reflection_type.hh"
static_assert(IsPointer<int volatile*>);
static_assert(IsPointer<int***>);
static_assert(IsPointer<int** const* const volatile>);
static_assert(IsPointer<void (*)(void*) noexcept>);
static_assert(!IsPointer<int>);


template<typename T>
concept IsReference = bool{!IsSame<T, RefRemoved<T>>};

template<class T>
constexpr inline T&& Forward(RefRemoved<T>& t) noexcept
{
    return static_cast<T&&>(t);
}

template<class T>
constexpr inline T&& Forward(RefRemoved<T>&& t) noexcept
{
    static_assert(!IsReference<T>, "Cannot Forward an rvalue as an lvalue");
    return static_cast<T&&>(t);
}

template<typename T>
constexpr RefRemoved<T>&& move(T&& arg) noexcept
{
    return static_cast<RefRemoved<T>&&>(arg);
}

template<typename T>
concept IsConstPointer = bool{IsConst<T> && IsPointer<T>};

template<typename T>
concept IsCVPointer = bool{IsConst<T> && IsVolatile<T> && IsPointer<T>};

template<typename T>
concept IsConstReference = bool{IsConst<T> && IsReference<T>};

template<typename T>
concept IsCVReference = bool{IsConst<T> && IsVolatile<T> && IsReference<T>};

template<typename T>
concept IsEnum = __is_enum(T);

template<typename T>
concept IsClass = __is_class(T);

template<typename T>
concept IsNotClass = !__is_class(T);

template<typename T>
concept IsFunction = __is_function(T);

template<typename T>
concept IsAbstract = __is_abstract(T);

template<typename Derived, typename Base>
concept IsBaseOf = __is_base_of(Base, Derived);

template<typename Base, typename Derived>
concept IsDerivedFrom = __is_base_of(Base, Derived);


template<typename From, typename To>
concept ConvertibleTo = __is_convertible(From, To);
# 270 "./include/commons/core/reflection_type.hh"
template<typename T>
concept TypeIsBool = ConvertibleTo<T, bool>;

template<auto value>
concept ValueIsBool = TypeIsBool<decltype(value)>;




namespace detail {
template<typename T, bool bIsEnum>
struct GetUnderlyingTypeOf;

template<typename T>
struct GetUnderlyingTypeOf<T, true>
{
    using Type = __underlying_type(CVRefRemoved<T>);
};

template<typename T>
struct GetUnderlyingTypeOf<T, false>
{
    using Type = CVRefRemoved<T>;
};
}

template<typename T>
using UnderlyingTypeOf = typename detail::GetUnderlyingTypeOf<T, IsEnum<T>>::Type;

template<typename T, typename U>
concept IsUnderlyingType = IsSame<UnderlyingTypeOf<T>, U>;

template<typename T, typename... A>
concept IsUnderlyingTypeOneOf = ((IsSame<UnderlyingTypeOf<T>, A>) || ...);

template<typename T>
concept IsBool = IsUnderlyingTypeOneOf<T, bool>;


template<typename T>
concept IsInteger = !IsBool<T> && __is_integral(UnderlyingTypeOf<T>);

template<typename T>
concept IsIntegerPrimitiveType = !IsBool<T> && __is_integral(T);
# 327 "./include/commons/core/reflection_type.hh"
static_assert(IsSame<decltype(0), int>);
static_assert(IsSame<UnderlyingTypeOf<decltype(0)>, int>);
static_assert(IsUnderlyingType<decltype(0), int>);


template<typename T>
concept IsUnsignedInteger = IsPointer<T> || (IsInteger<T> && static_cast<T>(-1) > static_cast<T>(1));

template<typename T>
concept IsIntegerSigned = IsPointer<T> || (IsInteger<T> && static_cast<T>(-1) < static_cast<T>(1));


template<typename T>
concept IsFloatingPoint = IsUnderlyingTypeOneOf<T, float, double, long double>;

template<typename T>
concept IsChar = IsUnderlyingTypeOneOf<T, char, wchar_t, char8_t, char16_t, char32_t>;

template<typename T>
concept IsPrimitiveType = IsPointer<T> || IsIntegerPrimitiveType<T> || IsBool<T> || IsFloatingPoint<T> || IsChar<T>;


template<typename T>
concept IsMutable = requires (T a, T b) {
    { a = b };
};

template<int index, typename A, typename B>
struct TSelectType;

template<typename A, typename B>
struct TSelectType<0, A, B>
{
    using Type = A;
};

template<typename A, typename B>
struct TSelectType<1, A, B>
{
    using Type = B;
};

template<int index, typename A, typename B>
using SelectType = typename TSelectType<index, A, B>::Type;
# 383 "./include/commons/core/reflection_type.hh"
template<typename T>
concept DefaultConstructible = requires { T{}; };

template<typename T, typename... Args>
concept Constructible = __is_constructible(CVRefRemoved<T>, Args...);

template<typename T>
concept CopyConstructible = Constructible<T, T const&>;

template<typename T>
concept CopyAssignable = requires (T& a, T const& b) {
    { a = b };
};

template<typename T, typename U = T>
concept MoveConstructible = Constructible<T, U&&>;

template<typename T, typename U = T>
concept MoveAssignable = requires (T& a, U&& b) {
    { a = b };
};

template<typename T>
concept TriviallyCopyConstructible = __is_trivially_copyable(T);

template<typename T>
concept TriviallyMoveConstructible = __is_trivially_copyable(T) && __is_trivially_constructible(T, T&&);

template<typename T>
concept TriviallyDefaultConstructible = IsPrimitiveType<T> || __is_trivially_constructible(T);

template<typename T, typename U = T>
concept TriviallyCopyAssignable =
    IsPrimitiveType<T> ||
                                                                            __is_trivially_assignable(T, U);

template<typename T, typename U = T>
concept TriviallyMoveAssignable = (IsPrimitiveType<T> && IsPrimitiveType<U>) || __is_trivially_assignable(T, U&&);

template<typename T>
concept TriviallyDestructible = IsPrimitiveType<T> || __is_trivially_destructible(T);
# 432 "./include/commons/core/reflection_type.hh"
template<typename T>
concept IsTriviallyRelocatable = __builtin_is_cpp_trivially_relocatable(T);

template<typename T>
concept Destructible = __is_destructible(T);

template<typename T, typename U>
concept IsImplicitlyAssignable = requires (T& t, U const& u) {
    { t = u };
};
# 454 "./include/commons/core/reflection_type.hh"
template<typename Func, typename... Args>
concept IsCallableWith = requires (Func func, Args... args) {
    { func(args...) };
};

template<typename F, typename ReturnType, typename... Args>
concept IsCallableAndReturns = requires (F f, Args... args) {
    { f(args...) } -> IsSame<ReturnType>;
};
# 475 "./include/commons/core/reflection_type.hh"
template<typename T>
concept IsPrefixIncrementable = requires (T a) {
    { ++a };
};

template<typename T>
concept IsPostfixIncrementable = requires (T a) {
    { a++ };
};

template<typename T>
concept IsIncrementable = IsPrefixIncrementable<T> && IsPostfixIncrementable<T>;

template<typename T>
concept IsPrefixDecrementable = requires (T a) {
    { --a };
};

template<typename T>
concept IsPostfixDecrementable = requires (T a) {
    { a-- };
};

template<typename T>
concept IsDecrementable = IsPrefixDecrementable<T> && IsPostfixDecrementable<T>;

template<typename T>
concept IsIncrementableDecrementable = IsIncrementable<T> && IsDecrementable<T>;


template<typename T1, typename T2>
concept IsComparableLT = requires (T1 t1, T2 t2) {
    { t1 < t2 };
};

template<typename T1, typename T2>
concept IsComparableGT = requires (T1 t1, T2 t2) {
    { t1 > t2 };
};

template<typename T1, typename T2>
concept IsComparableLE = requires (T1 t1, T2 t2) {
    { t1 <= t2 };
};

template<typename T1, typename T2>
concept IsComparableGE = requires (T1 t1, T2 t2) {
    { t1 >= t2 };
};

template<typename T1, typename T2>
concept IsComparable =
    IsComparableLT<T1, T2> && IsComparableGT<T1, T2> && IsComparableLE<T1, T2> && IsComparableGE<T1, T2>;

template<typename T1, typename T2 = T1>
concept IsEquatable = requires (T1 t1, T2 t2) {
    { t1 == t2 };
};

template<typename T1, typename T2>
concept IsDivideable = requires (T1 t1, T2 t2) {
    { t1 / t2 };
};
template<typename T1, typename T2>
concept IsDivisionAssignable = requires (T1 t1, T2 t2) {
    { t1 /= t2 };
};
template<typename T1, typename T2>
concept IsMultipliable = requires (T1 t1, T2 t2) {
    { t1 * t2 };
};
template<typename T1, typename T2>
concept IsMultiplicationAssignable = requires (T1 t1, T2 t2) {
    { t1 *= t2 };
};
template<typename T1, typename T2>
concept IsAddable = requires (T1 t1, T2 t2) {
    { t1 + t2 };
};
template<typename T1, typename T2>
concept IsAdditionAssignable = requires (T1 t1, T2 t2) {
    { t1 += t2 };
};
template<typename T1, typename T2>
concept IsSubtractable = requires (T1 t1, T2 t2) {
    { t1 - t2 };
};
template<typename T1, typename T2>
concept IsSubtractionAssignable = requires (T1 t1, T2 t2) {
    { t1 -= t2 };
};
template<typename T1, typename T2>
concept IsMODAble = requires (T1 t1, T2 t2) {
    { t1 % t2 };
};
template<typename T1, typename T2>
concept IsMODAssignable = requires (T1 t1, T2 t2) {
    { t1 %= t2 };
};



template<typename T1, typename T2>
concept IsANDAble = requires (T1 t1, T2 t2) {
    { t1 & t2 };
};
template<typename T1, typename T2>
concept IsANDAssignable = requires (T1 t1, T2 t2) {
    { t1 &= t2 };
};
template<typename T1, typename T2>
concept IsORAble = requires (T1 t1, T2 t2) {
    { t1 | t2 };
};
template<typename T1, typename T2>
concept IsORAssignable = requires (T1 t1, T2 t2) {
    { t1 |= t2 };
};
template<typename T1>
concept IsBitwiseNOTAble = requires (T1 t1) {
    { ~t1 };
};
template<typename T1, typename T2>
concept IsXORAble = requires (T1 t1, T2 t2) {
    { t1 ^ t2 };
};
template<typename T1, typename T2>
concept IsXORAssignable = requires (T1 t1, T2 t2) {
    { t1 ^= t2 };
};
template<typename T1, typename T2>
concept IsRightShiftable = requires (T1 t1, T2 t2) {
    { t1 >> t2 };
};
template<typename T1, typename T2>
concept IsRightShiftAssignable = requires (T1 t1, T2 t2) {
    { t1 >>= t2 };
};
template<typename T1, typename T2>
concept IsLeftShiftable = requires (T1 t1, T2 t2) {
    { t1 << t2 };
};
template<typename T1, typename T2>
concept IsLeftShiftAssignable = requires (T1 t1, T2 t2) {
    { t1 <<= t2 };
};

template<typename T, typename IndexType>
concept IsIndexable = requires (T t, IndexType i) {
    { t[i] };
};

template<typename T>
concept HasOperatorAdd = requires {
    { &T::operator+ };
};
# 642 "./include/commons/core/reflection_type.hh"
template<typename T>
concept IsPrimitiveData = bool{
    TriviallyDestructible<T> && TriviallyCopyConstructible<T> && TriviallyDefaultConstructible<T> &&
    TriviallyCopyAssignable<T>};

static_assert(TriviallyDestructible<char>);
static_assert(TriviallyCopyAssignable<char>);

template<bool B, class T>
class TConstIf;

template<class T>
class TConstIf<true, T> {
    using Type = T const;
};

template<class T>
class TConstIf<false, T> {
    using Type = T;
};

template<bool B, class T>
using ConstIf = typename TConstIf<B, T>::Type;


namespace detail {
template<typename T>
struct AddRValueReference
{
    using Type = T&&;
};

template<>
struct AddRValueReference<void>
{
    using Type = void;
};
}

template<typename T>
using RValueReferenceAdded = typename detail::AddRValueReference<T>::Type;


template<typename T>
RValueReferenceAdded<T> declval() noexcept
{
    static_assert(false, "declval not allowed in an evaluated context");
}

template<class T, template<class...> class U>
constexpr inline bool is_instance_of_template = false;

template<template<class...> class U, class... Vs>
constexpr inline bool is_instance_of_template<U<Vs...>, U> = true;





class NonCopyable {
protected:
    constexpr NonCopyable() = default;
    constexpr ~NonCopyable() = default;

public:
    constexpr NonCopyable(NonCopyable const&) = delete;
    constexpr NonCopyable& operator=(NonCopyable const&) = delete;

    constexpr NonCopyable(NonCopyable&&) noexcept(true) = default;
    constexpr NonCopyable& operator=(NonCopyable&&) noexcept(true) = default;
};

template<typename T, unsigned ID>
class Wrapper {
    T val_;

public:

    Wrapper(T val)
        : val_(val)
    {}


    operator T() const noexcept { return val_; }
};






template<typename Derived>
struct IArithmetic
{

    constexpr inline Derived operator+(auto a) const
    {
        Derived d(static_cast<Derived const&>(*this));
        d.add(a);
        return d;
    }

    constexpr inline Derived& operator+=(auto a)
    {
        static_cast<Derived&>(*this).add(a);
        return static_cast<Derived&>(*this);
    }

    constexpr inline Derived operator-(auto a) const
    {
        Derived d(static_cast<Derived const&>(*this));
        d.sub(a);
        return d;
    }

    constexpr inline Derived& operator-=(auto a)
    {
        static_cast<Derived&>(*this).sub(a);
        return static_cast<Derived&>(*this);
    }

    constexpr inline Derived operator*(auto a) const
    {
        Derived d(static_cast<Derived const&>(*this));
        d.mul(a);
        return d;
    }

    constexpr inline Derived& operator*=(auto a)
    {
        static_cast<Derived&>(*this).mul(a);
        return static_cast<Derived&>(*this);
    }

    constexpr inline Derived operator/(auto a) const
    {
        Derived d(static_cast<Derived const&>(*this));
        d.div(a);
        return d;
    }

    constexpr inline Derived& operator/=(auto a)
    {
        static_cast<Derived&>(*this).div(a);
        return static_cast<Derived&>(*this);
    }
};

template<typename T, typename U = T>
concept IsArithmetic = requires (T a, U b) {
    { a + b } -> ConvertibleTo<T>;
    { a += b };
    { a - b } -> ConvertibleTo<T>;
    { a -= b };
    { a * b } -> ConvertibleTo<T>;
    { a *= b };
    { a / b } -> ConvertibleTo<T>;
    { a /= b };
};

template<typename T>
concept IsIterator = requires (T a) {
    { ++a };
    { *a };
    { a == a } -> IsBool;
    { a != a } -> IsBool;
};


template<typename T>
concept IsIterable = requires (T a) {
    { a.begin() } -> IsIterator;
    { a.end() } -> IsIterator;
};
# 35 "./include/commons/core.hh" 2
# 1 "./include/commons/core/comparable.hh" 1
# 20 "./include/commons/core/comparable.hh"
namespace cm {







using Comparator = int (*)(void const*, void const*);





template<typename Derived>
struct IComparable
{

    constexpr static Comparator comparator = [](void const* e1, void const* e2) -> int {
        auto t1 = reinterpret_cast<Derived const*>(e1);
        auto t2 = reinterpret_cast<Derived const*>(e2);
        return t1->compare(*t2);
    };

    constexpr inline bool operator<(auto const& x) const { return static_cast<Derived const*>(this)->compare(x) < 0; }

    constexpr inline bool operator>(auto const& x) const { return static_cast<Derived const*>(this)->compare(x) > 0; }

    constexpr inline bool operator<=(auto const& x) const { return static_cast<Derived const*>(this)->compare(x) <= 0; }

    constexpr inline bool operator>=(auto const& x) const { return static_cast<Derived const*>(this)->compare(x) >= 0; }

    constexpr inline bool lessThanAny(auto... args) const { return ((this->operator<(args)) || ...); }

    constexpr inline bool lessThanAll(auto... args) const { return ((this->operator<(args)) && ...); }

    constexpr inline bool greaterThanAny(auto... args) const { return ((this->operator>(args)) || ...); }

    constexpr inline bool greaterThanAll(auto... args) const { return ((this->operator>(args)) && ...); }

    constexpr inline bool lessEqualThanAny(auto... args) const { return ((this->operator<=(args)) || ...); }

    constexpr inline bool lessEqualThanAll(auto... args) const { return ((this->operator<=(args)) && ...); }

    constexpr inline bool greaterEqualThanAny(auto... args) const { return ((this->operator>=(args)) || ...); }

    constexpr inline bool greaterEqualThanAll(auto... args) const { return ((this->operator>=(args)) && ...); }
};




template<typename A, typename B = A>
concept IsFullyComparable = requires (A a, B b) {
    { a == b } -> IsSame<bool>;
    { a != b } -> IsSame<bool>;
    { a < b } -> IsSame<bool>;
    { a <= b } -> IsSame<bool>;
    { a > b } -> IsSame<bool>;
    { a >= b } -> IsSame<bool>;
};




template<typename A>
concept HasCompareMemberFunction = requires (A a1, A a2) {
    { a1.compare(a2) } -> IsSame<int>;
};







constexpr int Compare(auto const& a, auto const& b)
{
    if constexpr (HasCompareMemberFunction<decltype(a)>) {

        return a.compare(b);
    } else {
        if (a == b) {
            return 0;
        } else if (a < b) {
            return -1;
        } else {
            return 1;
        }
    }
}







constexpr int ComparePointed(auto const& a, auto const& b)
{
    if constexpr (IsPointer<decltype(a)> || IsPointer<decltype(b)>) {
        return Compare(*a, *b);
    } else {
        return Compare(a, b);
    }
}

}
# 36 "./include/commons/core.hh" 2
# 1 "./include/commons/core/generator.hh" 1
# 20 "./include/commons/core/generator.hh"
namespace cm {

template<typename T>
struct Generator
{};


template<typename Collection, typename CollectionFilterIterator, typename Predicate>
struct FilterGenerator
{
    Collection const* _collection;
    Predicate const* _predicate;

    struct FilterIterator
    {
        FilterGenerator const* _filter;
        CollectionFilterIterator _curr;

        constexpr inline FilterIterator(FilterGenerator const* filter, bool is_begin)
            : _filter(filter), _curr(is_begin ? _filter->_collection->begin() : _filter->_collection->end())
        {

            while (_curr != _filter->_collection->end() && !(*_filter->_predicate)(*_curr)) {
#pragma GCC diagnostic push
# 43 "./include/commons/core/generator.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 43 "./include/commons/core/generator.hh"
                            ;
                ++_curr;
#pragma GCC diagnostic pop
# 45 "./include/commons/core/generator.hh"
                          ;
            }
        }

        constexpr ~FilterIterator() = default;
        constexpr FilterIterator(FilterIterator const&) = default;


        constexpr inline bool operator==(FilterIterator const& other) const
        {
            if (this == &other) {
                return true;
            }
            return this->_curr == other._curr;
        }

        constexpr inline FilterIterator& operator++()
        {

            while (_curr != _filter->_collection->end() && !(*_filter->_predicate)(*_curr)) {
#pragma GCC diagnostic push
# 65 "./include/commons/core/generator.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 65 "./include/commons/core/generator.hh"
                            ;
                ++_curr;
#pragma GCC diagnostic pop
# 67 "./include/commons/core/generator.hh"
                          ;
            }
            return *this;
        }

        constexpr inline auto& operator*() const { return *_curr; }
    };

    constexpr inline FilterIterator begin() const { return FilterIterator(this, true); }
    constexpr inline FilterIterator end() const { return FilterIterator(this, false); }
    constexpr inline FilterGenerator(Collection const* collection, Predicate const* predicate)
        : _collection(collection), _predicate(predicate)
    {}
};


constexpr auto Filter(auto const& iterable, auto const& predicate)
{
    return FilterGenerator<
        CVRefRemoved<decltype(iterable)>, decltype(iterable.begin()), CVRefRemoved<decltype(predicate)>>(
        &iterable, &predicate);
}


}
# 37 "./include/commons/core.hh" 2
# 1 "./include/commons/core/initializer_list.hh" 1
# 20 "./include/commons/core/initializer_list.hh"
namespace std {




template<class E>
class initializer_list {
public:
    typedef E value_type;
    typedef E const& reference;
    typedef E const& const_reference;
    typedef long unsigned int usizeype;
    typedef E const* iterator;
    typedef E const* const_iterator;

private:
    E const* _array;
    long unsigned int _len;


    constexpr initializer_list(E const* a, long unsigned int l)
        : _array(a), _len(l)
    {}

public:
    constexpr initializer_list() noexcept
        : _array(nullptr), _len(0)
    {}


    constexpr auto size() const noexcept { return _len; }


    constexpr E const* begin() const noexcept { return _array; }


    constexpr E const* end() const noexcept { return
# 56 "./include/commons/core/initializer_list.hh"
#pragma GCC diagnostic push
# 56 "./include/commons/core/initializer_list.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 56 "./include/commons/core/initializer_list.hh"
                                                     begin() + size()
# 56 "./include/commons/core/initializer_list.hh"
#pragma GCC diagnostic pop
# 56 "./include/commons/core/initializer_list.hh"
                                                                             ; }
};





template<class T>
constexpr T const* begin(initializer_list<T> a) noexcept
{
    return a.begin();
}





template<class T>
constexpr T const* end(initializer_list<T> a) noexcept
{
    return a.end();
}

}


namespace cm {
# 106 "./include/commons/core/initializer_list.hh"
template<typename T>
struct RefWrapper
{

    [[gnu::always_inline]] inline constexpr RefWrapper(T& x) : ptr(&x) {}
    [[gnu::always_inline]] inline constexpr operator T&() { return *ptr; }
    [[gnu::always_inline]] inline constexpr operator T const&() const { return *ptr; }
    [[gnu::always_inline]] inline constexpr T* operator->() { return ptr; }
    [[gnu::always_inline]] inline constexpr T const* operator->() const { return ptr; }
    [[gnu::always_inline]] inline constexpr RefWrapper& operator=(T& ref) { ptr = &ref; return *this; }



    [[gnu::always_inline]] inline constexpr T operator+(auto& val) const requires (IsAddable<T, decltype(val)>) { return *ptr + val; }
    [[gnu::always_inline]] inline constexpr T operator-(auto& val) const requires (IsSubtractable<T, decltype(val)>) { return *ptr - val; }
    [[gnu::always_inline]] inline constexpr T operator*(auto& val) const requires (IsMultipliable<T, decltype(val)>) { return *ptr * val; }
    [[gnu::always_inline]] inline constexpr T operator/(auto& val) const requires (IsDivideable<T, decltype(val)>) { return *ptr / val; }
    [[gnu::always_inline]] inline constexpr T operator%(auto& val) const requires (IsMODAble<T, decltype(val)>) { return *ptr % val; }
    [[gnu::always_inline]] inline constexpr T operator&(auto& val) const requires (IsANDAble<T, decltype(val)>) { return *ptr & val; }
    [[gnu::always_inline]] inline constexpr T operator|(auto& val) const requires (IsORAble<T, decltype(val)>) { return *ptr | val; }
    [[gnu::always_inline]] inline constexpr T operator^(auto& val) const requires (IsXORAble<T, decltype(val)>) { return *ptr ^ val; }

    [[gnu::always_inline]] inline constexpr auto& operator++() requires (!IsConst<T> && IsPrefixIncrementable<T>) { ++(*ptr); return *this; }
    [[gnu::always_inline]] inline constexpr T operator++(int) requires (!IsConst<T> && IsPostfixIncrementable<T>) { T tmp = *ptr; ++(*ptr); return move(tmp); }
    [[gnu::always_inline]] inline constexpr auto& operator--() requires (!IsConst<T> && IsPrefixDecrementable<T>) { --(*ptr); return *this; }
    [[gnu::always_inline]] inline constexpr T operator--(int) requires (!IsConst<T> && IsPostfixDecrementable<T>) { T tmp = *ptr; --(*ptr); return move(tmp); }

    [[gnu::always_inline]] inline constexpr auto& operator+=(auto& val) requires (!IsConst<T> && IsAddable<T, decltype(val)>) { *ptr += val; return *this; }
    [[gnu::always_inline]] inline constexpr auto& operator-=(auto& val) requires (!IsConst<T> && IsSubtractable<T, decltype(val)>) { *ptr - val; return *this; }
    [[gnu::always_inline]] inline constexpr auto& operator*=(auto& val) requires (!IsConst<T> && IsMultipliable<T, decltype(val)>) { *ptr * val; return *this; }
    [[gnu::always_inline]] inline constexpr auto& operator/=(auto& val) requires (!IsConst<T> && IsDivideable<T, decltype(val)>) { *ptr /= val; return *this; }
    [[gnu::always_inline]] inline constexpr auto& operator%=(auto& val) requires (!IsConst<T> && IsMODAble<T, decltype(val)>) { *ptr %= val; return *this; }
    [[gnu::always_inline]] inline constexpr auto& operator&=(auto& val) requires (!IsConst<T> && IsANDAble<T, decltype(val)>) { *ptr &= val; return *this; }
    [[gnu::always_inline]] inline constexpr auto& operator|=(auto& val) requires (!IsConst<T> && IsORAble<T, decltype(val)>) { *ptr |= val; return *this; }
    [[gnu::always_inline]] inline constexpr auto& operator^=(auto& val) requires (!IsConst<T> && IsXORAble<T, decltype(val)>) { *ptr ^= val; return *this; }


    [[gnu::always_inline]] inline constexpr static void outputString(RefWrapper const& self, auto const& out) {
        OutputString(*self.ptr, out);
    }
private:
    T* ptr;
};



template<typename T>
struct TWrapIfReference { using Type = T; };
template<typename T>
struct TWrapIfReference<T&> { using Type = RefWrapper<T>; };
template<typename T>
struct TWrapIfReference<T volatile&> { using Type = RefWrapper<T volatile>; };
template<typename T>
struct TWrapIfReference<T const&> { using Type = RefWrapper<T const>; };
template<typename T>
struct TWrapIfReference<T const volatile&> { using Type = RefWrapper<T const volatile>; };




template<typename T>
using WrapIfReference = SelectType<IsReference<T>, T, typename TWrapIfReference<T>::Type>;





template<typename T, unsigned N>
struct CVArray
{
    T _data[N];

    constexpr CVArray() = default;

    constexpr CVArray(auto... values) requires (sizeof...(values) <= N)
        : _data{static_cast<T>(values)...}
    {}

    constexpr CVArray(T const (&values)[N])
        : _data(values)
    {}

    [[gnu::always_inline]] inline constexpr T& operator[](auto index)
    {
#pragma GCC diagnostic push
# 190 "./include/commons/core/initializer_list.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 190 "./include/commons/core/initializer_list.hh"
        { return _data[index]; }
# 190 "./include/commons/core/initializer_list.hh"
#pragma GCC diagnostic pop
# 190 "./include/commons/core/initializer_list.hh"
                                        ;
    }

    [[gnu::always_inline]] inline constexpr T const& operator[](auto index) const
    {
#pragma GCC diagnostic push
# 195 "./include/commons/core/initializer_list.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 195 "./include/commons/core/initializer_list.hh"
        { return _data[index]; }
# 195 "./include/commons/core/initializer_list.hh"
#pragma GCC diagnostic pop
# 195 "./include/commons/core/initializer_list.hh"
                                        ;
    }
};

template<typename T, unsigned N>
CVArray(T const (&)[N]) -> CVArray<T, N>;


namespace CArrays {
# 222 "./include/commons/core/initializer_list.hh"
#pragma GCC diagnostic push
# 222 "./include/commons/core/initializer_list.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
template<typename T>
constexpr void moveDataToNewRegion(T* dst, T const* src, usize n)
{
    if consteval {
        for (long unsigned int i = 0; i < n; i++) {
            new (dst + i) T(src[i]);
        }
    } else {
        if constexpr (TriviallyCopyConstructible<T> || IsDerivedFrom<SafeBinaryMoveable, T>) {
            __builtin_memmove(dst, src, sizeof(T) * n);
        } else {
            for (long unsigned int i = 0; i < n; i++) {
                new (dst + i) T(src[i]);
            }
        }
    }
}

template<typename T>
constexpr bool equal(T const* a, T const* b, usize n)
{
    if consteval {
        for (usize i = 0; i < n; i++) {
            if (a[i] != b[i]) {
                return false;
            }
        }
    } else {
        if constexpr (IsPrimitiveData<T>) {
            return memcmp(a, b, n * sizeof(T)) == 0;
        } else {
            for (usize i = 0; i < n; i++) {
                if (a[i] != b[i]) {
                    return false;
                }
            }
        }
    }
    return true;
}

template<typename T>
constexpr void DefaultInitialize(T const* a, usize n)
{
    if consteval {
        for (usize i = 0; i < n; i++) {
            a[i] = T{};
        }
    } else {
        if constexpr (IsPrimitiveData<T>) {
            memset(a, 0, sizeof(T) * n);
        } else {
            for (auto i = 0uz; i < n; i++) {
                a[i] = T{};
            }
        }
    }
}

template<typename T>
constexpr auto stringLen(T const* str)
{
    if consteval {
        auto len = 0uz;
        while (*str++ != T{}) {
            len++;
        }
        return len;
    } else {
        if constexpr (IsUnderlyingTypeOneOf<T, char*, char8_t*>) {
            return strlen(str);
        } else {
            auto len = 0uz;
            while (*str++ != T{}) {
                len++;
            }
            return len;
        }
    }
}

constexpr auto For(Range<usize> const& range, auto* ptr, auto func)
{
    for (auto i : range) {
        func(ptr[i]);
    }
}

}

#pragma GCC diagnostic pop

}
# 38 "./include/commons/core.hh" 2
# 1 "./include/commons/core/tuple.hh" 1
# 22 "./include/commons/core/tuple.hh"
template<typename T, T... Index>
struct IntegerSequence
{
    constexpr static auto size() { return sizeof...(Index); }
};

template<typename T, auto N>
using MakeIntegerSequence = __make_integer_seq<IntegerSequence, T, N>;
# 62 "./include/commons/core/tuple.hh"
template<unsigned I, typename Item>
struct TupleLeaf
{
    Item value;
};

template<unsigned I, typename... Items>
struct TupleImpl;

template<unsigned I>
struct TupleImpl<I>
{};


template<unsigned I, typename H, typename... T>
struct TupleImpl<I, H, T...> : public TupleLeaf<I, H>, public TupleImpl<I + 1, T...>
{

protected:
    constexpr TupleImpl(H h, T... t)
        : TupleLeaf<I, H>{h}, TupleImpl<I + 1, T...>{t...}
    {}
};




template<typename... Args>
struct Tuple : TupleImpl<0, Args...>
{




    constexpr Tuple(auto&&... args)
        : TupleImpl<0, Args...>{Forward<decltype(args)>(args)...}
    {}




    constexpr Tuple(Tuple const&) = default;




    consteval auto size() const noexcept { return sizeof...(Args); }




    template<unsigned I>
    requires (I < sizeof...(Args))
    constexpr auto const& get() const noexcept
    {
        return get_<0, I, Args...>();
    }




    template<Constructible<Args...> T>
    constexpr inline T construct() const noexcept
    {
        if constexpr (TriviallyDefaultConstructible<T> && sizeof(*this) == sizeof(T)) {
            return __builtin_bit_cast(T, *this);
        } else {
            return construct_<T>(MakeIntegerSequence<unsigned, sizeof...(Args)>{});
        }
    }

private:
    template<unsigned I, unsigned N, typename H_, typename... T_>
    constexpr auto const& get_() const noexcept
    {
        if constexpr (I != N) {
            return get_<I + 1, N, T_...>();
        } else {
            return static_cast<TupleImpl<I, H_, T_...> const&>(*this).TupleLeaf<I, H_>::value;
        }
    }

    template<Constructible<Args...> T, unsigned... Indices>
    constexpr inline T construct_(IntegerSequence<unsigned, Indices...>) const noexcept
    {
        return T(get<Indices>()...);
    }

public:
    template<unsigned I, unsigned N, typename H, typename... T>
    using GetValueType = decltype(Tuple<Args...>::template TupleImpl<I, H, T...>::template TupleLeaf<I, H>::value);

    template<unsigned I, unsigned N, typename H, typename... T>
    struct GetType;

    template<unsigned I, unsigned N, typename H, typename... T>
    requires (I == N)
    struct GetType<I, N, H, T...>
    {
        using Type = GetValueType<I, N, H, T...>;
    };

    template<unsigned I, unsigned N, typename H, typename... T>
    requires (I != N)
    struct GetType<I, N, H, T...>
    {
        using Type = GetType<I + 1, N, T...>::Type;
    };

    template<unsigned N>
    requires (N < sizeof...(Args))
    using Element = GetType<0, N, Args...>::Type;
};

template<typename TupleT, unsigned N>
struct GetTupleElement
{
    using Type = typename TupleT::template Element<N>;
};

template<typename TupleT, unsigned N>
using TupleElement = typename GetTupleElement<TupleT, N>::Type;


namespace std {

template<typename>
struct tuple_size;

template<long unsigned int, typename>
struct tuple_element;


template<typename... Args>
struct tuple_size<::Tuple<Args...>>
{
    constexpr static auto value = sizeof...(Args);
};

template<decltype(0uz) Index, typename... Args>
struct tuple_element<Index, ::Tuple<Args...>>
{
    using type = TupleElement<::Tuple<Args...>, Index> const;
};
}
# 39 "./include/commons/core.hh" 2
# 1 "./include/commons/core/reflection_function.hh" 1
# 23 "./include/commons/core/reflection_function.hh"
template<typename FnType>
struct FunctionTraits
{
private:
    template<typename Fn_>
    struct Trait : Trait<decltype(&Fn_::operator())>
    {};

    template<typename ClassType, typename ReturnType, typename... Args>
    struct Trait<ReturnType (ClassType::*)(Args...)> : public Trait<ReturnType(Args...)>
    {
        enum : bool {
            eNoexcept = false,
            eInstanceFunction = true,
        };
    };

    template<typename ClassType, typename ReturnType, typename... Args>
    struct Trait<ReturnType (ClassType::*)(Args...) const> : public Trait<ReturnType(Args...)>
    {
        enum : bool {
            eNoexcept = false,
            eInstanceFunction = true,
            eConstInstanceFunction = true
        };
    };

    template<typename ClassType, typename ReturnType, typename... Args>
    struct Trait<ReturnType (ClassType::*)(Args...) noexcept> : public Trait<ReturnType(Args...)>
    {
        enum : bool {
            eNoexcept = true,
            eInstanceFunction = true,
            eConstInstanceFunction = false
        };
    };

    template<typename ClassType, typename ReturnType, typename... Args>
    struct Trait<ReturnType (ClassType::*)(Args...) const noexcept> : public Trait<ReturnType(Args...)>
    {
        enum : bool {
            eNoexcept = true,
            eInstanceFunction = true,
            eConstInstanceFunction = true
        };
    };

    template<typename ReturnType, typename... Args>
    struct Trait<ReturnType (*)(Args...)> : public Trait<ReturnType(Args...)>
    {
        enum : bool {
            eNoexcept = false,
            eInstanceFunction = false,
            eConstInstanceFunction = false
        };
    };

    template<typename ReturnType, typename... Args>
    struct Trait<ReturnType (*)(Args...) noexcept> : public Trait<ReturnType(Args...)>
    {
        enum : bool {
            eNoexcept = true,
            eInstanceFunction = false,
            eConstInstanceFunction = false
        };
    };

    template<typename ReturnType_, typename... Args>
    struct Trait<ReturnType_(Args...)>
    {
        enum {
            eNumArgs = sizeof...(Args)
        };

        using ReturnType = ReturnType_;

        template<auto N>
        struct Arg
        {
            static_assert(N >= 0 && N < eNumArgs, "Argument N does not exist");

            using Type = TupleElement<Tuple<Args...>, N>;
            constexpr static auto cSizeBytes = sizeof(Type);
        };
    };

public:
    constexpr static bool cNoexcept = Trait<FnType>::eNoexcept;
    constexpr static bool cMemberFunction = Trait<FnType>::eInstanceFunction;
    constexpr static bool cConstMemberFunction = Trait<FnType>::eConstInstanceFunction;
    constexpr static auto cNumArgs = Trait<FnType>::eNumArgs;

    using ReturnType = Trait<FnType>::ReturnType;

    template<auto N>
    using Arg = Trait<FnType>::template Arg<N>;
};
# 40 "./include/commons/core.hh" 2
# 1 "./include/commons/core/function.hh" 1
# 20 "./include/commons/core/function.hh"
namespace cm {


template<typename>
class CFunction;






template<typename ReturnType_, typename... Args>
class CFunction<ReturnType_(Args...)> : IEquatable<CFunction<ReturnType_(Args...)>> {
public:
    using ReturnType = ReturnType_;
    using PtrType = ReturnType (*)(Args...);

    constexpr inline CFunction(CFunction const&) = default;
    constexpr inline CFunction& operator=(CFunction const&) = default;
    constexpr inline CFunction(CFunction&&) = default;
    constexpr inline CFunction& operator=(CFunction&&) = default;

    constexpr inline CFunction()
        : _func(nullptr)
    {}

    constexpr inline CFunction(ReturnType (*funcPtr)(Args...))
        : _func(funcPtr)
    {}

    constexpr inline CFunction& operator=(ReturnType (*funcPtr)(Args...))
    {
        _func = funcPtr;
        return *this;
    }

    constexpr inline bool equals(CFunction const& other) const { return _func == other._func; }
    constexpr inline bool equals(ReturnType (*funcPtr)(Args...)) const { return _func == funcPtr; }

    constexpr inline ReturnType operator()(Args... args) const { return _func(args...); }
    constexpr inline operator bool() const { return _func != nullptr; }
    constexpr inline operator PtrType() const { return _func; }

private:
    ReturnType (*_func)(Args...);
};


template<typename>
struct Function;





template<typename ReturnType, typename... Args>
struct Function<ReturnType(Args...)>
{
private:
    struct Callable
    {
        virtual ~Callable() = default;
        virtual ReturnType invoke(Args...) = 0;
        decltype(0uz) refcount = 1uz;
    };

    template<typename T>
    class CallableT : public Callable {
        T t_;

    public:
        CallableT(T const& t)
            : t_(t)
        {}
        ~CallableT() override = default;
        ReturnType invoke(Args... args) override { return t_(args...); }
    };

    Callable* _callable;

public:
    Function(Function const&) = delete;
    Function& operator=(Function const&) = delete;


    template<typename T>
    inline Function(T&& t) requires (IsCallableAndReturns<T, ReturnType, Args...>)
        : _callable(new CallableT<T>(t))
    {}

    constexpr inline Function(Function&& other)
        : _callable(other._callable)
    {
        other._callable = nullptr;
    }

    inline ~Function()
    {
        if (_callable) {
            delete _callable;
        }
    }







    ReturnType operator()(Args... args) const { return _callable->invoke(args...); }
};





struct Functions
{



    template<long N>
    struct NthIdentityT
    {
    private:



        template<long I>
        constexpr static auto __nth_ident(auto const& val, auto const&... args)
        {
            if constexpr (I == N) {
                return val;
            } else {
                return __nth_ident<I + 1>(args...);
            }
        }

        template<long I>
        constexpr static auto __nth_ident(auto const& val)
        {
            static_assert(I == N, "Bad");
            return val;
        }

    public:
        constexpr static auto operator()(auto const&... args) requires ((N - 1) <= long(sizeof...(args)))
        {
            return __nth_ident<0>(args...);
        }
    };
    template<long N>
    constexpr static NthIdentityT<N> identity = {};


    template<typename T>
    struct Cast
    {
# 187 "./include/commons/core/function.hh"
        template<long N>
        struct FromNthT
        {
            constexpr T operator()(auto const&... args) const
                requires ((N - 1) <= long(sizeof...(args)) && (__is_convertible(decltype(args), T) && ...))
            {
                long i = 0;
                T result = {};
                (
                    [&]() {
                        if (i >= N) {
                            result = T(args);
                        }
                        i++;
                    }(),
                    ...);
                return result;
            }
        };
        template<unsigned N>
        constexpr static FromNthT<N> FromNth = {};
    };
};

constexpr inline auto NotNull = [](auto* ptr) -> bool {
    return ptr != nullptr;
};


}
# 41 "./include/commons/core.hh" 2
# 1 "./include/commons/core/intbase.hh" 1
# 27 "./include/commons/core/intbase.hh"
template<typename Dst>
constexpr inline auto bit_cast(auto src) noexcept
{
    static_assert(sizeof(Dst) == sizeof(decltype(src)), "bit_cast must be performed on types of equal size");
    return __builtin_bit_cast(Dst, src);
}


namespace cm {
namespace impl {
template<unsigned long long N>
consteval auto GetUintRanged()
{
    if constexpr (N > 281474976710656ull) {
        return u64{};
    } else if constexpr (N > 4294967296ull) {
        return u48{};
    } else if constexpr (N > 16777216ull) {
        return u32{};
    } else if constexpr (N > 65536ull) {
        return u24{};
    } else if constexpr (N > 256ull) {
        return u16{};
    } else if constexpr (N > 16ull) {
        return u8{};
    } else if constexpr (N > 8ull) {
        return u4{};
    } else if constexpr (N > 4ull) {
        return u3{};
    } else if constexpr (N > 2ull) {
        return u2{};
    } else {
        return u1{};
    }
}

template<unsigned long long N>
consteval auto GetIntRanged()
{
    if constexpr (N > 281474976710656ull) {
        return i64{};
    } else if constexpr (N > 4294967296ull) {
        return i48{};
    } else if constexpr (N > 16777216ull) {
        return i32{};
    } else if constexpr (N > 65536ull) {
        return i24{};
    } else if constexpr (N > 256ull) {
        return i16{};
    } else if constexpr (N > 16ull) {
        return i8{};
    } else if constexpr (N > 8ull) {
        return i4{};
    } else if constexpr (N > 4ull) {
        return i3{};
    } else {
        return i2{};
    }
}

template<unsigned Bits>
consteval auto GetUintN()
{
    if constexpr (Bits == 256) {
        return u256{};
    } else if constexpr (Bits == 128) {
        return u128{};
    } else if constexpr (Bits == 64) {
        return u64{};
    } else if constexpr (Bits == 32) {
        return u32{};
    } else if constexpr (Bits == 16) {
        return u16{};
    } else if constexpr (Bits == 8) {
        return u8{};
    } else {
        return static_cast<unsigned _BitInt(Bits)>(0);
    }
}

template<unsigned Bits>
consteval auto GetIntN()
{
    if constexpr (Bits == 256) {
        return i256{};
    } else if constexpr (Bits == 128) {
        return i128{};
    } else if constexpr (Bits == 64) {
        return i64{};
    } else if constexpr (Bits == 32) {
        return i32{};
    } else if constexpr (Bits == 16) {
        return i16{};
    } else if constexpr (Bits == 8) {
        return i8{};
    } else {
        return static_cast<_BitInt(Bits)>(0);
    }
}

}





template<unsigned long long N>
using UintRanged = decltype(impl::GetUintRanged<N>());




template<unsigned long long N>
using IntRanged = decltype(impl::GetIntRanged<N>());





template<unsigned Bits>
using UintN = decltype(impl::GetUintN<Bits>());





template<unsigned Bits>
using IntN = decltype(impl::GetIntN<Bits>());




constexpr auto operator""_KB(unsigned long long _) { return 1024ull * _; }




constexpr auto operator""_MB(unsigned long long _) { return 1024ULL * 1024ULL * _; }

using uint = unsigned int;




template<typename T>
constexpr inline usize SizeOf = usize(sizeof(T));



template<typename T, unsigned int Size>
using Vector = __attribute__((__vector_size__(Size * sizeof(T)))) T;







template<unsigned Size>
using VectorU8x = Vector<u8, Size>;
template<unsigned Size>
using VectorU32x = Vector<u32, Size>;

static_assert(sizeof(Vector<int, 4>) == 16, "");







constexpr decltype(auto) max(auto const& a) { return a; }


constexpr decltype(auto) max(auto const& a, auto const& b) { return a > b ? a : b; }


constexpr decltype(auto) max(auto const& a, auto const& b, auto const&... args) { return max(a, max(b, args...)); }







constexpr decltype(auto) min(auto const& a) { return a; }


constexpr decltype(auto) min(auto const& a, auto const& b) { return a < b ? a : b; }


constexpr decltype(auto) min(auto const& a, auto const& b, auto const&... args)
{



    return min(a, min(b, args...));
}


static_assert(max(-1, 2, 3, -11, 5) == 5);

static_assert(min(-1, 2, 3, -11, 5) == -11);
static_assert(min(0, -999999999999LL) == -999999999999LL);







using int8_t = signed char;
using uint8_t = unsigned char;
using int16_t = short;
using uint16_t = unsigned short;
using int32_t = int;
using uint32_t = unsigned int;
using int64_t = long int;
using uint64_t = long unsigned int;
using size_t = long unsigned int;
using ssize_t = long int;
# 507 "./include/commons/core/intbase.hh"
}
# 42 "./include/commons/core.hh" 2
# 1 "./include/commons/core/assert.hh" 1
# 20 "./include/commons/core/assert.hh"
namespace cm {





constexpr auto
    ASMS_DATA_CORRUPTION = "Data corruption",
    ASMS_DATA_CORRUPTION2 = "Data corruption (Invalid checksum)",
    ASMS_BUG = "Bug",
    ASMS_NO_MEMORY = "Out of memory",
    ASMS_BAD_CIRCUMSTANCE = "Function called under the wrong circumstances",
    ASMS_PARAMETER = "Invalid parameter",
    ASMS_BOUNDS = "Index out of bounds";
# 42 "./include/commons/core/assert.hh"
struct SourceLocation
{

    decltype(__builtin_LINE()) line_;
    decltype(__builtin_COLUMN()) column_;
    decltype(__builtin_FILE()) file_;


    decltype(__builtin_FUNCTION()) function_;




public:
    constexpr inline __attribute__((noinline)) SourceLocation() noexcept = default;

    [[gnu::always_inline]] inline constexpr SourceLocation(unsigned line, unsigned column, char const* file) noexcept
        : line_(line), column_(column), file_(file), function_("")
    {}

    [[gnu::always_inline]] inline constexpr SourceLocation(
        unsigned line, unsigned column, char const* file, char const* function) noexcept
        : line_(line), column_(column), file_(file), function_(function)
    {}

    [[gnu::always_inline]] inline static consteval SourceLocation current(
        unsigned line = __builtin_LINE(), unsigned column = __builtin_COLUMN(), char const* file = __builtin_FILE(),
        char const* function = __builtin_FUNCTION()) noexcept
    {
        return SourceLocation{line, column, file, function};
    }

    [[gnu::always_inline]] inline constexpr auto line() const noexcept { return line_; }
    [[gnu::always_inline]] inline constexpr auto column() const noexcept { return column_; }
    [[gnu::always_inline]] inline constexpr auto file() const noexcept { return file_; }
    [[gnu::always_inline]] inline constexpr auto function() const noexcept { return function_; }
};





[[noreturn]]
void panic(char const* message, char const* reason, SourceLocation src);




constexpr void Assert(bool CONDITION, char const* message = "", SourceLocation src = SourceLocation::current())
{
    (void)message;
    if consteval {
        if (!(CONDITION))
            panic("Assertion failed", message, src);
    } else {
        if (!(CONDITION))
            panic("Assertion failed", message, src);
    }
}

}
# 43 "./include/commons/core.hh" 2
# 1 "./include/commons/core/property.hh" 1
# 20 "./include/commons/core/property.hh"
namespace cm {

template<typename Container_>
struct ComputedProperty
{
protected:
    friend Container_;

    ComputedProperty() = default;

    template<typename T1>
    inline auto findOffset(T1 Container_::* member) const noexcept
    {
        signed char data[sizeof(Container_)];
        Container_ const& object = *reinterpret_cast<Container_*>(data);
        return reinterpret_cast<long long>(&(object.*member)) - reinterpret_cast<long long>(&object);
    }

    template<typename T1>
    inline Container_* container(T1 Container_::* member) noexcept
    {
        return reinterpret_cast<Container_*>(reinterpret_cast<long long>(this) - findOffset(member));
    }

    template<typename T1>
    inline Container_ const* container(T1 Container_::* member) const noexcept
    {
        return reinterpret_cast<Container_ const*>(reinterpret_cast<long long>(this) - findOffset(member));
    }
};
}
# 44 "./include/commons/core.hh" 2
# 1 "./include/commons/core/pointer.hh" 1
# 25 "./include/commons/core/pointer.hh"
inline static bool __ptr_is_rodata(void const* address)
{

    extern char const etext, edata, end;
    return address >= &etext && address < &end;




}

namespace cm {


enum Access : u8 {
    READ_BIT = 1,
    WRITE_BIT = 1 << 1,
    EXECUTE_BIT = 1 << 2,
    READ_WRITE_BITS = READ_BIT | WRITE_BIT
};


struct Ptr
{
    Ptr(void* base, usize n_bytes);







    inline static bool isRomData(void const* address) { return __ptr_is_rodata(address); }


    static u8 leastPermissiveAccess(u8 access0, u8 access1) noexcept { return access0 & access1; }

    inline static bool canRead(void* base, usize n_bytes) noexcept
    {
        return getAccessBits(base, n_bytes) & Access::READ_BIT;
    }

    inline static bool canWrite(void* base, usize n_bytes) noexcept
    {
        return getAccessBits(base, n_bytes) & Access::WRITE_BIT;
    }

    inline static bool canReadWrite(void* base, usize n_bytes) noexcept
    {
        return getAccessBits(base, n_bytes) & Access::READ_WRITE_BITS;
    }







    template<typename T>
#pragma GCC diagnostic push
# 84 "./include/commons/core/pointer.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 84 "./include/commons/core/pointer.hh"
    constexpr static T const* findSubstring(T const* str, T const* substring) { T const* a; T const* b = substring; if (*b == 0) return str; for (; *str != 0; str += 1) { if (*str != *b) { continue; } a = str; while (1) { if (*b == 0) { return str; } if (*a++ != *b++) { break; } } b = substring; } return nullptr; }
# 84 "./include/commons/core/pointer.hh"
#pragma GCC diagnostic pop
# 108 "./include/commons/core/pointer.hh"
      ;






    static u8 getAccessBits(void* base, usize n_bytes) noexcept
    {


        if (base == nullptr)
            return 0;

        (void)n_bytes;
        return Access::READ_WRITE_BITS;
# 168 "./include/commons/core/pointer.hh"
    }
};

}
# 45 "./include/commons/core.hh" 2
# 1 "./include/commons/core/class.hh" 1
# 21 "./include/commons/core/class.hh"
namespace cm {

class String;





typedef const class Class {
public:
    using IDType = u64;

    constexpr static bool PRINT_CLASS_IDS = true;
    char const* name;
    CFunction<void(void*)> destructor;
    CFunction<void(void*)> defaultConstructor;
    CFunction<void(void*, void const*)> copyConstructor;
    CFunction<void(void*, void const*)> copyAssignOperator;
    CFunction<void(void*, void*)> moveConstructor;
    CFunction<void(void*, void*)> moveAssignOperator;
    u32 sizeBytes;
    u32 isPrimitive : 1;
    u32 isSigned : 1;
    u32 isFloatingPoint : 1;
    u32 isInteger : 1;

    [[no_unique_address]] struct IDProperty : ComputedProperty<Class>
    {
        constexpr operator IDType() const
        {
            usize result = usize(size_t(this));
            return static_cast<IDType>(result);
        }
        String* toString() const;
    } id;

public:



    static Class const& fromID(IDType id);




    constexpr bool operator==(Class const& c) const { return name == c.name; }




    template<typename T>
    static consteval Class init(char const* name)
    {
        Class result{};
        result.name = name;
        result.sizeBytes = sizeof(T);
        result.isPrimitive = !IsClass<T>;
        result.isSigned = !IsClass<T> && !IsUnsignedInteger<T>;
        result.isFloatingPoint = IsFloatingPoint<T>;
        result.isInteger = IsInteger<T>;

        result.destructor = [](void* ptr) -> void {
            if constexpr (!TriviallyDestructible<T>) {
                reinterpret_cast<T*>(ptr)->~T();
            }
        };
        if constexpr (DefaultConstructible<T>) {
            result.defaultConstructor = [](void* ptr) -> void {
                new (ptr) T();
            };
        }
        if constexpr (CopyConstructible<T>) {
            result.copyConstructor = [](void* dst, void const* src) -> void {
                new (dst) T(*static_cast<T const*>(src));
            };
        }
        if constexpr (CopyAssignable<T>) {
            result.copyAssignOperator = [](void* left, void const* right) -> void {
                *(static_cast<T*>(left)) = (*static_cast<T const*>(right));
            };
        }
        if constexpr (MoveConstructible<T>) {
            result.moveConstructor = [](void* left, void* right) -> void {
                new (left) T(static_cast<T&&>(*static_cast<T*>(right)));
            };
        }
        if constexpr (MoveAssignable<T>) {
            result.moveAssignOperator = [](void* left, void* right) -> void {
                *(static_cast<T*>(left)) = static_cast<T&&>(*static_cast<T*>(right));
            };
        }
        return result;
    }


    template<unsigned... Idxs>
    struct NameBuffer
    {
        consteval NameBuffer(auto... args)
            : data{args...}
        {}
        char data[sizeof...(Idxs) + 1];
    };

    template<typename T>
    static consteval auto _getNameBuffer()
    {



        constexpr static char prefix[6] = "[T = ";
        constexpr static char suffix[2] = "]";
        constexpr static auto function = __PRETTY_FUNCTION__;
# 145 "./include/commons/core/class.hh"
#pragma clang diagnostic push
# 145 "./include/commons/core/class.hh"
                                        ;
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
# 146 "./include/commons/core/class.hh"
                                                                     ;
        auto toNameBuffer = []<size_t... Idxs>(char const* str, IntegerSequence<size_t, Idxs...>) consteval {
            return NameBuffer<Idxs...>(str[Idxs]..., '\0');
        };

        return toNameBuffer(
            (Ptr::findSubstring(function, prefix) + (sizeof(prefix) - 1)),
            MakeIntegerSequence<
                size_t, (Ptr::findSubstring(function, suffix) -
                         (Ptr::findSubstring(function, prefix) + (sizeof(prefix) - 1)))>{});
#pragma clang diagnostic pop
# 156 "./include/commons/core/class.hh"
                                       ;
    }
} *ClassPtr, &ClassRef;


template<typename T>
struct ClassData
{
    constexpr static auto nameBuffer = Class::_getNameBuffer<T>();
    constexpr static char const* name = nameBuffer.data;
    constexpr static auto class_ = Class::init<T>(name);
};

template<typename T>
constexpr Class const& ClassOf = ClassData<T>::class_;


}
# 46 "./include/commons/core.hh" 2
# 1 "./include/commons/core/union.hh" 1
# 32 "./include/commons/core/union.hh"
namespace cm {

namespace impl {

template<bool>
class UnionDestructorCallback {};

template<>
class UnionDestructorCallback<true> {
protected:
    CFunction<void(void*)> _dtor;
};

template<bool>
class UnionCopyConstructorCallback {};

template<>
class UnionCopyConstructorCallback<true> {
protected:
    CFunction<void(void*, void const*)> _copyCtor;
};

template<bool>
class UnionCopyAssignmentCallback {};

template<>
class UnionCopyAssignmentCallback<true> {
protected:
    CFunction<void(void*, void const*)> _copyAssignOp;
};

template<bool>
class UnionMoveConstructorCallback {};

template<>
class UnionMoveConstructorCallback<true> {
protected:
    CFunction<void(void*, void const*)> _moveCtor;
};

template<bool>
class UnionMoveAssignmentCallback {};

template<>
class UnionMoveAssignmentCallback<true> {
protected:
    CFunction<void(void*, void const*)> _moveAssignOp;
};

template<typename... Types>
struct UnionExtraVariables
{
    constexpr static bool triviallyDestructible = ((TriviallyDestructible<Types>) && ...);
    constexpr static bool triviallyCopyConstructible = ((TriviallyCopyConstructible<Types>) && ...);
    constexpr static bool triviallyCopyAssignable = ((TriviallyCopyAssignable<Types>) && ...);
    constexpr static bool triviallyMoveConstructible = ((TriviallyMoveConstructible<Types>) && ...);
    constexpr static bool triviallyMoveAssignable = ((TriviallyMoveAssignable<Types>) && ...);
};






template<typename... Types>
consteval auto unionMetadata()
{
    using S = UnionExtraVariables<Types...>;

    if constexpr (
        S::triviallyDestructible && S::triviallyCopyConstructible && S::triviallyCopyAssignable &&
        S::triviallyMoveConstructible && S::triviallyMoveAssignable)
    {
        constexpr static S s;
        return s;
    } else {
        struct R : S
        {
            Class const* _class = nullptr;
        } static constexpr r;
        return r;
    }
}


}


template<typename... Types>
class Union : decltype(impl::unionMetadata<Types...>()) {
public:
    using Metadata = decltype(impl::unionMetadata<Types...>());
    static consteval bool hasMetadata() { return sizeof(Metadata) > 1; }
    static consteval bool hasNonTrivialDestructor() { return !Metadata::triviallyDestructible; }
    static consteval bool hasNonTrivialCopyConstructor() { return !Metadata::triviallyCopyConstructible; }
    static consteval bool hasNonTrivialCopyAssignment() { return !Metadata::triviallyCopyAssignable; }
    static consteval bool hasNonTrivialMoveConstructor() { return !Metadata::triviallyMoveConstructible; }
    static consteval bool hasNonTrivialMoveAssignment() { return !Metadata::triviallyMoveAssignable; }


    template<typename T, int I>
    struct InitSuccess
    {
        using WhichOne = T;
        constexpr static auto Tag = I;
    };
    struct InitFailure
    {};

private:

    UintRanged<sizeof...(Types)> _tag;
    alignas(max(alignof(Types)...)) u8 _data[max(sizeof(Types)...)];


    struct Strong
    {
        template<typename From, typename To>
        constexpr static bool match()
        {
            return IsSame<From, To>

                   || (IsFloatingPoint<From> && IsFloatingPoint<To> && sizeof(To) >= sizeof(From))

                   || (IsInteger<From> && IsInteger<To> && sizeof(To) >= sizeof(From) &&
                       ((IsIntegerSigned<From> && IsIntegerSigned<To>) ||
                        (!IsIntegerSigned<From> && !IsIntegerSigned<To>)));
        }
    };


    struct Medium
    {
        template<typename From, typename To>
        constexpr static bool match()
        {
            return (IsFloatingPoint<From> && IsFloatingPoint<To>) || (IsInteger<From> && IsInteger<To>);
        }
    };


    struct Weak
    {
        template<typename From, typename To>
        constexpr static bool match()
        {
            return ConvertibleTo<From, To>;
        }
    };

    template<typename matchFn, int I, typename U, typename... Tn>
    struct Initializer;

    template<typename matchFn, int I, typename From, typename T1, typename... Tn>
    struct Initializer<matchFn, I, From, T1, Tn...>
    {
        template<typename V>
        constexpr static auto match(void* data, V&& value)
        {
            static_assert(IsSame<CVRefRemoved<V>, CVRefRemoved<From>>);
            if constexpr (matchFn::template match<From, T1>()) {
                new (data) T1(Forward<V>(value));
                return InitSuccess<T1, I>{};
            } else {
                return Initializer<matchFn, I + 1, From, Tn...>::match(data, Forward<V>(value));
            }
        }
    };

    template<typename matchFn, int I, typename From, typename To>
    struct Initializer<matchFn, I, From, To>
    {
        template<typename V>
        constexpr static auto match(void* data, V&& value)
        {
            static_assert(IsSame<CVRefRemoved<V>, CVRefRemoved<From>>);
            if constexpr (matchFn::template match<From, To>()) {
#pragma GCC diagnostic push
# 209 "./include/commons/core/union.hh"
                                              ;
#pragma GCC diagnostic ignored "-Wsign-conversion"
# 210 "./include/commons/core/union.hh"
                                                                       ;
                new (data) To(Forward<V>(value));
#pragma GCC diagnostic pop
# 212 "./include/commons/core/union.hh"
                                             ;
                return InitSuccess<To, I>{};
            } else {
                return InitFailure{};
            }
        }
    };

    template<typename matchFn, int I, typename U>
    struct Initializer<matchFn, I, U>
    {
        template<typename V>
        constexpr static auto match(void*, V&&)
        {
            return InitFailure{};
        }
    };





    template<int I, typename U, typename... Tn>
    struct TryInit
    {
        template<typename V>
        constexpr static auto next(void* data, V&& value)
        {

            if constexpr (IsSame<
                              decltype(Initializer<Strong, I, U, Tn...>::match(data, Forward<V>(value))), InitFailure>)
            {

                if constexpr (IsSame<
                                  decltype(Initializer<Medium, I, U, Tn...>::match(data, Forward<V>(value))),
                                  InitFailure>)
                {

                    return Initializer<Weak, I, U, Tn...>::match(data, Forward<V>(value));
                } else {
                    return Initializer<Medium, I, U, Tn...>::match(data, Forward<V>(value));
                }
            } else {
                return Initializer<Strong, I, U, Tn...>::match(data, Forward<V>(value));
            }
        }
    };

    template<typename T>
    constexpr inline void storeCallbacksFor()
    {
        if constexpr (hasMetadata()) {
            Metadata::_class = &ClassOf<T>;
        }
    }

public:




    constexpr ~Union() requires (!hasNonTrivialDestructor())
    = default;
    constexpr ~Union() requires (hasNonTrivialDestructor())
    {
        if (Metadata::_class) {
            Metadata::_class->destructor(_data);
        }
    }




    constexpr inline Union(Union const& other) requires (!hasNonTrivialCopyConstructor())
    = default;
    constexpr inline Union(Union const& other) requires (hasNonTrivialCopyConstructor())
    {
        Metadata::_class = other._class;
        _tag = other._tag;
        Assert(Metadata::_class->copyConstructor, "Type in union not copy constructible");
        Metadata::_class->copyConstructor(_data, other._data);
    }




    constexpr inline Union& operator=(Union const& other)
        requires (!hasNonTrivialDestructor() && !hasNonTrivialCopyAssignment() && !hasNonTrivialCopyConstructor())
        = default;
    constexpr inline Union& operator=(Union const& other)
        requires (hasNonTrivialDestructor() || hasNonTrivialCopyAssignment() || hasNonTrivialCopyConstructor())
    {
        if (_tag == other._tag) {

            if constexpr (hasNonTrivialCopyAssignment()) {
                Assert(Metadata::_class->copyAssignOperator, "Type in union not copy assignable");
                Metadata::_class->copyAssignOperator(_data, other._data);
            } else {
                __builtin_memcpy_inline(_data, other._data, sizeof(_data));
            }
        } else {

            if constexpr (hasNonTrivialDestructor()) {
                Metadata::_class->destructor(_data);
            }
            Metadata::_class = other._class;
            if constexpr (hasNonTrivialCopyConstructor())
            {
                Assert(Metadata::_class->copyConstructor, "Type in union not copy constructible");
                Metadata::_class->copyConstructor(_data, other._data);
            } else {
                __builtin_memcpy_inline(_data, other._data, sizeof(_data));
            }
            _tag = other._tag;
        }
        return *this;
    }




    constexpr inline Union(Union&& other) requires (!hasNonTrivialMoveConstructor())
    = default;
    constexpr inline Union(Union&& other) requires (hasNonTrivialMoveConstructor())
    {
        Metadata::_class = other._class;
        _tag = other._tag;
        Assert(Metadata::_class->moveConstructor, "Type in union not move constructible");
        Metadata::_class->moveConstructor(_data, other._data);
    }




    constexpr inline Union& operator=(Union&& other)
        requires (!hasNonTrivialDestructor() && !hasNonTrivialMoveAssignment() && !hasNonTrivialMoveConstructor())
        = default;
    constexpr inline Union& operator=(Union&& other)
        requires (hasNonTrivialDestructor() || hasNonTrivialMoveAssignment() || hasNonTrivialMoveConstructor())
    {
        if (_tag == other._tag) {

            if constexpr (hasNonTrivialMoveAssignment()) {
                Assert(Metadata::_class->moveAssignOperator, "Type in union not move assignable");
                Metadata::_class->moveAssignOperator(_data, other._data);
            } else {
                __builtin_memcpy_inline(_data, other._data, sizeof(_data));
            }
        } else {
            if constexpr (hasNonTrivialDestructor()) {
                Metadata::_class->destructor(_data);
            }
            Metadata::_class = other._class;
            if constexpr (hasNonTrivialMoveConstructor()) {
                Assert(Metadata::_class->moveConstructor, "Type in union not move constructible");
                Metadata::_class->moveConstructor(_data, other._data);
            } else {
                __builtin_memcpy_inline(_data, other._data, sizeof(_data));
            }
            _tag = other._tag;
        }
        if constexpr (hasNonTrivialDestructor()) {
            other._class = nullptr;
        }
        return *this;
    }




    template<typename GivenType>
    constexpr Union(GivenType&& t1)

        requires (!IsSame<decltype(TryInit<0, GivenType, Types...>::next(_data, Forward<GivenType>(t1))), InitFailure>)
    {
        using Init = decltype(TryInit<0, GivenType, Types...>::next(_data, Forward<GivenType>(t1)));
        TryInit<0, GivenType, Types...>::next(_data, Forward<GivenType>(t1));
        _tag = static_cast<unsigned char>(Init::Tag);
        storeCallbacksFor<typename Init::WhichOne>();
    }




    template<typename GivenType>
    constexpr Union& operator=(GivenType&& t1)
    {
        using Init = decltype(TryInit<0, GivenType, Types...>::next(_data, Forward<GivenType>(t1)));
        auto newTag = static_cast<unsigned char>(Init::Tag);
        if (_tag != newTag) {
            if constexpr (hasNonTrivialDestructor()) {
                Metadata::_dtor(_data);
            }
            _tag = newTag;
            TryInit<0, GivenType, Types...>::next(_data, Forward<GivenType>(t1));
            storeCallbacksFor<typename Init::WhichOne>();
        }
        return *this;
    }




    template<typename T>
    constexpr inline bool is() const noexcept
    {
        if constexpr (CopyConstructible<T>) {
            auto const& k = (_ref<T>());
            using Init = decltype(TryInit<0, T, Types...>::next(const_cast<u8*>(_data), k));
            if constexpr (IsSame<Init, InitFailure>) {
                return false;
            } else {
                return _tag == Init::Tag;
            }
        } else if constexpr (MoveConstructible<T>) {
            using Init = decltype(TryInit<0, T, Types...>::template next<T&&>(
                const_cast<u8*>(_data), static_cast<T&&>(const_cast<T&>(_ref<T>()))));
            if constexpr (IsSame<Init, InitFailure>) {
                return false;
            } else {
                return _tag == Init::Tag;
            }
        } else {
            static_assert(false, "Unhandled edge case");
        }
    }




    template<typename T>
    [[nodiscard]] inline T const& _ref() const noexcept
    {
        if constexpr (IsReference<T>) {
            using U = RefRemoved<T>;
            T const& ref = **reinterpret_cast<U const* const*>(_data);
            return ref;
        } else {
            T const& ref = *reinterpret_cast<T const*>(_data);
            return ref;
        }
    }
    template<typename T>
    [[nodiscard]] inline T& _ref() noexcept
    {
        return const_cast<T&>(static_cast<Union const*>(this)->_ref<T>());
    }




    template<typename T>
    [[nodiscard]] inline T const& get() const noexcept
    {
        Assert(is<T>());
        return const_cast<ConstRemoved<decltype(_ref<T>())>>(_ref<T>());
    }
    template<typename T>
    [[nodiscard]] inline T& get() noexcept
    {
        return const_cast<T&>(static_cast<Union const*>(this)->get<T>());
    }





    template<typename T>
    [[nodiscard]] inline T const& getOrDefault(T const& defaultValue) const noexcept
    {
        if (is<T>()) {
            return const_cast<ConstRemoved<decltype(_ref<T>())>>(_ref<T>());
        } else {
            return defaultValue;
        }
    }




    [[nodiscard]] inline auto tag() const noexcept { return _tag; }






    inline auto match(auto func, auto... funcs) const
    {
        using T = FunctionTraits<decltype(func)>::template Arg<0>::Type;
        if (this->is<T>() || this->is<RefRemoved<T>>()) {
            return func(this->get<T>());
        } else {
            return this->match(funcs...);
        }
    }

    inline auto match(auto func) const
    {
        using T = FunctionTraits<decltype(func)>::template Arg<0>::Type;
        if (this->is<T>()) {
            return func(this->get<T>());
        } else {
            __builtin_trap();
            __builtin_unreachable();
        }
    }

    template<auto Default>
    inline auto matchOr(auto func, auto... funcs) const
    {
        using T = FunctionTraits<decltype(func)>::template Arg<0>::Type;
        if (this->is<T>()) {
            return func(this->get<T>());
        } else {
            return this->matchOr<Default>(funcs...);
        }
    }

    template<auto Default>
    inline auto matchOr(auto func) const
    {
        using T = FunctionTraits<decltype(func)>::template Arg<0>::Type;
        if (this->is<T>()) {
            return func(this->get<T>());
        } else {
            if constexpr (IsFunction<decltype(Default)> || IsClass<decltype(Default)>) {
                return Default();
            } else {
                return Default;
            }
        }
    }
};

}
# 47 "./include/commons/core.hh" 2
# 1 "./include/commons/core/result.hh" 1
# 21 "./include/commons/core/result.hh"
namespace cm {
struct ResultSuccessTag
{};

struct ResultErrorTag
{};
# 36 "./include/commons/core/result.hh"
template<typename SuccessType_, typename ErrorType_>
class Result {
public:
    using SuccessType = SuccessType_;
    using ErrorType = ErrorType_;

    template<typename... Args>
    requires (Constructible<SuccessType, Args...>)
    explicit constexpr Result(ResultSuccessTag, Args&&... args)
        : _u(SuccessWrapper{SuccessType(Forward<Args>(args)...)})
    {}

    template<typename... Args>
    requires (Constructible<ErrorType, Args...>)
    explicit constexpr Result(ResultErrorTag, Args&&... args)
        : _u(ErrorWrapper{ErrorType(Forward<Args>(args)...)})
    {}

    constexpr inline ~Result() = default;

    [[gnu::always_inline]] inline constexpr operator bool() const noexcept { return _u.template is<SuccessWrapper>(); }
    [[gnu::always_inline]] inline constexpr bool isOk() const noexcept { return _u.template is<SuccessWrapper>(); }
    [[gnu::always_inline]] inline constexpr bool isErr() const noexcept { return _u.template is<ErrorWrapper>(); }

    [[gnu::always_inline]] inline constexpr SuccessType const& unwrap() const noexcept
    {
        if (isErr()) {
            CPU.trap();
        }
        return _u.success;
    }

    [[gnu::always_inline]] inline constexpr auto then(auto func)
    {
        if (isOk()) {
            func(_u.success);
        }
    }

    [[gnu::always_inline]] inline constexpr auto then(auto func) const
    {
        if (isOk()) {
            func(_u.success);
        }
    }

private:


    struct SuccessWrapper
    {
        SuccessType success;
    };
    struct ErrorWrapper
    {
        ErrorType error;
    };
    Union<SuccessWrapper, ErrorWrapper> _u;
};



template<typename... Args>
struct Ok
{
    explicit constexpr Ok(Args... args)
        : args_(Forward<Args>(args)...)
    {}

    template<typename ResultType>
    constexpr operator ResultType() const
    {
        return ResultType(ResultSuccessTag{}, args_.template construct<typename ResultType::SuccessType>());
    }

private:
    Tuple<Args...> args_;
};

template<typename... Args>
Ok(Args&...) -> Ok<Args...>;
template<typename... Args>
Ok(Args&&...) -> Ok<Args...>;

template<>
struct Ok<>
{
    constexpr explicit Ok() = default;

    template<typename ResultType>
    constexpr operator ResultType() const
    {
        return ResultType(ResultSuccessTag{});
    }
};

template<typename... Args>
struct Err
{
public:
    explicit constexpr Err() = default;

    explicit constexpr Err(Args&... args)
        : args_(args...)
    {}

    explicit constexpr Err(Args&&... args)
        : args_(args...)
    {}

    template<typename ResultType>
    constexpr operator ResultType() const
    {
        return ResultType(ResultErrorTag{}, args_.template construct<typename ResultType::ErrorType>());
    }

private:
    Tuple<Args&...> args_;
};

template<typename... Args>
Err(Args&...) -> Err<Args...>;
template<typename... Args>
Err(Args&&...) -> Err<Args...>;

template<>
struct Err<>
{
    explicit constexpr Err() = default;

    template<typename ResultType>
    constexpr operator ResultType() const
    {
        return ResultType(ResultErrorTag{});
    }
};

}
# 48 "./include/commons/core.hh" 2
# 1 "./include/commons/core/optional.hh" 1
# 21 "./include/commons/core/optional.hh"
namespace cm {

struct NoneType
{};

constexpr auto None = NoneType{};




template<typename T>
class Optional : public Union<T, NoneType> {
    using Base = Union<T, NoneType>;

public:
    using Union<T, NoneType>::Union;




    constexpr Optional()
        : Base(NoneType{})
    {}

    constexpr Optional(T const& val)
        : Base(val)
    {}
    constexpr Optional(NoneType const&)
        : Base(NoneType{})
    {}




    constexpr inline operator bool() const noexcept { return Base::template is<T>(); }
    constexpr inline bool hasValue() const noexcept { return this->operator bool(); }
    constexpr inline bool operator==(NoneType const&) const noexcept { return !hasValue(); }




    constexpr inline T const& value() const { return Base::template get<T>(); }
    constexpr inline RefRemoved<T>* operator->() const noexcept { return const_cast<RefRemoved<T>*>(&this->value()); }





    template<class U>
    constexpr inline T valueOr(U&& x) const&
    {
        return this->hasValue() ? this->value() : static_cast<T>(Forward<U>(x));
    }

    constexpr static void outputString(Optional const& self, auto const& out)
    {
        if (self.hasValue()) {
            OutputString(self.value(), out);
        } else {
            out('N');
            out('o');
            out('n');
            out('e');
        }
    }
};

}
# 49 "./include/commons/core.hh" 2
# 1 "./include/commons/core/errors.hh" 1
# 21 "./include/commons/core/errors.hh"
namespace cm {




namespace Errors {
struct Overflow
{};
struct BadFormat
{};
};

struct UnknownException
{};

struct NumberOverflowException
{};

struct StackOverflowException
{};

struct Forbid
{};

struct InvalidArgumentException
{};

struct BadFormatException
{};

struct OutOfMemoryException
{};
# 71 "./include/commons/core/errors.hh"
}
# 50 "./include/commons/core.hh" 2
# 1 "./include/commons/core/profiler.hh" 1
# 20 "./include/commons/core/profiler.hh"
namespace cm {




class Profiler {
public:
    struct StackFrame
    {
        void const* funcAddr;
        void const* callAddr;
        long tElapsed = 0;
    };

    static void init();
    static void push(SourceLocation src);
    static void pop();
    static void printStackTrace();
    static StackFrame const& getCurrentStackFrame();
};






struct ProfiledScope
{
    inline ProfiledScope(SourceLocation src = SourceLocation::current()) { Profiler::push(src); }

    inline ~ProfiledScope() { Profiler::pop(); }
};


}
# 51 "./include/commons/core.hh" 2

# 1 "./include/commons/core/array_iterator.hh" 1
# 18 "./include/commons/core/array_iterator.hh"
namespace cm {





template<typename Container, typename T>
class LinearIterator {
public:



    constexpr auto next() const { return LinearIterator(_array, _index + 1); }




    constexpr auto prev() const { return LinearIterator(_array, _index - 1); }




    constexpr bool hasPrev() const { return (_index > 0) && (_index < _array->length()); }




    constexpr bool hasNext() const { return _index < usize(max(i64(_array->length()) - 1, 0)); }




    constexpr bool isNotEnd() const { return _index < _array->length(); }





    constexpr LinearIterator& operator++() { return (*this = this->next()); }





    constexpr LinearIterator& operator--() { return (*this = this->prev()); }




    constexpr T const& operator*() const noexcept { return _get(); }




    constexpr T& operator*() noexcept { return _get(); }




    constexpr T const* operator->() const noexcept { return &_get(); }




    constexpr T* operator->() noexcept { return &_get(); }




    constexpr bool operator==(LinearIterator const& other) const
#pragma GCC diagnostic push
# 88 "./include/commons/core/array_iterator.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 88 "./include/commons/core/array_iterator.hh"
        { return _array->data() + _index == other._array->data() + other._index; }
# 88 "./include/commons/core/array_iterator.hh"
#pragma GCC diagnostic pop
# 88 "./include/commons/core/array_iterator.hh"
                                                                                          ;






    void insert(T const& value) { _array->insert(_index, value); }






    void remove() { _array->erase(_index); }

    constexpr LinearIterator(Container const* array, size_t index)
        : _array(const_cast<Container*>(array)), _index(index)
    {}

    constexpr LinearIterator(Container* array, size_t index)
        : _array(array), _index(index)
    {}

    constexpr T const& _get() const { return (*_array)[_index]; }
    constexpr T& _get() { return (*_array)[_index]; }

private:
    Container* _array;
    T* _ptr;
    size_t _length;
    size_t _index;
};
# 130 "./include/commons/core/array_iterator.hh"
template<class Derived, class T>
class LinearIteratorComponent {
    using DP = Derived*;
    using DCP = Derived const*;

public:
    using Iterator = LinearIterator<Derived, T>;




    constexpr auto begin() { return Iterator(DP(this), 0); }




    constexpr auto begin() const { return Iterator(DCP(this), 0); }




    constexpr auto end() { return Iterator(DP(this), DCP(this)->length()); }




    constexpr auto end() const { return Iterator(DCP(this), DCP(this)->length()); }




    constexpr auto first() { return this->begin(); }




    constexpr auto first() const { return this->begin(); }




    constexpr auto last() { return Iterator(DP(this), DCP(this)->length() - 1); }




    constexpr auto last() const { return Iterator(DCP(this), DCP(this)->length() - 1); }




    constexpr auto at(size_t index)
    {
        DP(this)->operator[](index);
        return Iterator(this, index);
    }




    constexpr auto at(size_t index) const
    {
        DCP(this)->operator[](index);
        return Iterator(this, index);
    }




    template<typename... Args>
    struct ExclusionIterator : public IEquatable<ExclusionIterator<Args...>>
    {
        constexpr ExclusionIterator(T const* ptr, T const* end, Tuple<Args...> const& skip)
            : skip_(skip), ptr_(ptr), end_(end)
        {

            while (shouldSkip_() && ptr_ < end_)
                ptr_++;
        }

        constexpr T const& operator*() const noexcept { return *this->ptr_; }

        constexpr T& operator*() noexcept { return const_cast<T&>(*this->ptr_); }

        constexpr bool equals(T c) const noexcept { return *ptr_ == c; }

        constexpr bool equals(ExclusionIterator<Args...> const& i) const noexcept
        {
            return ptr_ == i.ptr_ || (ptr_ >= end_ && i.ptr_ >= end_);
        }

        constexpr ExclusionIterator& operator++() noexcept
# 221 "./include/commons/core/array_iterator.hh"
#pragma GCC diagnostic push
# 221 "./include/commons/core/array_iterator.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 221 "./include/commons/core/array_iterator.hh"
                                                           { do { if (ptr_ >= end_) break; ptr_++; } while (shouldSkip_()); return *this; }
# 221 "./include/commons/core/array_iterator.hh"
#pragma GCC diagnostic pop






          ;

        constexpr ExclusionIterator operator++(int) noexcept
# 230 "./include/commons/core/array_iterator.hh"
#pragma GCC diagnostic push
# 230 "./include/commons/core/array_iterator.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 230 "./include/commons/core/array_iterator.hh"
                                                             { do { if (ptr_ >= end_) break; ptr_++; } while (shouldSkip_()); return *this; }
# 230 "./include/commons/core/array_iterator.hh"
#pragma GCC diagnostic pop






          ;

    private:
        Tuple<Args...> skip_;
        T const* ptr_;
        T const* end_;

        constexpr bool shouldSkip_() const noexcept
# 244 "./include/commons/core/array_iterator.hh"
#pragma GCC diagnostic push
# 244 "./include/commons/core/array_iterator.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 244 "./include/commons/core/array_iterator.hh"
                                                    { return [&]<int N_>() { auto _ = [&]<int N>(auto ref) { if constexpr (N >= sizeof...(Args)) { return false; } else { if (*ptr_ == skip_.template get<N>()) return true; return ref.template operator()<N + 1>(ref); } }; return _.template operator()<N_>(_); }.template operator()<0>(); }
# 244 "./include/commons/core/array_iterator.hh"
#pragma GCC diagnostic pop
# 258 "./include/commons/core/array_iterator.hh"
          ;
    };

    template<typename... Args>
    struct Excluder
    {
        constexpr Excluder(LinearIteratorComponent const& b, Args... args)
            : b_(b), skip_(args...)
        {}

        constexpr auto begin() const
        {
            return ExclusionIterator<Args...>(
                static_cast<Derived const&>(b_).iterable_begin(), static_cast<Derived const&>(b_).iterable_end(),
                skip_);
        }

        constexpr auto end() const
        {
            auto const _ = static_cast<Derived const&>(b_).iterable_end();
            return ExclusionIterator<Args...>(_, _, skip_);
        }

    private:
        LinearIteratorComponent const& b_;
        Tuple<Args...> skip_;
    };




    template<typename... Args>
    requires (__is_same(Args, T) && ...)
    Excluder<Args...> exclude(Args... args) const
    {
        return Excluder<Args...>(*this, args...);
    }

    template<typename... Args>
    struct IncludingIterator
    {

        constexpr IncludingIterator(T const* ptr, T const* end, Tuple<Args...> const& include)
            : include_(include), ptr_(ptr), end_(end)
        {
            while (shouldSkip_() && ptr_ < end_)
                ptr_++;
        }

        constexpr operator T() const { return ptr_ >= end_ ? *end_ : *ptr_; }

        constexpr IncludingIterator operator*() const noexcept { return *this; }

        constexpr bool equals(T c) const noexcept { return *ptr_ == c; }

        constexpr bool equals(IncludingIterator const& i) const noexcept
        {
            return ptr_ == i.ptr_ || (ptr_ >= end_ && i.ptr_ >= end_);
        }

        constexpr IncludingIterator& operator++() noexcept
        {
            do {
                if (ptr_ >= end_)
                    break;
                ptr_++;
            } while (shouldSkip_());
            return *this;
        }

        constexpr IncludingIterator operator++(int) noexcept
        {
            do {
                if (ptr_ >= end_)
                    break;
                ptr_++;
            } while (shouldSkip_());
            return *this;
        }

    private:
        Tuple<Args...> include_;
        T const* ptr_;
        T const* end_;

        constexpr bool shouldSkip_() const noexcept
        {
            return [&]<int N_>() {
                auto _ = [&]<int N>(auto ref) {
                    if constexpr (N >= sizeof...(Args)) {
                        return true;
                    } else {
                        if (*ptr_ == include_.template get<N>())
                            return false;

                        return ref.template operator()<N + 1>(ref);
                    }
                };
                return _.template operator()<N_>(_);
            }.template operator()<0>();
        }
    };

    template<typename... Args>
    struct Includer
    {

        constexpr Includer(LinearIteratorComponent const& b, Args... args)
            : b_(b), skip_(args...)
        {}

        constexpr auto begin() const
        {
            return IncludingIterator<Args...>(
                static_cast<Derived const&>(b_).iterable_begin(), static_cast<Derived const&>(b_).iterable_end(),
                skip_);
        }

        constexpr auto end() const
        {
            auto end = static_cast<Derived const&>(b_).iterable_end();
            return IncludingIterator<Args...>(end, end, skip_);
        }

    private:
        LinearIteratorComponent const& b_;
        Tuple<Args...> skip_;
    };

    template<typename... Args>
    requires (__is_same(Args, T) && ...)
    constexpr Includer<Args...> include(Args... args) const
    {
        return Includer<Args...>(*this, args...);
    }
};

}
# 53 "./include/commons/core.hh" 2
# 1 "./include/commons/core/index.hh" 1
# 19 "./include/commons/core/index.hh"
namespace cm {







struct Index : Union<isize, usize>
{
    using Union<isize, usize>::Union;





    [[nodiscard]] [[gnu::always_inline]] inline constexpr auto computeUnchecked(auto const& a) const
    {
        usize i = match(
            [&](usize k) { return k; },
            [&](isize k) {
                return k < 0 ? usize(isize(a.length()) + k) : usize(k);
            });
        return i;
    }





    [[nodiscard]] [[gnu::always_inline]] inline constexpr auto compute(auto const& a) const
    {
        usize i = computeUnchecked(a);
        Assert(i < a.length(), ASMS_BOUNDS);
        return i;
    }




    [[nodiscard]] [[gnu::always_inline]] inline constexpr usize assertPositive() const
    {
        usize i = match(
            [&](usize k) { return k; },
            [&](isize k) {
                Assert(k >= 0, ASMS_PARAMETER);
                return usize(k);
            });
        return i;
    }
};


}
# 54 "./include/commons/core.hh" 2
# 1 "./include/commons/core/iterable.hh" 1
# 20 "./include/commons/core/iterable.hh"
namespace cm {






template<typename Derived>
struct Iterable
{




    constexpr static void outputString(Iterable const& self, auto const& out)
    {
        out('{');
        auto const& _self = DerivedRef(self);
        if (_self.length() != 0) {
            auto iter = _self.begin();
            auto i = 0uz;
            while (i < _self.length() - 1) {
                auto const& value = *iter;
                ::cm::OutputString(value, out);
                out(',');
                out(' ');
                ++i;
                ++iter;
            }
            if (i < _self.length()) {
                auto const& value = *iter;
                ::cm::OutputString(value, out);
            }
        }
        out('}');
    }




    constexpr bool equals(this Iterable const& self, auto const& other)
    {
        auto const& _self = DerivedRef(self);
        auto const& otherDerived = DerivedRef(other);

        if (&self == &other) {
            return true;
        }
        if (_self.length() != otherDerived.length()) {
            return false;
        }
        auto x = _self.begin();
        auto y = otherDerived.begin();
        while (x != _self.end()) {
            if (*x != *y) {
                return false;
            }
            ++x;
            ++y;
        }
        return true;
    }






    constexpr int equalsTimesafe(this Iterable const& self, auto const& other)
    {
        auto const& _self = DerivedRef(self);
        auto const& otherDerived = DerivedRef(other);
        auto selfIter = _self.begin();
        auto otherIter = otherDerived.begin();
        auto remaining = i64(::cm::max(_self.length(), otherDerived.length()));
        auto i = i64(0);
        auto notEqual = 0;

        while (i < i64(::cm::min(_self.length(), otherDerived.length()))) {
            notEqual |= (*selfIter != *otherIter);
            ++selfIter;
            ++otherIter;
            ++i;
            --remaining;
        }
        return !notEqual && (remaining == 0);
    }




    constexpr int compare(this Iterable const& self, auto const& other)
    {
        auto const& _self = DerivedRef(self);
        auto const& otherDerived = DerivedRef(other);

        if (&self == &other) {
            return 0;
        }
        if (_self.length() != otherDerived.length()) {
            return Compare(_self.length(), otherDerived.length());
        }
        auto x = _self.begin();
        auto y = otherDerived.begin();
        while (x != _self.end()) {
            if (*x < *y) {
                return -1;
            } else if (*x > *y) {
                return 1;
            }
            ++x;
            ++y;
        }
        return 0;
    }






    constexpr int compareTimesafe(this Iterable const& self, auto const& other)
    {
        auto const& _self = DerivedRef(self);
        auto const& otherDerived = DerivedRef(other);
        auto selfIter = _self.begin();
        auto otherIter = otherDerived.begin();
        auto remaining = i64(::cm::max(_self.length(), otherDerived.length()));
        auto i = i64(0);
        auto notEqual = 0;
        auto sign = 0;

        while (i < i64(::cm::min(_self.length(), otherDerived.length()))) {
            auto c = Compare(*selfIter, *otherIter);
            notEqual |= (c != 0);
            sign |= (c * (sign == 0));
            ++selfIter;
            ++otherIter;
            ++i;
            --remaining;
        }
        return (notEqual * sign) - !!remaining;
    }




    constexpr auto const& min(this Iterable const& self)
    {
        auto const& _self = DerivedRef(self);
        auto iter = _self.begin();
        auto const* minValue = &*iter;

        while (iter != _self.end()) {
            if (*iter < *minValue) {
                minValue = &*iter;
            }
            ++iter;
        }
        return *minValue;
    }




    constexpr auto const& max(this Iterable const& self)
    {
        auto const& _self = DerivedRef(self);
        auto iter = _self.begin();
        auto const* maxValue = &*iter;

        while (iter != _self.end()) {
            if (*iter > *maxValue) {
                maxValue = &*iter;
            }
            ++iter;
        }
        return *maxValue;
    }







    template<typename SumType = NoneType>
    constexpr auto sum(this Iterable const& self)
    {
        auto const& _self = DerivedRef(self);
        auto iter = _self.begin();




        SelectType<IsSame<SumType, NoneType>, SumType, CRefRemoved<decltype(*_self.begin())>> res = *iter;
        ++iter;
        while (iter != _self.end()) {
            res += *iter;
            ++iter;
        }
        return res;
    }
# 231 "./include/commons/core/iterable.hh"
    template<typename ProductType = NoneType>
    constexpr auto product(this Iterable const& self)
    {
        auto const& _self = DerivedRef(self);
        auto iter = _self.begin();
        SelectType<IsSame<ProductType, NoneType>, ProductType, CRefRemoved<decltype(*_self.begin())>> res = *iter;
        ++iter;
        while (iter != _self.end()) {
            res *= *iter;
            ++iter;
        }
        return res;
    }




    template<typename MeanType = NoneType>
    constexpr auto mean(this Iterable const& self)
    {
        using T = SelectType<IsSame<MeanType, NoneType>, MeanType, double>;
        return self.sum<T>() / T(DerivedRef(self).length());
    }




    inline Optional<usize> find(this Iterable const& self, Derived const& str, usize baseIndex = 0)
    {
        auto _self = DerivedRef(self);
        if (_self.length() < str.length()) {
            return None;
        } else if (_self.length() == str.length() && _self.equals(str)) {
            return 0uz;
        } else {
            for (usize i = baseIndex; i < _self.length() - str.length(); i++) {
#pragma GCC diagnostic push
# 267 "./include/commons/core/iterable.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 267 "./include/commons/core/iterable.hh"
                            ;
                auto s1 = _self.slice(i, str.length());
#pragma GCC diagnostic pop
# 269 "./include/commons/core/iterable.hh"
                          ;
                auto s2 = str.slice(0, str.length());
                if (s1.equals(s2)) {
                    return i;
                }
            }
            return None;
        }
    }


private:
    using DerivedRef = Derived const&;
};

}
# 55 "./include/commons/core.hh" 2
# 1 "./include/commons/core/arrayref.hh" 1
# 20 "./include/commons/core/arrayref.hh"
namespace cm {
# 29 "./include/commons/core/arrayref.hh"
template<typename T>

struct [[clang::trivial_abi]] [[clang::consumable(unconsumed)]] ArrayRef
    : public Iterable<ArrayRef<T>>,
      public LinearIteratorComponent<ArrayRef<T>, T const>,
      public IEquatable<ArrayRef<T>>,
      public IComparable<ArrayRef<T>>
{
private:
    using Iterable = Iterable<ArrayRef<T>> const&;
    T const* _ptr = nullptr;
    size_t _length = 0;


public:



    [[clang::return_typestate(unconsumed)]]
    constexpr ArrayRef() noexcept = default;






    [[clang::return_typestate(unconsumed)]]
    inline explicit constexpr ArrayRef(T const* ptr_, size_t length_) noexcept
        : _ptr(ptr_), _length(length_)
    {}
# 72 "./include/commons/core/arrayref.hh"
    [[clang::return_typestate(consumed)]]
    constexpr inline ArrayRef([[clang::lifetimebound]] std::initializer_list<T> const& v) noexcept
        : _ptr(const_cast<T*>(v.begin())), _length(v.size())
    {}




    template<unsigned N>
    [[clang::return_typestate(unconsumed)]] constexpr inline ArrayRef(
        [[clang::lifetimebound]] T const (&literal)[N]) noexcept
        : _ptr(const_cast<T*>(literal)), _length(N)
    {}


    [[clang::callable_when(unconsumed)]]
    constexpr ArrayRef([[clang::param_typestate(unconsumed)]] [[clang::lifetimebound]] ArrayRef const&) noexcept =
        default;

    [[clang::callable_when(unconsumed)]]
    constexpr ArrayRef([[clang::param_typestate(unconsumed)]] [[clang::lifetimebound]] ArrayRef&&) noexcept = default;


    [[clang::callable_when(unconsumed)]]
    constexpr ArrayRef&
    operator=([[clang::param_typestate(unconsumed)]] [[clang::lifetimebound]] ArrayRef&&) noexcept = default;

    [[clang::callable_when(unconsumed)]]
    constexpr ArrayRef&
    operator=([[clang::param_typestate(unconsumed)]] [[clang::lifetimebound]] ArrayRef const&) noexcept = default;






    constexpr inline T const& operator[](Index const& i) const
    {
        size_t i_ = i.compute(*this);
        Assert(i_ < length(), ASMS_BOUNDS);
#pragma GCC diagnostic push
# 112 "./include/commons/core/arrayref.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 112 "./include/commons/core/arrayref.hh"
        { return _ptr[i_]; }
# 112 "./include/commons/core/arrayref.hh"
#pragma GCC diagnostic pop
# 112 "./include/commons/core/arrayref.hh"
                                    ;
    }




    constexpr inline T const& operator()(Index const& i) const
    {
        size_t i_ = i.computeUnchecked(*this);
#pragma GCC diagnostic push
# 121 "./include/commons/core/arrayref.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 121 "./include/commons/core/arrayref.hh"
        { return _ptr[i_]; }
# 121 "./include/commons/core/arrayref.hh"
#pragma GCC diagnostic pop
# 121 "./include/commons/core/arrayref.hh"
                                    ;
    }




    constexpr inline auto length() const noexcept { return _length; }




    constexpr inline auto sizeBytes() const noexcept { return _length * sizeof(T); }




    constexpr inline T const* data() const noexcept { return _ptr; }
# 146 "./include/commons/core/arrayref.hh"
    constexpr bool equals(this ArrayRef<T> const& self, ArrayRef<T> const& other)
    {
        if consteval {
            return Iterable(self).equals(other);
        } else {

            if constexpr (IsPrimitiveData<T>) {
#pragma GCC diagnostic push
# 153 "./include/commons/core/arrayref.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 153 "./include/commons/core/arrayref.hh"
                            ;
                return __builtin_memcmp(self.data(), other.data(), self.sizeBytes()) == 0;
#pragma GCC diagnostic pop
# 155 "./include/commons/core/arrayref.hh"
                          ;
            } else {
                return Iterable(self).equals(other);
            }
        }
    }




    constexpr int compare(this ArrayRef<T> const& self, ArrayRef<T> const& other)
    {
        if consteval {
            return Iterable(self).compare(other);
        } else {

            if constexpr (IsPrimitiveData<T>) {
#pragma GCC diagnostic push
# 172 "./include/commons/core/arrayref.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 172 "./include/commons/core/arrayref.hh"
                            ;
                return __builtin_memcmp(self.data(), other.data(), self.sizeBytes());
#pragma GCC diagnostic pop
# 174 "./include/commons/core/arrayref.hh"
                          ;
            } else {
                return Iterable(self).compare(other);
            }
        }
    }
};






template<typename T>
ArrayRef(T*, size_t) -> ArrayRef<T>;

template<typename T>
ArrayRef(std::initializer_list<T>&&) -> ArrayRef<T>;

template<typename T, unsigned N>
ArrayRef(T const (&literal)[N]) -> ArrayRef<T>;


}
# 56 "./include/commons/core.hh" 2
# 1 "./include/commons/core/string_ref.hh" 1
# 21 "./include/commons/core/string_ref.hh"
namespace cm {





struct StringRef : public ArrayRef<char>, public IEquatable<StringRef>, public IComparable<StringRef>
{
private:
    using Base = ArrayRef<char>;

public:
    using ArrayRef<char>::ArrayRef;





    StringRef(char const* str, usize len)
        : Base(str, len + 1)
    {

    }




    constexpr StringRef(char const* str)
        : Base(str, CArrays::stringLen(str) + 1)
    {}





    constexpr StringRef(char ch)
        : Base(::cm::Data::oneCharStringTable(ch), 2)
    {}

    constexpr bool equals(StringRef const& s) const { return Base::equals(s); }
    constexpr bool equalsTimesafe(StringRef const& s) { return Base::equalsTimesafe(s); }
    constexpr int compare(StringRef const& s) const { return Base::compare(s); }
    constexpr int compareTimesafe(StringRef const& s) { return Base::compareTimesafe(s); }

    constexpr usize length() const { return usize(::cm::max(0, isize(Base::length()) - 1)); }

    constexpr usize sizeBytes() const { return this->length() * sizeof(char); }

    constexpr static void outputString(StringRef const& s, auto const& out)
    {
        for (char c : s) {
            out(c);
        }
    }




    constexpr char const* cstr() const noexcept { return this->data(); }




};


}
# 57 "./include/commons/core.hh" 2
# 1 "./include/commons/core/math_int.hh" 1
# 20 "./include/commons/core/math_int.hh"
namespace cm {


enum class IntegerParsingScheme {
# 32 "./include/commons/core/math_int.hh"
    JSON,
# 42 "./include/commons/core/math_int.hh"
    YAML,


    DEFAULT = JSON
};




enum class IntBaseFmt {

    B0 = 0,
    B1 = 1,
    B2 = 2,
    B3 = 3,
    B4 = 4,
    B5 = 5,
    B6 = 6,
    B7 = 7,
    B8 = 8,
    B9 = 9,
    B10 = 10,
    B16 = 16,


    B64,


    B64_URL,
    B64_crypt,
    B64_bcrypt,
    B64_Bash,
};





constexpr inline auto intBaseTables = []() {
    constexpr static struct T
    {
        ArrayRef<char const> charSet;
    } charSets[128] = {
        [int(IntBaseFmt::B2)] = {"0123456789abcdef"},
        [int(IntBaseFmt::B3)] = {"0123456789abcdef"},
        [int(IntBaseFmt::B8)] = {"0123456789abcdef"},
        [int(IntBaseFmt::B10)] = {"0123456789abcdef"},
        [int(IntBaseFmt::B16)] = {"0123456789abcdef"},
        [int(IntBaseFmt::B64_crypt)] = {"./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"},
        [int(IntBaseFmt::B64_bcrypt)] = {"./ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"},
        [int(IntBaseFmt::B64_Bash)] = {"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ@_"},
    };
    return ArrayRef<T>(const_cast<T*>(charSets), 128);
}();




template<IsInteger T>
constexpr inline auto BITS = unsigned(sizeof(T)) * 8u;

template<IsInteger T>
constexpr inline auto MIN_VALUE = IsIntegerSigned<T> ? (T(1) << (BITS<T> - 1)) : T(0);

template<IsInteger T>
constexpr inline auto MAX_VALUE = IsIntegerSigned<T> ? (~T(0) ^ MIN_VALUE<T>) : (~T(0));
# 134 "./include/commons/core/math_int.hh"
template<IsInteger T>
constexpr inline bool addOverflow(T const& x, T const& y, T& res)
{
    return __builtin_add_overflow(x, y, &res);
}







template<IsInteger T>
constexpr inline static bool addOverflow(T const& x, T const& y)
{
    T res{};
    return addOverflow(x, y, res);
}
# 160 "./include/commons/core/math_int.hh"
template<IsInteger T>
constexpr inline static bool mulOverflow(T const& x, T const& y, T& res)
{
    if constexpr (IsSame<T, u256>) {
        using u512 = _BitInt(512);
        auto r = u512(x) * u512(y);
        res = u256(r);
        return (r > (~u256(0)));
    } else if constexpr (IsSame<T, i256>) {
        using i512 = _BitInt(512);
        auto r = i512(x) * i512(y);
        res = i256(r);
        return (r < MIN_VALUE<i256> || r > MAX_VALUE<i256>);
    } else {
        return __builtin_mul_overflow(x, y, &res);
    }
}







template<IsInteger T>
constexpr inline static bool mulOverflow(T const& x, T const& y)
{
    T res{};
    return mulOverflow(x, y, res);
}
# 383 "./include/commons/core/math_int.hh"
template<IsInteger T, T Base>
struct PowerLookupTable
{
    T table[([] {
        auto k = T(1);
        auto size = 1u;
        for (; !mulOverflow(k, Base, k); size++)
            ;
        return size;
    }())];

    consteval PowerLookupTable()
    {
        auto k = T(1);
        auto i = T(0);
        (*this)[i++] = k;
        while (!mulOverflow(k, Base, k)) {
            (*this)[i++] = k;
        }
    }

    constexpr auto length() const { return sizeof(table) / sizeof(T); }
    constexpr T& operator[](T const& i)
# 405 "./include/commons/core/math_int.hh"
#pragma GCC diagnostic push
# 405 "./include/commons/core/math_int.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 405 "./include/commons/core/math_int.hh"
                                        { return table[i]; }
# 405 "./include/commons/core/math_int.hh"
#pragma GCC diagnostic pop
# 405 "./include/commons/core/math_int.hh"
                                                                    ;
    constexpr T const& operator[](T const& i) const
# 406 "./include/commons/core/math_int.hh"
#pragma GCC diagnostic push
# 406 "./include/commons/core/math_int.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 406 "./include/commons/core/math_int.hh"
                                                    { return table[i]; }
# 406 "./include/commons/core/math_int.hh"
#pragma GCC diagnostic pop
# 406 "./include/commons/core/math_int.hh"
                                                                                ;
};






template<IsInteger auto Base>
constexpr static auto pow(IsInteger auto k) -> Optional<decltype(k)>
{
    using T = CVRefRemoved<decltype(k)>;
    constexpr static PowerLookupTable<T, Base> table;
    if constexpr (IsIntegerSigned<T>) {
        if (k < 0) {
            return None;
        }
    }
    [[assume(k >= 0)]];
    if (UintN<BITS<T>>(k) >= table.length()) {
        return None;
    }
    return table[k];
}






constexpr static auto clz(IsInteger auto x) -> UintRanged<BITS<decltype(x)>>
{
    using R = UintRanged<BITS<decltype(x)>>;
    if constexpr (sizeof(x) <= sizeof(int)) {
        return R(__builtin_clz(__builtin_bit_cast(unsigned int, x)));
    } else if constexpr (sizeof(x) <= sizeof(long)) {
        return R(__builtin_clzl(__builtin_bit_cast(unsigned long, x)));
    } else if constexpr (sizeof(x) <= sizeof(long long)) {
        return R(__builtin_clzll(__builtin_bit_cast(unsigned long long, x)));
    } else {
        constexpr auto bits = R(sizeof(x)) * 8u;
        R count = 0;
        if (!x) {
            return bits;
        }
        for (auto i = R(bits) - 1; i >= 0; --i) {
            auto mask = static_cast<decltype(x)>(1) << i;
            if ((x & mask) != 0) {
                break;
            }
            count++;
        }
        return R(count);
    }
}
# 469 "./include/commons/core/math_int.hh"
template<IsInteger auto Base = 10>
constexpr static u8 log(IsInteger auto x)
{
    if constexpr (Base == 2) {
        return u8(((sizeof(x) * 8) - 1) - unsigned(clz(x)));
    } else if constexpr (Base == 16) {
        return log<2>(x) / 4;
    } else if constexpr (Base == 10) {
        constexpr u8
# 477 "./include/commons/core/math_int.hh"
#pragma GCC diagnostic push
# 477 "./include/commons/core/math_int.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 477 "./include/commons/core/math_int.hh"
                     guess[33]
# 477 "./include/commons/core/math_int.hh"
#pragma GCC diagnostic pop
# 477 "./include/commons/core/math_int.hh"
                                       = {0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4,
                                          5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9};
        u8
# 479 "./include/commons/core/math_int.hh"
#pragma GCC diagnostic push
# 479 "./include/commons/core/math_int.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 479 "./include/commons/core/math_int.hh"
           digits
# 479 "./include/commons/core/math_int.hh"
#pragma GCC diagnostic pop
# 479 "./include/commons/core/math_int.hh"
                          = guess[log<2>(x)];
        return u8(digits + ((x / 10) >= pow<10>(digits).value()));
    } else {
        static_assert(false, "Not implemented");
    }
}




template<unsigned BaseN>
constexpr static auto msd(IsInteger auto x) -> decltype(x)
{
    using T = decltype(x);
    return T(x / pow<BaseN>(log<BaseN>(x)).value());
}
# 509 "./include/commons/core/math_int.hh"
constexpr void ::cm::__outputString(auto const& value, auto const& out)
{
    constexpr IntBaseFmt Base = IntBaseFmt::B10;
    constexpr IntegerParsingScheme S = IntegerParsingScheme::DEFAULT;
    using T = CVRefRemoved<decltype(value)>;

    if constexpr (IsUnderlyingTypeOneOf<T, char, char8_t>) {
        out(value);
    } else if constexpr (IsBool<T>) {
        if (value) {
            out('t');
            out('r');
            out('u');
            out('e');
        } else {
            out('f');
            out('a');
            out('l');
            out('s');
            out('e');
        }
    } else if constexpr (IsInteger<T>) {
        if constexpr (S == IntegerParsingScheme::JSON) {
            static_assert(Base == IntBaseFmt::B10, "JSON only supports base 10");
        }
        auto num = value;
        if constexpr (IsIntegerSigned<T>) {
            if (num < 0) {
                out('-');
                num = static_cast<T>(-value);
            }
        }

        if constexpr (Base == IntBaseFmt::B2) {
            if constexpr (S == IntegerParsingScheme::YAML) {
                out("0b");
            }
        } else if constexpr (Base == IntBaseFmt::B8) {
            if constexpr (S == IntegerParsingScheme::YAML) {
                out("0o");
            }
        } else if constexpr (Base == IntBaseFmt::B16) {
            if constexpr (S == IntegerParsingScheme::YAML) {
                out("0x");
            }
        }
        if (num == 0) {
            out(intBaseTables[uint(Base)].charSet[0]);
            return;
        }
        do {
            out(intBaseTables[uint(Base)].charSet[msd<uint(Base)>(num)]);
            num /= uint(Base);
        } while (num != 0);

    } else if constexpr (IsFloatingPoint<T>) {


        auto val = double(value);
        i64 integer;
        int zero_threshold = 16;
        int zero_seq_len = 0;

        if (__builtin_isnan(val)) {
            out('N'), out('a'), out('N');
            return;
        } else if (__builtin_isinf(val)) {
            if (__builtin_signbit(val)) {
                out('-'), out('i'), out('n'), out('f');
                return;
            } else {
                out('i'), out('n'), out('f');
                return;
            }
        }
        if (val < 0) {
            out('-');
            val = -val;
        }
        integer = i64(val);
        if (integer == 0) {
            out('0');
            return;
        }
        OutputString(integer, out);
        out('.');

        while (true) {
            val = val - static_cast<double>(i64(val));
            val *= 10;
            integer = i64(val);
            if (integer == 0) {
                zero_seq_len++;
                out('0');
                if (zero_seq_len >= zero_threshold) {
                    return;
                }
            } else {
                zero_seq_len = 0;
                OutputString(integer, out);
            }
            zero_threshold--;
        }
    }
}

}
# 58 "./include/commons/core.hh" 2
# 1 "./include/commons/core/math_float.hh" 1
# 21 "./include/commons/core/math_float.hh"
namespace cm {




struct Float
{

    constexpr static float MIN_VALUE = -3.40282347E+38f;


    constexpr static float MAX_VALUE = 3.40282347E+38f;


    constexpr static float ZERO = bit_cast<float>(0);


    constexpr static float NEG_ZERO = bit_cast<float>(0x80000000);


    constexpr static float POS_INF = bit_cast<float>(0x7F800000);


    constexpr static float NEG_INF = bit_cast<float>(0xFF800000);


    constexpr static float QNAN = bit_cast<float>(0x7fc00000);


    constexpr static float SNAN = bit_cast<float>(0x7F800001);


    constexpr static float MIN_EPSILON = 1.401298E-45f;


    constexpr static float E = 2.7182818284590452354f;


    constexpr static float PI = 3.1415926535897932385f;


    constexpr static float TAU = 6.2831853071795864769f;




    [[gnu::always_inline]] inline constexpr static int signbit(float x) { return (bit_cast<u32>(x) >> 31u) == 1; }


    [[gnu::always_inline]] inline constexpr static int signum(float x) { return signbit(x) == 1 ? -1 : 1; }


    [[gnu::always_inline]] inline constexpr static bool isNaN(float x)
    {
        return ((bit_cast<u32>(x) & 0x7fffffffu) > 0x7f800000u) != 0u;
    }


    [[gnu::always_inline]] inline constexpr static bool isInf(float x) { return (bit_cast<u32>(x) & 0x7fffffffu) == 0x7f800000u; }


    [[gnu::always_inline]] inline constexpr static float fma(float x, float y, float z)
    {

#pragma clang fp contract(fast)
        return (x * y) + z;
    }





    [[gnu::always_inline]] inline constexpr static float max(float x, float y)
    {
        if !consteval {
            return _maxIntrin(x, y);
        } else {
            if (isNaN(x)) {
                if (isNaN(y))
                    return QNAN;
                return y;
            }
            if (isNaN(y)) {
                return x;
            }
            return x > y ? x : y;
        }
    }


public:
    [[nodiscard]] constexpr static float min(float x, float y)
    {
        if !consteval {
            return _minIntrin(x, y);
        } else {
            if (isNaN(x)) {
                if (isNaN(y))
                    return __builtin_bit_cast(float, 0x7fc00000);
                return y;
            }
            if (isNaN(y)) {
                return x;
            }
            return x < y ? x : y;
        }
    }

    [[gnu::always_inline]] inline constexpr static float trunc(float x)
    {






        {
            auto i0 = bit_cast<i32>(x);
            auto sx = i0 & bit_cast<i32>(0x80000000);
            auto j0 = ((i0 >> 23) & 0xff) - 0x7f;

            if (j0 < 23) {
                if (j0 < 0)

                    x = bit_cast<float>(x);
                else
                    x = bit_cast<float>(sx | (i0 & ~(0x007fffff >> j0)));
            } else {
                if (j0 == 0x80)

                    return x + x;
            }
            return x;
        }
    }


    [[gnu::always_inline]] inline constexpr static float round(float x)
    {
        auto i0 = bit_cast<i32>(x);
        auto j0 = ((i0 >> 23) & 0xff) - 0x7f;

        if (j0 < 23) {
            if (j0 < 0) {
                i0 &= 0x80000000;
                if (j0 == -1)
                    i0 |= 0x3f800000;
            } else {
                u32 i = 0x007fffff >> j0;
                if ((i0 & bit_cast<i32>(i)) == 0) {

                    return x;
                }
                i0 += 0x00400000 >> j0;
                i0 &= ~i;
            }
        } else {

            return j0 == 0x80 ? x + x : x;
        }
        return bit_cast<float>(i0);
    }

    [[gnu::always_inline]] inline constexpr static float floor(float x)
    {
        if consteval {
            return _floor(x);
        } else {
            return _floorIntrin(x);
        }
    }
# 212 "./include/commons/core/math_float.hh"
    constexpr static Tuple<float, int> frexpFiniteNonzero(float value)
    {
        u32 const bits = __builtin_bit_cast(u32, value);
        int const expon = ((bits & 0x7e000000) >> 23) - 126;
        u32 const final_bits = (bits & 0x807fffff) | 0x3f000000;
        return {__builtin_bit_cast(float, final_bits), expon};
    }




    constexpr static float fractionFiniteNonzero(float value)
    {
        u32 const bits = __builtin_bit_cast(u32, value);
        u32 const final_bits = (bits & 0x807fffff) | 0x3f000000;
        return __builtin_bit_cast(float, final_bits);
    }
# 254 "./include/commons/core/math_float.hh"
    constexpr static Tuple<float, int> frexp(float value)
    {

        u32 const bits = __builtin_bit_cast(u32, value);
        bool isNotFiniteOrIsZero = (bits >= 0x7f800000u || bits == 0);

        int const expon = (((bits & 0x7e000000) >> 23) - 126) * !isNotFiniteOrIsZero;
        u32 const finalBits = (bits & 0x807fffff) | 0x3f000000;





        u64 const choice = (u64(bits) << 32) | u64(finalBits);


        u8 const shift = u8(u8(isNotFiniteOrIsZero) << 5);
        u32 const finalFinalBits = u32((choice & (u64(0xffffffff) << shift)) >> shift);

        float const result = __builtin_bit_cast(float, finalFinalBits);
        return {result, expon};
    }





    [[gnu::flatten]]
    constexpr static float log2(float value)
    {
        constexpr float a = 0.59329970349044314f, b = 2.3979646338966889f, c = -0.96358966800238843f,
                        d = -1.8439274267589987f, e = -0.18374724264449727f, f = 0.1068562844523792f,
                        g = 1.2392957064266512f, h = 2.0062979261642901f, i = 0.63680961689938775f,
                        j = 0.028211791264274255f;

        u32 mask = (__builtin_bit_cast(u32, value) & 0x7fffffffu);
        if (mask >= 0x7f800000u || value <= 0.0f) [[unlikely]] {
            if (value < 0.0f) {
                return -QNAN;
            } else if (mask == 0) {
                return NEG_INF;
            } else {
                return value;
            }
        }

        auto [dM, iExp] = frexpFiniteNonzero(value);
        auto dM2 = dM * dM;
        auto dM3 = dM * dM2;
        auto dM4 = dM2 * dM2;
        auto x = 1.0f / (((f * dM4 + h * dM2) + (g * dM3 + i * dM)) + j);
        return float(iExp) + x * (((a * dM4 + c * dM2) + (b * dM3 + d * dM)) + e);
    }

private:
    static float _maxIntrin(float x, float y);
    static float _minIntrin(float x, float y);
    static float _floorIntrin(float x);


    constexpr static float _floor(float x)
    {
        auto i0 = bit_cast<int>(x);
        auto j0 = ((i0 >> 23) & 0xff) - 0x7f;

        if (j0 < 0) {
            if (i0 >= 0) {
                i0 = 0;
            } else if ((i0 & 0x7fffffff) != 0) {
                i0 = __builtin_bit_cast(int, 0xbf800000);
            }
        } else if (j0 < 23) {
            auto i = (0x007fffff) >> j0;
            if ((i0 & i) == 0) {
                return x;
            }
            if (i0 < 0) {
                i0 += (0x00800000) >> j0;
            }
            i0 &= (~i);
        } else {
            return j0 == 0x80 ? x + x : x;
        }
        return bit_cast<float>(i0);
    }

} inline constexpr Float;
# 352 "./include/commons/core/math_float.hh"
constexpr long double fmal(long double x, long double y, long double z)
{
#pragma clang fp contract(fast)
    return (x * y) + z;
}
# 368 "./include/commons/core/math_float.hh"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wfloat-equal"


static_assert(Float.max(1.0f, 2.0f) == 2.0f);
static_assert(Float.max(0.001f, -0.2f) == 0.001f);
static_assert(Float.max(-1.0f, Float.QNAN) == -1.0f);
static_assert(Float.max(Float.QNAN, -1.0f) == -1.0f);
static_assert(Float.max(3.141f, Float.QNAN) == 3.141f);
static_assert(Float.max(Float.QNAN, 3.141f) == 3.141f);
static_assert(Float.max(-3.1f, 0.0f) == 0.0f);
static_assert(Float.max(0.0f, -3.1f) == 0.0f);
static_assert(Float.max(+0.0f, -0.0f) == +0.0f);
static_assert(Float.isNaN(Float.max(Float.QNAN, Float.QNAN)));
static_assert(Float.trunc(-3.49f) == -3.0f);
static_assert(Float.trunc(1.33f) == 1.0f);

#pragma clang diagnostic pop

}
# 59 "./include/commons/core.hh" 2
# 1 "./include/commons/core/math_double.hh" 1
# 21 "./include/commons/core/math_double.hh"
namespace cm {




struct Double
{
    constexpr static double MIN_VALUE = -1.7976931348623157E+308;

    constexpr static double MAX_VALUE = 1.7976931348623157E+308;

    constexpr static double ZERO = double(0);

    constexpr static double POS_INF = bit_cast<double>(0x7FF0000000000000);

    constexpr static double NEG_INF = bit_cast<double>(0xFFF0000000000000);



    constexpr static double QNAN = bit_cast<double>(0x7ff8000000000000);


    constexpr static double SNAN = bit_cast<double>(0x7ff0000000000001);


    constexpr static double MIN_EPSILON = 4.94065645841247E-324;


    constexpr static double E = 2.7182818284590452354;


    constexpr static double PI = 3.1415926535897932385;


    constexpr static double TAU = 6.2831853071795864769;


    [[gnu::always_inline]] inline constexpr static int signbit(double x) { return (bit_cast<u64>(x) >> 63u) == 1; }


    [[gnu::always_inline]] inline constexpr static int signum(double x) { return signbit(x) == 1 ? -1 : 1; }





    [[gnu::always_inline]] inline constexpr static bool isNaN(double x)
    {
        auto u = bit_cast<u64>(x);
        return (static_cast<u32>(u >> 32) & 0x7fffffff) + (static_cast<u32>(u) != 0) > 0x7ff00000;
    }




    [[gnu::always_inline]] inline constexpr static int isInf(double x)
    {
        auto u = bit_cast<u64>(x);
        return (static_cast<u32>(u >> 32) & 0x7fffffff) == 0x7ff00000 && static_cast<u32>(u) == 0;
    }


    [[gnu::always_inline]] inline constexpr static double fma(double x, double y, double z)
    {
#pragma clang fp contract(fast)
# 85 "./include/commons/core/math_double.hh"
                                          ;
        return (x * y) + z;
    }


    [[gnu::always_inline]] inline constexpr static double trunc(double x)
    {
        long long i = static_cast<long long>(x);
        if (x < 0) {
            return static_cast<double>(i + (i < static_cast<long long>(x)));
        }
        return static_cast<double>(i - (i > static_cast<long long>(x)));
    }


    constexpr static double pow(double x, int n)
    {
        double r = 1.0;
        decltype(n) y;

        if (x == 1.0)
            return 1.0;

        if (bit_cast<i64>(x) == bit_cast<i64>(-1.0))
            return !(n & 1) ? 1.0 : -1.0;

        if (n == ::cm::MAX_VALUE<int>)
            return (x < 1.0 && x > -1.0) ? 0.0 : (1.0 / 0.0);

        if (n == ::cm::MIN_VALUE<int>)
            return 0.0;

        y = n;
        if (n < 0)
            n = -n;

        for (; n > 0; n--, r *= x)
            ;
        return y < 0 ? 1.0 / r : r;
    }


    constexpr static auto frexp_v2_finite_nonzero(double value) -> Tuple<double, int>
    {
        u64 const bits = __builtin_bit_cast(u64, value);
        int const expon = ((bits >> 52) & 0x7FF) - 1022;
        u64 const final_bits = (bits & 0x800fffff'ffffffff) | 0x3fe0000000000000;
        return {__builtin_bit_cast(double, final_bits), expon};
    }

    constexpr static auto frexp_v2(double value) -> Tuple<double, int>
    {
        u64 const bits = __builtin_bit_cast(u64, value);
        bool const isNotFiniteOrIsZero = (bits >= 0x7FF0000000000000 || bits == 0);

        int const expon = (((bits >> 52) & 0x7FF) - 1022) * !isNotFiniteOrIsZero;
        u64 const finalBits = (bits & 0x800fffff'ffffffff) | 0x3fe0000000000000;

        u128 const choice = (u128(bits) << 64) | u128(finalBits);
        u8 const shift = u8(u8(isNotFiniteOrIsZero) << 6);
        u64 const finalFinalBits = u64((choice & (u128(~0ULL) << shift)) >> shift);

        double result = __builtin_bit_cast(double, finalFinalBits);
        return {result, expon};
    }





    constexpr static double log2(double value)
    {
        using T = double;
        constexpr double a = 1.000000000000000000000e+00L, b = 1.251649209001242901707e+01L,
                         c = 2.046583854860732643033e+01L, d = -1.097536826489419503616e+01L,
                         e = -1.881965876655596403566e+01L, f = -4.046684236630626152476e+00L,
                         g = -1.406193705389736370304e-01L, h = 1.518692933639071429575e-01L,
                         i = 3.897795033656223484542e+00L, j = 1.728188727732104723600e+01L,
                         k = 2.168720176727976678421e+01L, l = 8.568477446142038544963e+00L,
                         m = 9.581795852523320444760e-01L, n = 1.851054408942580734032e-02L;

        u64 mask = (__builtin_bit_cast(u64, value));
        if (mask >= 0x7FF0000000000000 || value <= 0.0) [[unlikely]] {
            if (value < 0.0) {
                return -QNAN;
            } else if (mask == 0) {
                return NEG_INF;
            } else {
                return value;
            }
        }
        auto [dM, iExp] = frexp_v2_finite_nonzero(value);
        T dM2 = dM * dM;
        T dM3 = dM * dM2;
        T dM4 = dM2 * dM2;
        T dM5 = dM2 * dM3;
        T dM6 = dM3 * dM3;
        T x = 1.0 / ((((h * dM6 + n) + (i * dM5 + m * dM)) + (j * dM4 + l * dM2)) + k * dM3);
        return iExp + x * (((((a * dM6 + g) + (c * dM4 + d * dM3)) + b * dM5) + f * dM) + e * dM2);
    }




    constexpr static double log10(double x)
    {
        return double(0.301029995663981195213738894724493026768189881462108541310L) * log2(x);
    }




    constexpr static double ln(double x)
    {
        return double(0.693147180559945309417232121458176568075500134360255254120L) * log2(x);
    }

} inline constexpr Double;

}
# 60 "./include/commons/core.hh" 2


# 1 "./include/commons/core/rng.hh" 1
# 21 "./include/commons/core/rng.hh"
namespace cm {
# 33 "./include/commons/core/rng.hh"
struct FastPRNG
{

    [[gnu::always_inline]]
    inline static u32 getU32()
    {






        _state = __builtin_ia32_crc32si(_state, _state & 0x0000ffff);
        return _state;
    }

private:
    static u32 _state;
};




struct ND_PRNG
{
    inline static u32 getU32()
    {
# 70 "./include/commons/core/rng.hh"
        long unsigned int rip;
        asm volatile("lea (%%rip), %0" : "=r"(rip));
        _state = __builtin_ia32_crc32si(static_cast<u32>(rip), _state);

        return __builtin_ia32_crc32si(static_cast<u32>(__builtin_readcyclecounter()), _state);
    }

private:
    static u32 _state;
};

}
# 63 "./include/commons/core.hh" 2
# 1 "./include/commons/core/hash.hh" 1
# 19 "./include/commons/core/hash.hh"
# 1 "./include/commons/core/hashers/crc32_hasher.hh" 1
# 19 "./include/commons/core/hashers/crc32_hasher.hh"
namespace cm {





struct Crc32
{
    constexpr static auto DEFAULT_SEED = 1;
    using SeedType = u32;
    using HashResult = u32;






    constexpr static auto const& table()
    {
        constexpr static VectorU32x<256> tbl = {

# 1 "./include/commons/resources/crc32_castagnoli_table.csv" 1
# 15 "./include/commons/resources/crc32_castagnoli_table.csv"
0, 4067132163, 3778769143, 324072436, 3348797215, 904991772, 648144872,
3570033899, 2329499855, 2024987596, 1809983544, 2575936315, 1296289744, 3207089363,
2893594407, 1578318884, 274646895, 3795141740, 4049975192, 51262619, 3619967088,
632279923, 922689671, 3298075524, 2592579488, 1760304291, 2075979607, 2312596564,
1562183871, 2943781820, 3156637768, 1313733451, 549293790, 3537243613, 3246849577,
871202090, 3878099393, 357341890, 102525238, 4101499445, 2858735121, 1477399826,
1264559846, 3107202533, 1845379342, 2677391885, 2361733625, 2125378298, 820201905,
3263744690, 3520608582, 598981189, 4151959214, 85089709, 373468761, 3827903834,
3124367742, 1213305469, 1526817161, 2842354314, 2107672161, 2412447074, 2627466902,
1861252501, 1098587580, 3004210879, 2688576843, 1378610760, 2262928035, 1955203488,
1742404180, 2511436119, 3416409459, 969524848, 714683780, 3639785095, 205050476,
4266873199, 3976438427, 526918040, 1361435347, 2739821008, 2954799652, 1114974503,
2529119692, 1691668175, 2005155131, 2247081528, 3690758684, 697762079, 986182379,
3366744552, 476452099, 3993867776, 4250756596, 255256311, 1640403810, 2477592673,
2164122517, 1922457750, 2791048317, 1412925310, 1197962378, 3037525897, 3944729517,
427051182, 170179418, 4165941337, 746937522, 3740196785, 3451792453, 1070968646,
1905808397, 2213795598, 2426610938, 1657317369, 3053634322, 1147748369, 1463399397,
2773627110, 4215344322, 153784257, 444234805, 3893493558, 1021025245, 3467647198,
3722505002, 797665321, 2197175160, 1889384571, 1674398607, 2443626636, 1164749927,
3070701412, 2757221520, 1446797203, 137323447, 4198817972, 3910406976, 461344835,
3484808360, 1037989803, 781091935, 3705997148, 2460548119, 1623424788, 1939049696,
2180517859, 1429367560, 2807687179, 3020495871, 1180866812, 410100952, 3927582683,
4182430767, 186734380, 3756733383, 763408580, 1053836080, 3434856499, 2722870694,
1344288421, 1131464017, 2971354706, 1708204729, 2545590714, 2229949006, 1988219213,
680717673, 3673779818, 3383336350, 1002577565, 4010310262, 493091189, 238226049,
4233660802, 2987750089, 1082061258, 1395524158, 2705686845, 1972364758, 2279892693,
2494862625, 1725896226, 952904198, 3399985413, 3656866545, 731699698, 4283874585,
222117402, 510512622, 3959836397, 3280807620, 837199303, 582374963, 3504198960,
68661723, 4135334616, 3844915500, 390545967, 1230274059, 3141532936, 2825850620,
1510247935, 2395924756, 2091215383, 1878366691, 2644384480, 3553878443, 565732008,
854102364, 3229815391, 340358836, 3861050807, 4117890627, 119113024, 1493875044,
2875275879, 3090270611, 1247431312, 2660249211, 1828433272, 2141937292, 2378227087,
3811616794, 291187481, 34330861, 4032846830, 615137029, 3603020806, 3314634738,
939183345, 1776939221, 2609017814, 2295496738, 2058945313, 2926798794, 1545135305,
1330124605, 3173225534, 4084100981, 17165430, 307568514, 3762199681, 888469610,
3332340585, 3587147933, 665062302, 2042050490, 2346497209, 2559330125, 1793573966,
3190661285, 1279665062, 1595330642, 2910671697
# 41 "./include/commons/core/hashers/crc32_hasher.hh" 2

        };
        return tbl;
    }




    constexpr static u32 hash8(u8 value, u32 seed) noexcept
    {

        if consteval

        {
            auto const& tbl = table();
            auto const hash = seed;
            return tbl[(hash ^ value) & 0xffu] ^ (hash >> 8u);
        }

        else {

            return static_cast<u32>(__builtin_ia32_crc32qi(seed, value));




        }

    }




    constexpr static u32 hash16(u16 value, u32 seed) noexcept
    {

        if consteval

        {
            auto const& tbl = table();
            seed = tbl[(seed ^ bit_cast<VectorU8x<2>>(value)[0]) & 0xffu] ^ (seed >> 8u);
            seed = tbl[(seed ^ bit_cast<VectorU8x<2>>(value)[1]) & 0xffu] ^ (seed >> 8u);
            return seed;
        }

        else {

            return static_cast<u32>(__builtin_ia32_crc32hi(seed, value));



        }

    }




    constexpr static u32 hash32(u32 value, u32 seed) noexcept
    {

        if consteval

        {
            auto const& tbl = table();
            seed = tbl[(seed ^ bit_cast<VectorU8x<4>>(value)[0]) & 0xffu] ^ (seed >> 8u);
            seed = tbl[(seed ^ bit_cast<VectorU8x<4>>(value)[1]) & 0xffu] ^ (seed >> 8u);
            seed = tbl[(seed ^ bit_cast<VectorU8x<4>>(value)[2]) & 0xffu] ^ (seed >> 8u);
            seed = tbl[(seed ^ bit_cast<VectorU8x<4>>(value)[3]) & 0xffu] ^ (seed >> 8u);
            return seed;
        }

        else {

            return static_cast<u32>(__builtin_ia32_crc32si(seed, value));




        }

    }




    constexpr static u32 hash64(u64 value, u32 seed) noexcept
    {

        if consteval

        {
            auto const& tbl = table();
            seed = tbl[(seed ^ bit_cast<VectorU8x<8>>(value)[0]) & 0xffu] ^ (seed >> 8u);
            seed = tbl[(seed ^ bit_cast<VectorU8x<8>>(value)[1]) & 0xffu] ^ (seed >> 8u);
            seed = tbl[(seed ^ bit_cast<VectorU8x<8>>(value)[2]) & 0xffu] ^ (seed >> 8u);
            seed = tbl[(seed ^ bit_cast<VectorU8x<8>>(value)[3]) & 0xffu] ^ (seed >> 8u);
            seed = tbl[(seed ^ bit_cast<VectorU8x<8>>(value)[4]) & 0xffu] ^ (seed >> 8u);
            seed = tbl[(seed ^ bit_cast<VectorU8x<8>>(value)[5]) & 0xffu] ^ (seed >> 8u);
            seed = tbl[(seed ^ bit_cast<VectorU8x<8>>(value)[6]) & 0xffu] ^ (seed >> 8u);
            seed = tbl[(seed ^ bit_cast<VectorU8x<8>>(value)[7]) & 0xffu] ^ (seed >> 8u);
            return seed;
        }

        else {

            return static_cast<u32>(__builtin_ia32_crc32di(seed, value));




        }

    }
};


}
# 20 "./include/commons/core/hash.hh" 2



namespace cm {

#pragma GCC diagnostic push
# 25 "./include/commons/core/hash.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 25 "./include/commons/core/hash.hh"
            ;
# 35 "./include/commons/core/hash.hh"
template<typename Hasher>
struct Hash
{
    constexpr static auto DEFAULT_SEED = Hasher::DEFAULT_SEED;
    using HashResult = Hasher::HashResult;
    using SeedType = Hasher::SeedType;

    template<typename T>
    constexpr inline static HashResult hash(T in, SeedType seed = DEFAULT_SEED)
    {
        if constexpr (IsUnderlyingTypeOneOf<T, char*, wchar_t*, char8_t*, char16_t*, char32_t*>) {
            return _hashCharPtr(in);
        } else if constexpr (!IsClass<T>) {
            if constexpr (sizeof(in) == 1) {
                return Hasher::hash8(bit_cast<u8>(in), seed);
            } else if constexpr (sizeof(in) == 2) {
                return Hasher::hash16(bit_cast<u16>(in), seed);
            } else if constexpr (sizeof(in) == 4) {
                return Hasher::hash32(bit_cast<u32>(in), seed);
            } else if constexpr (sizeof(in) == 8) {
                return Hasher::hash64(bit_cast<u64>(in), seed);
            } else {
                static_assert(false, "Unable to hash primitive type");
                return HashResult{};
            }
        } else {
            if constexpr (IsPointer<T>) {
                return hash(reinterpret_cast<usize>(in), seed);
            } else {
                return in.template hash<Hasher>(seed);
            }
        }
    }

    constexpr static auto hashData(void const* mem, usize sizeBytes, SeedType seed = DEFAULT_SEED)
# 69 "./include/commons/core/hash.hh"
#pragma GCC diagnostic push
# 69 "./include/commons/core/hash.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 69 "./include/commons/core/hash.hh"
                                                                                                   { auto* bytes = static_cast<u8 const*>(mem); for (usize i = {}; i < sizeBytes; i += usize(1)) { seed = hash(bytes[size_t(i)], seed); }; return seed; }
# 69 "./include/commons/core/hash.hh"
#pragma GCC diagnostic pop





      ;

private:
    template<typename T>
    constexpr static auto _hashCharPtr(T in, SeedType seed = DEFAULT_SEED)
# 79 "./include/commons/core/hash.hh"
#pragma GCC diagnostic push
# 79 "./include/commons/core/hash.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 79 "./include/commons/core/hash.hh"
                                                                           { do { seed = hash(*in, seed); } while (*in++); return seed; }
# 79 "./include/commons/core/hash.hh"
#pragma GCC diagnostic pop




      ;
};

using DefaultHasher = Hash<Crc32>;

}

#pragma GCC diagnostic pop
# 64 "./include/commons/core.hh" 2

# 1 "./include/commons/core/search.hh" 1







namespace cm {


template<typename T>
constexpr inline auto DefaultSortPredicate = [](T const& x, T const& y) -> int {
    if (x < y) {
        return -1;
    } else if (x == y) {
        return 0;
    } else {
        return 1;
    }
};

namespace Sort {

template<typename T>
constexpr inline auto Ascending = DefaultSortPredicate<T>;
}

namespace Search {

template<typename T>
constexpr inline auto DefaultComparator = [](T const& a, T const& b) -> bool {
    return a == b;
};

template<typename T>
struct Result
{
    i64 index;
    T value;
    Result(i64 index, T value)
        : index(index), value(value)
    {}
};

template<typename T>
Result(i64, T) -> Result<T>;
# 55 "./include/commons/core/search.hh"
template<typename T, typename F = decltype(DefaultComparator<T>)>
Optional<Result<T>> linearSearch(auto const& container, T const& value, F cmp = DefaultComparator<T>)
{
    long i = 0;
    for (auto const& cont_value : container) {
        if (cmp(cont_value, value)) {
            return Result(i, cont_value);
        }
        i++;
    }
    return None;
}
# 78 "./include/commons/core/search.hh"
template<typename T, typename F = decltype(DefaultComparator<T>)>
Optional<Result<T>> binary(T const& value, ArrayRef<T> const& container, F cmp = DefaultComparator<T>)
{
    i64 low = 0;
    i64 high = i64(container.length()) - 1;

    while (low <= high) {

        i64 mid = (low + (high - low) / 2);

        if (cmp(container[mid], value) == 0) {
            return Result(mid, container[mid]);
        } else if (cmp(container[mid], value) < 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return None;
}






}

}
# 66 "./include/commons/core.hh" 2
# 1 "./include/commons/core/predicates.hh" 1
# 25 "./include/commons/core/predicates.hh"
namespace cm {


#pragma GCC diagnostic push
# 28 "./include/commons/core/predicates.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 28 "./include/commons/core/predicates.hh"
            ;


constexpr auto Length(auto const& val)
{
    using T = decltype(val);
    if constexpr (IsPointer<T> && IsUnderlyingTypeOneOf<T, char*, wchar_t*, char8_t*, char16_t*, char32_t*>) {
        if (sizeof(*val) < 2) {
            return __builtin_strlen(val);
        } else if (sizeof(*val) == sizeof(wchar_t)) {
            return __builtin_wcslen(val);
        } else {
            unsigned long long len = 0;
            while (*val++ != 0) {
                len++;
            }
            return len;
        }
    } else {
        return val.length();
    }
}

template<typename T, unsigned N>
constexpr auto Length(T const (&)[N])
{
    if constexpr (IsUnderlyingTypeOneOf<T, char, wchar_t, char8_t, char16_t, char32_t>) {
        return max(0L, long(N) - 1L);
    } else {
        return N;
    }
}

constexpr auto FirstElement(auto&& elements)
{
    if constexpr (IsIterable<decltype(elements)>) {
        return *elements.begin();
    } else {
        return elements[0];
    }
}

constexpr auto LastElement(auto&& elements)
{
    if constexpr (IsIterable<decltype(elements)>) {
        return *elements.end();
    } else {
        return elements[Length(elements) - 1];
    }
}

constexpr auto Contains(auto&& elements, auto const& val)
{
    if constexpr (IsIterable<decltype(elements)>) {
        for (auto const& element : elements) {
            if (element == val) {
                return true;
            }
        }
        return false;
    } else {
        for (decltype(Length(elements)) i = {}; i < Length(elements); i++) {
            if (elements[i] == val) {
                return true;
            }
        }
        return false;
    }
}

#pragma GCC diagnostic pop
# 98 "./include/commons/core/predicates.hh"
          ;

namespace Predicate {
template<auto Val>
struct StartsWith
{
    constexpr static bool operator()(auto const& iterable) { return FirstElement(iterable) == Val; }
};
template<auto Val>
struct EndsWith
{
    constexpr static bool operator()(auto const& iterable) { return LastElement(iterable) == Val; }
};
template<auto Val>
struct Contains
{
    constexpr static bool operator()(auto const& iterable, auto const& val) { return Contains(iterable, val); }
};
}


template<auto Val>
constexpr inline Predicate::StartsWith<Val> StartsWith;
template<auto Val>
constexpr inline Predicate::EndsWith<Val> EndsWith;

}
# 67 "./include/commons/core.hh" 2




namespace cm {}
# 32 "./include/commons/system.hh" 2
# 1 "./include/commons/datastructs.hh" 1
# 33 "./include/commons/datastructs.hh"
# 1 "./include/commons/datastructs/collection.hh" 1
# 18 "./include/commons/datastructs/collection.hh"
namespace cm {

namespace CollectionMethods {
# 65 "./include/commons/datastructs/collection.hh"
template<typename Inheritor, bool InheritorIsClass = IsClass<typename Inheritor::ItsElementType>>
struct Map;

template<typename Inheritor>
struct Map<Inheritor, false>
{
    template<typename T>
    using Derived = Inheritor::template ItsType<T>;
    using ElementType = typename Inheritor::ItsElementType;
# 82 "./include/commons/datastructs/collection.hh"
    auto& map(auto fnMapper, Derived<decltype(fnMapper(declval<ElementType>()))> const& out = {}) const
    {

        static_cast<Derived<ElementType> const&>(*this).forEach(
            [&](ElementType const& e) { const_cast<ConstRemoved<decltype(out)>&>(out) += fnMapper(e); });

        return const_cast<ConstRemoved<decltype(out)>&>(out);
    }
};


template<typename Inheritor>
struct Map<Inheritor, true> : Map<Inheritor, false>
{
    template<typename T>
    using Derived = Inheritor::template ItsType<T>;
    using ElementType = typename Inheritor::ItsElementType;
# 107 "./include/commons/datastructs/collection.hh"
    auto&
    map(auto (ElementType::*method)() const,
        Derived<decltype((declval<ElementType>().*(method))())> const& out = {}) const
    {

        static_cast<Derived<ElementType> const&>(*this).forEach(
            [&](ElementType const& e) { const_cast<ConstRemoved<decltype(out)>&>(out) += (e.*(method))(); });

        return const_cast<ConstRemoved<decltype(out)>&>(out);
    }
};

template<typename Inheritor>
struct Join
{
    template<typename T>
    using Derived = Inheritor::template ItsType<T>;
    using ElementType = typename Inheritor::ItsElementType;







    auto join(auto const& delim) const
    {
        ElementType result;
        auto derived = static_cast<Derived<ElementType> const&>(*this);
        auto i = 0ull;

        derived.forEach([&](ElementType const& e) {
            result += e;
            if (i < derived.length() - 1) {
                result += delim;
            }
            i++;
        });
# 157 "./include/commons/datastructs/collection.hh"
        return result;
    }
};

template<typename Inheritor>
class Invert {
    using ElementType = typename Inheritor::ItsElementType;
    using Derived = typename Inheritor::template ItsType<ElementType>;

public:
    auto& invert()
    {

        auto len = static_cast<Derived&>(*this).length();

        if (len != 0) {
            auto l = static_cast<Derived&>(*this).begin();
            auto r = static_cast<Derived&>(*this).end();
            --r;

            for (auto i = decltype(len){}; i < (len / 2); i++) {
                auto tmp = *l;
                *l = *r;
                *r = tmp;
                --r;
                ++l;
            }
        }
        return static_cast<Derived&>(*this);
    }
};

}


template<typename Inheritor>
struct Collection : CollectionMethods::Map<Inheritor>,
                    CollectionMethods::Join<Inheritor>,
                    CollectionMethods::Invert<Inheritor>
{


    using ElementType = typename Inheritor::ItsElementType;
    using Derived = typename Inheritor::template ItsType<ElementType>;
    using RefElementType = ElementType&;


    void toString(String& res) const;
# 246 "./include/commons/datastructs/collection.hh"
};
}
# 34 "./include/commons/datastructs.hh" 2
# 1 "./include/commons/datastructs/array.hh" 1
# 20 "./include/commons/datastructs/array.hh"
namespace cm {

#pragma GCC diagnostic push
# 22 "./include/commons/datastructs/array.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"

constexpr inline u32 ARRAY_LENGTH_UNSPECIFIED = 0xffffffff;


template<typename T, u32 L = ARRAY_LENGTH_UNSPECIFIED>
class Array;

namespace CollectionType {
template<typename T, u32 L = ARRAY_LENGTH_UNSPECIFIED>
class Array {
public:
    using ItsElementType = T;

    template<typename SomeType = T>
    using ItsType = ::cm::Array<SomeType, L>;
};
}

template<typename T, u32 L>
struct ArrayBase
{
    T _data[L];
};

template<typename T>
struct ArrayBase<T, ARRAY_LENGTH_UNSPECIFIED>
{
    T* _data = nullptr;
    usize _length = 0;
};






template<typename T, u32 L>
class Array : protected ArrayBase<T, L>,
              public LinearIteratorComponent<Array<T, L>, T>,
              public Collection<CollectionType::Array<T, L>> {
protected:
    using Base = ArrayBase<T, L>;

public:
    using ElementType = T;




    Array() = default;




    ~Array()
    {
        if constexpr (L == ARRAY_LENGTH_UNSPECIFIED) {
            if (Base::_data) {
                delete[] Base::_data;
            }
        }
    }




    Array(Index const& len) requires ((L == ARRAY_LENGTH_UNSPECIFIED))
    {
        usize len_ = len.assertPositive();
        Base::_data = new T[len_]{};
        Base::_length = len_;
    }




    Array(T* ptr, usize len)
    {
        if constexpr (L == ARRAY_LENGTH_UNSPECIFIED) {
            Base::_data = new T[len];
            Base::_length = len;
            for (usize i = 0; i < len; i++)
                Base::_data[i] = ptr[i];

        } else {
            for (usize i = 0; i < min(L, len); i++)
                Base::_data[i] = ptr[i];
        }
    }




    Array(std::initializer_list<T> const& v)
    {
        if constexpr (L == ARRAY_LENGTH_UNSPECIFIED) {
            Base::_data = new T[v.size()];
            Base::_length = v.size();
        } else {
            Assert(v.size() <= L, "Invalid parameter \"""v""\"");
        }
        for (usize i = 0; i < v.size(); i++)
            Base::_data[i] = v.begin()[i];
    }




    template<unsigned N>
    Array(T const (&values)[N])
    {
        if constexpr (L == ARRAY_LENGTH_UNSPECIFIED) {
            Base::_data = new T[N];
            Base::_length = N;
        } else {
            static_assert(N <= L, "Too large");
        }
        for (unsigned int i = 0; i < N; i++)
            Base::_data[i] = values[i];
    }




    Array(Array<T, L> const& other)
    {
        if constexpr (L == ARRAY_LENGTH_UNSPECIFIED) {
            Base::_data = new T[other.length()];
            Base::_length = other.length();
        }
        for (usize i = 0; i < other.length(); i++) {
            Base::_data[i] = other._data[i];
        }
    }




    constexpr Array(Array<T, L>&& other) noexcept
    {
        Base::_data = other._data;
        Base::_length = other._length;
        other._data = nullptr;
        other._length = 0;
    }




    constexpr Array& operator=(Array<T, L> const& other)
    {
        this->~Array<T, L>();
        new (this) Array<T, L>(other);
        return *this;
    }




    constexpr Array& operator=(Array<T, L>&& other)
    {
        this->~Array<T, L>();
        new (this) Array<T, L>(other);
        return *this;
    }





    Array<T> times(Index const& count)
    {
        usize n = count.assertPositive();
        auto newArray = Array<T>(this->length() * n);

        for (usize i = 0; i < newArray.length(); i += this->length()) {
            for (usize j = 0; j < this->length(); j++) {
                newArray[i + j] = (*this)[j];
            }
        }
        return newArray;
    }





    constexpr T const& operator[](Index const& i) const [[clang::lifetimebound]]
    {
#pragma GCC diagnostic push
# 212 "./include/commons/datastructs/array.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 212 "./include/commons/datastructs/array.hh"
                    ;
        return this->_data[i.compute(*this)];
#pragma GCC diagnostic pop
# 214 "./include/commons/datastructs/array.hh"
                  ;
    }





    constexpr T& operator[](Index const& i) [[clang::lifetimebound]]
    {
        return const_cast<T&>(static_cast<Array const*>(this)->operator[](i));
    }




    constexpr inline auto data([[clang::lifetimebound]] this auto&& self) noexcept { return &self._data[0]; }




    constexpr inline auto length() const
    {
        if constexpr (L != ARRAY_LENGTH_UNSPECIFIED) {
            return L;
        } else {
            return Base::_length;
        }
    }




    constexpr inline auto sizeBytes() const { return length() * sizeof(T); }





    constexpr void forEach(this auto&& self, auto visitor)
    {
        for (auto& e : self) {
            visitor(e);
        }
    }






    constexpr static auto outputString(Array<T, L> const& array, auto const& out)
    {
        ArrayRef<T>(array).outputString(array, out);
    }






    template<typename Self>
    constexpr auto set(this Self&& self, auto const& indices, auto const& setter)
    {
        auto const actualSetter = [&](auto const& index) {
            if constexpr (IsCallableWith<decltype(setter), decltype(index), Self>) {
                return setter(index, self);
            } else {
                return setter(index);
            }
        };
        if constexpr (IsMutable<Self>) {
            if constexpr (IsIterable<decltype(indices)>) {
                for (usize index : indices) {
                    self[index] = actualSetter(index);
                }
            } else {
                self[indices] = actualSetter(indices);
            }
            return self;
        } else {
            return Array(self).set(indices, setter);
        }
    }
# 305 "./include/commons/datastructs/array.hh"
    template<typename Self>
    constexpr auto set(this Self&& self, auto const& rows, auto const& columns, auto const& setter)
    {
        auto const actualSetter = [&](auto const& row, auto const& col) {
            if constexpr (IsCallableWith<decltype(setter), decltype(row), decltype(col), Self>) {
                return setter(row, col, self);
            } else {
                return setter(row, col);
            }
        };
        if constexpr (IsMutable<Self>) {
            if constexpr (IsIterable<decltype(rows)> && IsIterable<decltype(columns)>) {
                for (auto const& row : rows) {
                    for (auto const& col : columns) {
                        self[row][col] = actualSetter(row, col);
                    }
                }
            } else if constexpr (IsIterable<decltype(rows)> && !IsIterable<decltype(columns)>) {
                for (auto row : rows) {
                    self[row][columns] = actualSetter(row, columns);
                }
            } else if constexpr (!IsIterable<decltype(rows)> && IsIterable<decltype(columns)>) {
                for (auto col : columns) {
                    self[rows][col] = actualSetter(rows, col);
                }
            } else {
                self[rows][columns] = actualSetter(rows, columns);
            }
            return self;
        } else {
            return Array(self).set(rows, columns, setter);
        }
    }




    constexpr auto element(this auto&& self, usize row) { return self[row]; }




    constexpr auto element(this auto&& self, usize row, usize col) { return self[row][col]; }




    constexpr auto& operator()(Index i) noexcept [[clang::lifetimebound]]
    {
#pragma GCC diagnostic push
# 354 "./include/commons/datastructs/array.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 354 "./include/commons/datastructs/array.hh"
        return Base::_data[i.computeUnchecked(*this)]
# 354 "./include/commons/datastructs/array.hh"
#pragma GCC diagnostic pop
# 354 "./include/commons/datastructs/array.hh"
                                                             ;
    }

    constexpr auto const& operator()(Index i) const noexcept [[clang::lifetimebound]]
    {
#pragma GCC diagnostic push
# 359 "./include/commons/datastructs/array.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 359 "./include/commons/datastructs/array.hh"
        return Base::_data[i.computeUnchecked(*this)];
# 359 "./include/commons/datastructs/array.hh"
#pragma GCC diagnostic pop
    }




    constexpr auto const& operator()(Index row, auto const& col) const [[clang::lifetimebound]]
    {
#pragma GCC diagnostic push
# 367 "./include/commons/datastructs/array.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 367 "./include/commons/datastructs/array.hh"
        return Base::_data[row.computeUnchecked(*this)][col];
# 367 "./include/commons/datastructs/array.hh"
#pragma GCC diagnostic pop
    }
};


template<typename T>
Array(std::initializer_list<T> const&) -> Array<T, ARRAY_LENGTH_UNSPECIFIED>;
# 386 "./include/commons/datastructs/array.hh"
template<typename T>
constexpr auto Array2D(auto rows, auto cols)
{
    return Array({Array<T>(cols)}).times(rows);
}

}

#pragma GCC diagnostic pop
# 35 "./include/commons/datastructs.hh" 2
# 1 "./include/commons/datastructs/fixed_stack.hh" 1
# 21 "./include/commons/datastructs/fixed_stack.hh"
namespace cm {






template<typename T, usize Capacity = ARRAY_LENGTH_UNSPECIFIED>
class FixedStack : private Array<T, Capacity> {
    using Base = Array<T, Capacity>;
    usize _size = 0;

public:
    constexpr FixedStack() noexcept = default;




    constexpr FixedStack(std::initializer_list<T> const& init)
        : Base(init), _size(init.size())
    {}




    constexpr bool push(T const& value)
    {
        if (_size >= Capacity) {
            return false;
        }
        Base::operator()(_size) = value;
        ++_size;
        return true;
    }





    constexpr Optional<T> pop() noexcept
    {
        if (_size == 0) {
            return None;
        }
        --_size;
        return Base::operator()(_size);
    }




    constexpr Optional<T> top() const noexcept
    {
        if (_size == 0) {
            return None;
        }
        return Base::operator()(_size - 1);
    }




    constexpr usize length() const noexcept { return _size; }
};

}
# 36 "./include/commons/datastructs.hh" 2
# 1 "./include/commons/datastructs/array_list.hh" 1
# 21 "./include/commons/datastructs/array_list.hh"
namespace cm {






struct ByteVector : IEquatable<ByteVector>
{
private:
    u8* _data;
    usize _length;
    usize _capacity;
    bool _needheap;

#pragma GCC diagnostic push
# 36 "./include/commons/datastructs/array_list.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 36 "./include/commons/datastructs/array_list.hh"
                 void _ensureDataOnHeap()
    {
        if (!_needheap)
            return;
        Assert(Ptr::isRomData(_data), ASMS_BAD_CIRCUMSTANCE);
        auto old = _data;
        _data = new u8[_capacity];
        memmove(_data, old, _capacity);
        _needheap = false;
#pragma GCC diagnostic pop
# 45 "./include/commons/datastructs/array_list.hh"
                  ;
    }

#pragma GCC diagnostic push
# 48 "./include/commons/datastructs/array_list.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 48 "./include/commons/datastructs/array_list.hh"
                 void _reallocate()
    {
        Assert(_capacity > 0, ASMS_BUG);
        auto old = _data;
        _data = new u8[size_t(_capacity)];
        memmove(_data, old, _length);
        delete[] old;
#pragma GCC diagnostic pop
# 55 "./include/commons/datastructs/array_list.hh"
                  ;
    }

public:
    constexpr ByteVector() = default;




    inline ByteVector(void const* ptr, usize len) noexcept
    {
        Assert(ptr, "Invalid parameter \"""ptr""\"");
        Assert(len, "Invalid parameter \"""len""\"");
        if (Ptr::isRomData(ptr)) {


            _data = static_cast<u8*>(const_cast<void*>(ptr));
            _length = _capacity = len;
            _needheap = true;
        } else {
            insert(0, ptr, len);
        }
    }




    inline ByteVector(ByteVector const& other) noexcept
    {
        if (other._data) {
            if (Ptr::isRomData(other._data)) {
                _data = const_cast<u8*>(other._data);
                _length = _capacity = other._length;
                _needheap = true;
            } else {
                insert(0, other._data, other._length);
            }
        }
    }




    inline ByteVector(ByteVector&& other) noexcept
    {
        _data = other._data;
        _length = other._length;
        _capacity = other._capacity;
        _needheap = other._needheap;
        other._data = nullptr;
    }




    inline ByteVector& operator=(ByteVector const& other) noexcept
    {
        clear();
        new (this) ByteVector(other);
        return *this;
    }




    inline ByteVector& operator=(ByteVector&& other) noexcept
    {
        clear();
        new (this) ByteVector(other);
        return *this;
    }




    inline ~ByteVector() { clear(); }




    inline bool equals(ByteVector const& other) const
    {
        if (this == &other)
            return true;
        if (length() != other.length())
            return false;
        return ArrayRef<u8>(this->_data, this->length()).equals(ArrayRef<u8>(other._data, other.length()));
    }

    void append(u8 byte);
    void append(ByteVector const& other);

#pragma GCC diagnostic push
# 147 "./include/commons/datastructs/array_list.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 147 "./include/commons/datastructs/array_list.hh"
                 void insert(size_t index, void const* bytes, size_t nBytes)
    {
        Assert(index <= _length, "Invalid parameter \"""index""\"");
        Assert(bytes, "Invalid parameter \"""bytes""\"");
        if (nBytes == 0) {
            return;
        }
        _ensureDataOnHeap();
        if (_length + nBytes >= _capacity) {
            auto prevCapacity = _capacity;
            _capacity = _capacity + (_capacity / 2);
            if (_capacity < prevCapacity + nBytes) {
                _capacity += nBytes + 2;
            }
            _reallocate();
        }
        _length += nBytes;

        memmove(&_data[index + nBytes], &_data[index], (_length - (index + nBytes)));

        memmove(&_data[index], bytes, nBytes);
#pragma GCC diagnostic pop
# 168 "./include/commons/datastructs/array_list.hh"
                  ;
    }

#pragma GCC diagnostic push
# 171 "./include/commons/datastructs/array_list.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 171 "./include/commons/datastructs/array_list.hh"
                 void erase(size_t index, size_t nBytes)
    {
        Assert(size_t((index + nBytes)) <= length());
        _ensureDataOnHeap();

        memmove(&_data[index], &_data[index + nBytes], _length - (index + nBytes));

        memset(&_data[_length - nBytes], 0, nBytes);
        _length -= nBytes;
        if (_length > 16 && _length < (_capacity / 4)) {
            _capacity = max(16u, _capacity / 2);
            _reallocate();
        }
#pragma GCC diagnostic pop
# 184 "./include/commons/datastructs/array_list.hh"
                  ;
    }

    void clear()
    {
        if (!_needheap && _data)
            delete[] _data;
        _capacity = 0;
        _length = 0;
        _data = nullptr;
    }

    [[gnu::always_inline]] inline size_t length() const { return _length; }
    [[gnu::always_inline]] inline bool empty() const { return _length == 0; }
    [[gnu::always_inline]] inline u8 const* data() const { return _data; }
    [[gnu::always_inline]] inline u8* data() { return _data; }
};
# 247 "./include/commons/datastructs/array_list.hh"
template<typename T>
struct StructVector : private ByteVector
{
    StructVector() = default;

    [[gnu::always_inline]] inline void append(T const& value)
    {
        ByteVector::insert(ByteVector::length() - sizeof(value), &value, sizeof(value));
    }

    [[gnu::always_inline]] inline void repeat(size_t n)
    {
        while (n-- != 0) {
            ByteVector::append(*this);
        }
    }


};


}
# 37 "./include/commons/datastructs.hh" 2
# 1 "./include/commons/datastructs/queue.hh" 1
# 25 "./include/commons/datastructs/queue.hh"
namespace cm {






template<typename T, usize Capacity = ARRAY_LENGTH_UNSPECIFIED>
struct FixedQueue
    : public Iterable<FixedQueue<T, Capacity>>,
      public LinearIteratorComponent<FixedQueue<T, Capacity>, Optional<T>>
{
private:
    Array<Optional<T>, Capacity> _array;
    usize _next = 0;
    usize _first = 0;
    usize _length = 0;

public:






    inline FixedQueue(usize cap, std::initializer_list<T> const& initializer)
        requires (Capacity == ARRAY_LENGTH_UNSPECIFIED)
        : _array([&] {
              Assert(cap >= initializer.size(), "Invalid parameter \"""cap""\"");
              return cap;
          }())
    {
        for (auto const& value : initializer) {
            push(value);
        }
    }





    constexpr inline FixedQueue(std::initializer_list<T> const& initializer)
        requires (Capacity != ARRAY_LENGTH_UNSPECIFIED)
    {
        Assert(initializer.size() <= Capacity, "Invalid parameter \"""initializer""\"");
        for (auto const& value : initializer) {
            push(value);
        }
    }

    void push(T const& value)
    {
        _array[_next] = value;

        if (_next == _array.length() - 1) {
            _next = 0;
        } else {
            ++_next;
        }
        _length = ::cm::min(_length + 1, _array.length());
    }

    Optional<T> pop()
    {
        if (length() == 0) {
            return None;
        } else {
            auto tmp = _first;
            if (_first == _array.length() - 1) {
                _first = 0;
            } else {
                ++_first;
            }
            --_length;

            Optional<T> opt = _array[tmp];
            _array[tmp] = None;
            return opt;
        }
    }

    inline Optional<T> const& operator[](usize index) const [[clang::lifetimebound]]
    {
        Assert(index < length(), ASMS_BOUNDS);
        auto tmp = _first;
        if (_first + index > _array.length()) {
            tmp = (_first + index) % _array.length();
        }
        return _array[tmp];
    }

    inline Optional<T>& operator[](usize index) [[clang::lifetimebound]]
    {
        return const_cast<Optional<T>&>(static_cast<FixedQueue const&>(*this)[index]);
    }

    inline usize length() const { return _length; }
    inline usize capacity() const { return _array.length(); }
};

}
# 38 "./include/commons/datastructs.hh" 2
# 1 "./include/commons/datastructs/string.hh" 1
# 21 "./include/commons/datastructs/string.hh"
namespace cm {


class String;

struct Printable
{
    virtual ~Printable() = default;
    virtual void output(String& result) const = 0;
};

template<typename T>
struct PrintableT final : Printable
{
    T* ptr;

    PrintableT(T const& ref)
        : ptr(&const_cast<T&>(ref))
    {}
    constexpr void output(String& result) const override;
};

template<typename T>
[[maybe_unused]] PrintableT(T const&) -> PrintableT<T>;






class String : public LinearIteratorComponent<String, char>,
               public Iterable<String>,
               public IEquatable<String> {

    ByteVector _data;

public:
    constexpr static auto FORMAT_DELIMITER = '`';
    using Index = Union<usize, isize> const&;





    template<long long N>
    String(char const (&cstring)[N])
        : String(cstring, ::cm::max(N - 1, 0))
    {}

    constexpr String()
        : _data("", 1)
    {}

    constexpr String(char c)
        : _data(Data::oneCharStringTable(c), 1)
    {}

    constexpr String(char const* str)
        : String(StringRef(str))
    {}

    constexpr String(char const* cstring, usize len)
        : _data(cstring, len + 1)
    {}

    constexpr String(StringRef const& sv)
        : String(sv.data(), sv.length())
    {}

    [[nodiscard]] String(String const&) = default;
    [[nodiscard]] String& operator=(String const&) = default;

    [[nodiscard]] operator ArrayRef<char>() { return ArrayRef<char>(const_cast<char*>(cstr()), length()); }
    [[nodiscard]] operator ArrayRef<char>() const { return ArrayRef<char>(const_cast<char*>(cstr()), length()); }
    [[nodiscard]] operator StringRef() { return StringRef(const_cast<char*>(cstr()), length()); }
    [[nodiscard]] operator StringRef() const { return StringRef(const_cast<char*>(cstr()), length()); }

    [[nodiscard]] inline usize getActualIndex(Index i) const
    {
        return i.match(
            [&](usize k) { return k; },
            [&](isize k) {
                return k < 0 ? usize(isize(length()) + k) : usize(k);
            });
    }

    [[nodiscard]] char& operator[](Index i)
    {
#pragma GCC diagnostic push
# 109 "./include/commons/datastructs/string.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 109 "./include/commons/datastructs/string.hh"
        u8* r = &_data.data()[getActualIndex(i) * sizeof(char)]
# 109 "./include/commons/datastructs/string.hh"
#pragma GCC diagnostic pop
# 109 "./include/commons/datastructs/string.hh"
                                                                       ;
        return *reinterpret_cast<char*>(r);
    }

    [[nodiscard]] char operator[](Index i) const
    {
#pragma GCC diagnostic push
# 115 "./include/commons/datastructs/string.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 115 "./include/commons/datastructs/string.hh"
        u8 const* r = &_data.data()[getActualIndex(i) * sizeof(char)]
# 115 "./include/commons/datastructs/string.hh"
#pragma GCC diagnostic pop
# 115 "./include/commons/datastructs/string.hh"
                                                                             ;
        return *reinterpret_cast<char const*>(r);
    }

    [[nodiscard]] constexpr usize length() const { return _data.length() - 1; }

    [[nodiscard]] constexpr usize sizeBytes() const { return length() * sizeof(char); }

    [[nodiscard]] constexpr bool empty() const { return length() == 0; }

    [[nodiscard]] constexpr char const* data() const { return reinterpret_cast<char const*>(_data.data()); }

    [[nodiscard]] constexpr char const* cstr() const { return this->data(); }

    [[nodiscard]] constexpr bool equals(StringRef value) const { return StringRef(*this).equals(value); }

    void ensureNullTermination();

    void erase(Index i, usize n) &
    {
        usize actualIndex = getActualIndex(i);
        _data.erase(actualIndex * sizeof(char), n * sizeof(char));
    }

    void insert(Index i, StringRef const& s) &
    {
        usize actualIndex = getActualIndex(i);
        _data.insert(actualIndex * sizeof(char), s.data(), s.sizeBytes());
    }


    void insertf(Index i, u64 value);





    template<unsigned N, typename... Args>
    [[nodiscard]] [[gnu::always_inline]] inline static String fmt(char const (&sFmt)[N], Args... args)
    {
        return _fmt(sFmt, {PrintableT(args)...});
    }






    [[gnu::always_inline]] inline void append(StringRef s) & { return insert(length(), s); }
    [[gnu::always_inline]] inline void push(StringRef s) & { return insert(length(), s); }
    inline char pop() &
    {
        char c = (*this)[-1];
        erase(-1, 1);
        return c;
    }

    [[nodiscard]] [[gnu::always_inline]] inline String replace(StringRef s, StringRef r) const { return String(*this).replace(s, r); }
    [[nodiscard]] [[gnu::always_inline]] inline String replace(StringRef s, StringRef r) && { return (replace(s, r), *this); }

    void replace(StringRef substr, StringRef replacement) &
    {
        usize baseIndex = 0;
        Optional<usize> index = None;
        while ((index = this->find(substr, baseIndex)) != None) {
            erase(index.value(), substr.length());
            insert(index.value(), replacement);
            baseIndex += replacement.length();
        }
    }


    [[nodiscard]] [[gnu::always_inline]] inline String erase(Index i, usize n) const { return String(*this).erase(i, n); }
    [[nodiscard]] [[gnu::always_inline]] inline String erase(Index i, usize n) && { return (erase(i, n), *this); }

    [[nodiscard]] [[gnu::always_inline]] inline String insert(Index i, StringRef s) const { return String(*this).insert(i, s); }
    [[nodiscard]] [[gnu::always_inline]] inline String insert(Index i, StringRef s) && { return (insert(i, s), *this); };

    [[nodiscard]] [[gnu::always_inline]] inline String append(StringRef s) const { return String(*this).append(s); }
    [[nodiscard]] [[gnu::always_inline]] inline String append(StringRef s) && { return (append(s), *this); }







    void removeSuffix(char refValue) &
    {
        if (length() != 0 && (*this)[length() - 1] == refValue) {
            erase(length() - 1, 1);
        }
    }





    constexpr StringRef slice(usize startIndex, usize endIndex) const
    {
#pragma GCC diagnostic push
# 215 "./include/commons/datastructs/string.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 215 "./include/commons/datastructs/string.hh"
                    ;
        Assert(endIndex <= length());
        Assert(startIndex < endIndex);
        return StringRef(this->data() + startIndex, endIndex - startIndex);
#pragma GCC diagnostic pop
# 219 "./include/commons/datastructs/string.hh"
                  ;
    }

public:
    template<int = 0>
    long long toInteger() const
    {

        auto result = 0, negate = 0;
        auto s = begin();
        while (*s == ' ' || *s == '\t') {
            ++s;
        }
        if (*s == '+' && *(s.next()) != '-') {
            ++s;
        }
        if (*s == '-') {
            negate = 1;
            ++s;
        }
        for (; *s != '\0'; ++s) {
            switch (*s) {
            case '0' ... '9':
                if (__builtin_mul_overflow(result, 10, &result) || __builtin_add_overflow(result, -('0' - *s), &result))
                    return negate ? -2147483648 : 2147483647;
                continue;
            default: break;
            }
            break;
        }
        return negate ? -result : result;
    }


    template<int = 0>
    double toDouble() const
    {
        if (*this == "-inf") {
            return -__builtin_huge_val();
        } else if (*this == "inf") {
            return __builtin_huge_val();
        } else if (*this == "NaN") {
            return __builtin_nan("");
        }
        auto it = begin();
        while (it != last() && *it != '.') {
            ++it;
        }
        auto wholePart = double(toInteger());

        String a;
        while (it != last()) {
            ++it;
            a.append(*it);
        }
        if (auto fracInt = a.toInteger(); fracInt != 0) {
            wholePart +=
                (double(fracInt) / (Double::pow(10.0, int(__builtin_floor(Double::log10(double(fracInt))))) * 10.0));
        }
        return wholePart;
    }

private:
    template<int = 0>
    static String _fmt(StringRef const& format, ArrayRef<RefWrapper<Printable const>> const& objects)
    {
        String result;
        auto fmtIter = format.begin();
        auto objIter = objects.begin();

        while (fmtIter.isNotEnd() && *fmtIter != '`') {
            result.append(*fmtIter);
            ++fmtIter;
        }
        while (objIter.isNotEnd()) {
            Assert(fmtIter != format.end() && *fmtIter == '`', "More arguments than specified in format string");
            String str;
            (*objIter)->output(str);
            result.append(str);
            ++objIter;
            ++fmtIter;
            while (fmtIter.isNotEnd() && *fmtIter != '`') {
                result.append(*fmtIter);
                ++fmtIter;
            }
        }
        return result;
    }
};


template<typename T>
constexpr inline void PrintableT<T>::output(String& result) const
{
    if constexpr (IsUnderlyingTypeOneOf<T, String, StringRef, char*, char>) {
        result.append(*ptr);
    } else {
        OutputString(*ptr, [&](char c) { result.append(c); });
    }
}


template<usize N>
struct FormatLiteral
{
    char fmt[N];


    constexpr inline FormatLiteral(char const (&str)[N])
    {
        for (usize i = 0; i < N; ++i)
            fmt[i] = str[i];
    }

    constexpr inline usize size() const { return N - 1; }


    template<typename... Args>
    inline String operator()(Args&&... args) const
    {

        return String::fmt(fmt, Forward<Args>(args)...);
    }
};

template<usize N>
FormatLiteral(char const (&str)[N]) -> FormatLiteral<N>;

template<FormatLiteral L>
constexpr inline auto operator""_fmt()
{
    return L;
}




}
# 39 "./include/commons/datastructs.hh" 2
# 1 "./include/commons/datastructs/linked_list.hh" 1
# 22 "./include/commons/datastructs/linked_list.hh"
namespace cm {

template<typename T>
struct DLList;




namespace impl {
class DLList {
protected:
    template<typename T>
    friend struct ::cm::DLList;

    struct Container
    {
        struct Node
        {
            Node* next;
            Node* prev;
        };

        struct Iterator
        {
            Container* _list;
            Node* _curr;

            void _remove();
            void _insert(void const* mem);
            void* _get();
            void const* _get() const;
            bool _hasNext() const { return _curr != _list->_tail; }
            Iterator _next() const { return {_list, _curr->next}; }
            bool _hasPrev() const { return _curr != _list->_head; }
            Iterator _prev() const { return {_list, _curr->prev}; }
        };

        ~Container();
        friend struct Iterator;

        void clear();
        Iterator _begin();
        Iterator _begin() const;
        Iterator _end();
        Iterator _end() const;

        Node* _head;
        Node* _tail;
        ClassRef _objclass;
        u32 _length;
    };
};
}





template<typename T>
struct DLList : protected impl::DLList::Container
{
    struct Iterator : protected impl::DLList::Container::Iterator
    {
        auto& insert(T const& value = {}) { return (_insert(&value), *this); }
        auto& remove() { return (_remove(), *this); }
        auto prev() { return Iterator(_list, _curr->prev); }
        auto next() { return Iterator(_list, _curr->next); }
        auto hasNext() { return _hasNext(); }
        auto hasPrev() { return _hasPrev(); }
        Iterator& operator++() { return (*this = this->next()); }
        bool operator==(Iterator const& i) const { return _curr == i._curr; }
        T const& operator*() const noexcept { return *static_cast<T const*>(_get()); }
        T& operator*() noexcept { return *static_cast<T*>(_get()); }
        T const* operator->() const noexcept { return static_cast<T const*>(_get()); }
        T* operator->() noexcept { return static_cast<T*>(_get()); }

    private:
        friend struct DLList<T>;
        Iterator(impl::DLList::Container const* l, impl::DLList::Container::Node* n)
            : impl::DLList::Container::Iterator{const_cast<Container*>(l), n}
        {}
    };

    DLList()
        : impl::DLList::Container{{}, {}, ClassOf<T>, {}}
    {}

    using impl::DLList::Container::clear;

    constexpr auto length() const { return _length; }
    constexpr auto begin() { return Iterator(this, _head); }
    constexpr auto end() { return Iterator(this, nullptr); }
    constexpr auto begin() const { return Iterator(this, _head); }
    constexpr auto end() const { return Iterator(this, nullptr); }

    constexpr auto const& first() const { return *begin(); }
    constexpr auto const& last() const { return *end().prev(); }
    constexpr auto& first() { return *begin(); }
    constexpr auto& last() { return *end().prev(); }

    constexpr auto forEach(auto func)
    {
        for (auto it = begin(); it != end(); ++it) {
            func(*it);
        }
    }
};


}

namespace cm::impl {



inline DLList::Container::~Container() { clear(); }



[[clang::noinline]]
inline void DLList::Container::clear()
{
    if (_objclass.destructor) {
        for (auto n = _head; n != nullptr;) {
            auto tmp = n->next;
#pragma GCC diagnostic push
# 147 "./include/commons/datastructs/linked_list.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 147 "./include/commons/datastructs/linked_list.hh"
            _objclass.destructor(reinterpret_cast<u8*>(n) + sizeof(Node))
# 147 "./include/commons/datastructs/linked_list.hh"
#pragma GCC diagnostic pop
# 147 "./include/commons/datastructs/linked_list.hh"
                                                                                 ;
            delete[] n;
            n = tmp;
        }
    } else {
        for (auto n = _head; n != nullptr;) {
            auto tmp = n->next;
            delete[] n;
            n = tmp;
        }
    }
    _head = _tail = nullptr;
    _length = 0;
}



[[clang::noinline]]
inline void DLList::Container::Iterator::_insert(void const* mem)
{
    Assert(mem);
    Node* newNode;

#pragma GCC diagnostic push
# 170 "./include/commons/datastructs/linked_list.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 170 "./include/commons/datastructs/linked_list.hh"
                ;
    {
        auto* newNodeBytes = new u8[sizeof(Node) + _list->_objclass.sizeBytes];
        memcpy(newNodeBytes + sizeof(Node), mem, _list->_objclass.sizeBytes);
        newNode = reinterpret_cast<Node*>(newNodeBytes);
    }
#pragma GCC diagnostic pop
# 176 "./include/commons/datastructs/linked_list.hh"
              ;

    if (_curr == nullptr) {
        newNode->prev = _list->_tail;
        newNode->next = nullptr;

        if (_list->_head == nullptr && _list->_tail == nullptr) {
            _list->_head = _list->_tail = newNode;
        } else {
            _list->_tail->next = newNode;
            _list->_tail = newNode;
        }
    } else {
        newNode->prev = _curr->prev;
        newNode->next = _curr;

        if (_curr->prev) {
            _curr->prev->next = newNode;
        }
        _curr->prev = newNode;
        if (_curr == _list->_head) {
            _list->_head = _curr->prev;
        }
    }
    _list->_length++;
    _curr = newNode;
}

[[clang::noinline]]
inline void DLList::Container::Iterator::_remove()
{
    Assert(_curr);
    Node* tmp;

    if (_curr->prev) {
        _curr->prev->next = _curr->next;
    }
    if (_curr->next) {
        _curr->next->prev = _curr->prev;
    }
    if (_curr == _list->_head) {
        _list->_head = _curr->next;
    }
    if (_curr == _list->_tail) {
        _list->_tail = _curr->prev;
    }
    _list->_length--;
    tmp = _curr->next;
    delete[] _curr;
    _curr = tmp;
}

inline auto DLList::Container::Iterator::_get() const -> void const*
{
    Assert(_curr);
    return
# 231 "./include/commons/datastructs/linked_list.hh"
#pragma GCC diagnostic push
# 231 "./include/commons/datastructs/linked_list.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 231 "./include/commons/datastructs/linked_list.hh"
           reinterpret_cast<u8 const*>(_curr) + sizeof(Node)
# 231 "./include/commons/datastructs/linked_list.hh"
#pragma GCC diagnostic pop
# 231 "./include/commons/datastructs/linked_list.hh"
                                                                    ;
}

inline auto DLList::Container::Iterator::_get() -> void*
{
    Assert(_curr);
    return
# 237 "./include/commons/datastructs/linked_list.hh"
#pragma GCC diagnostic push
# 237 "./include/commons/datastructs/linked_list.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 237 "./include/commons/datastructs/linked_list.hh"
           reinterpret_cast<u8*>(_curr) + sizeof(Node)
# 237 "./include/commons/datastructs/linked_list.hh"
#pragma GCC diagnostic pop
# 237 "./include/commons/datastructs/linked_list.hh"
                                                              ;
}

}
# 40 "./include/commons/datastructs.hh" 2
# 1 "./include/commons/datastructs/sparse_array.hh" 1
# 20 "./include/commons/datastructs/sparse_array.hh"
namespace cm {

template<typename Type>
struct SparseArray;




namespace impl {
class SparseArray {
protected:
    template<typename T>
    friend struct ::cm::SparseArray;


    struct Container
    {
        void _clear();
        void* _get(void const* pIndex) const;

        void** _data;
        ClassRef _objclass;
        usize _length;
    };
};
}


struct SparseArrayBase
{};
# 59 "./include/commons/datastructs/sparse_array.hh"
#pragma GCC diagnostic push
# 59 "./include/commons/datastructs/sparse_array.hh"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
template<typename Type>
struct SparseArray
{
    Type** data;
    usize _length;


public:
    SparseArray() { this->data = new Type*[256]{}; }

    ~SparseArray() { clear(); }

    void clear()
    {
        auto ptr = this->data;

        auto _ = [&](this auto const& next_, unsigned depth) -> void {
            if (depth == 0) {
                for (int k = 0; k < 256; k++) {
                    if (ptr[k]) {
                        delete ptr;
                    }
                }
            } else {
                for (int k = 0; k < 256; k++) {
                    if (ptr[k] != nullptr) {
                        ptr = reinterpret_cast<Type**>(ptr[k]);
                        next_(depth - 1);
                        delete[] ptr;
                    }
                }
            }
        };
        _(8);
    }


    void set(u64 index, Type const& value) noexcept
    {
        auto ptr = this->data;

        for (int _ = 0; _ < 7; _++, index >>= 8) {
            auto i = static_cast<u8>(index);
            ptr[i] = !ptr[i] ? reinterpret_cast<Type*>(new Type*[256]{}) : ptr[i];
            ptr = reinterpret_cast<Type**>(ptr[i]);
        }
        auto i = static_cast<u8>(index);
        if (!ptr[i]) {
            ptr[i] = new Type();
            _length++;
        }
        *ptr[i] = value;
    }

    Optional<Type> get(u64 index) const noexcept
    {
        auto ptr = this->data;

        for (int _ = 0; _ < 7; _++, index >>= 8) {
            if (ptr == nullptr)
                return None;
            ptr = reinterpret_cast<Type**>(ptr[static_cast<u8>(index)]);
        }
        auto i = static_cast<u8>(index);
        return !ptr[i] ? None : Optional<Type>(*ptr[i]);
    }

    void removeAt(u64 index)
    {
        auto ptr = this->data;
        for (int _ = 0; _ < 7; _++, index >>= 8) {
            if (ptr == nullptr)
                return;
            ptr = reinterpret_cast<Type**>(ptr[static_cast<u8>(index)]);
        }
        auto i = static_cast<u8>(index);
        if (ptr[i]) {
            delete ptr[i];
        }
        if (_length > 0)
            _length--;
    }

    void forEach(auto visitor)
    {
        auto ptr = this->data;

        auto _ = [&](this auto const& next_, unsigned depth) -> void {
            if (depth == 0) {
                for (int k = 0; k < 256; k++) {
                    if (ptr[k])
                        visitor(ptr);
                }
            } else {
                for (int k = 0; k < 256; k++) {
                    if (ptr[k] != nullptr) {
                        ptr = reinterpret_cast<Type**>(ptr[k]);
                        return next_(depth - 1);
                    }
                }
            }
        };
        _(8);
    }

    constexpr static auto INDEX_BITS = 64;
    constexpr static auto GRANULARITY = 256;


    struct Iterator
    {};


    Iterator begin() {}
};


}
#pragma GCC diagnostic pop
# 41 "./include/commons/datastructs.hh" 2
# 1 "./include/commons/datastructs/map.hh" 1
# 20 "./include/commons/datastructs/map.hh"
namespace cm {
# 33 "./include/commons/datastructs/map.hh"
template<typename K, typename V>
struct Map
{
    using HashFunction = CFunction<u32(K const&)>;
    constexpr static u32 defaultHashFunction(K const& k) { return Hash<Crc32>::hash(k); };


    Map(HashFunction const& hashFunc = defaultHashFunction)
        : _array(), _hashFunc(hashFunc)
    {}


    void put(K const& key, V const& value)
    {
        u32 keyHash = _hashFunc(key);
        u64 index = u64(keyHash) << 32;

        for (u32 j = 0; j < MAX_VALUE<u32>; j++) {
            u64 offset = index + j;
            Optional<Pair<K, V>> entry = _array.get(offset);

            if (!entry.hasValue()) {
                _array.set(offset, Pair<K, V>(key, value));
                break;
            }
        }
    }

    Optional<V&> get(K const& key)
    {
        u32 keyHash = _hashFunc(key);
        u64 index = u64(keyHash) << 32;

        for (u32 j = 0; j < MAX_VALUE<u32>; j++) {
            u64 offset = index + j;
            Optional<Pair<K, V>> entry = _array.get(offset);

            if (!entry.hasValue()) {
                return None;
            } else if (entry.hasValue() && key == entry.value().first) {
                return entry.value().second;
            }
        }
        return None;
    }

private:
    SparseArray<Pair<K, V>> _array;
    HashFunction _hashFunc;
};


}
# 42 "./include/commons/datastructs.hh" 2
# 33 "./include/commons/system.hh" 2


# 1 "./include/commons/system/streamstatus.inl" 1
# 18 "./include/commons/system/streamstatus.inl"
namespace cm {




enum [[clang::flag_enum]] StreamStatus : u64 {
    STATUS_OK = 0,
    STATUS_FAILED_INIT = (1 << 0),
    STATUS_FAILED_WRITE = (1 << 1),
    STATUS_FAILED_FLUSH = (1 << 2),
    STATUS_NOT_ALL_BYTES_FLUSHED = (1 << 3),


    STATUS_ERR_INVALID = (1 << 4),


    STATUS_ERR_INTERRUPT = (1 << 5),


    STATUS_ERR_IO = (1 << 6),


    STATUS_ERR_UNKNOWN = (1 << 7),

    STATUS_ERR_ACCESS = (1 << 8),
    STATUS_ERR_BUSY = (1 << 9),
    STATUS_ERR_QUOTA = (1 << 10),

    STATUS_ERR_ALREADY_EXISTS = (1 << 11),


    STATUS_ERR_UNSUPPORTED = (1 << 12),

    STATUS_ERR_LOOP = (1 << 13),
    STATUS_ERR_LIMIT = (1 << 14),

    STATUS_ERR_NOT_FOUND = (1 << 15),
    STATUS_ERR_MEMORY = (1 << 16),



    STATUS_ANY_ERR = 0xffff'ffff'ffff'fffe,
};

}
# 36 "./include/commons/system.hh" 2
# 1 "./include/commons/system/outstream.inl" 1
# 18 "./include/commons/system/outstream.inl"
namespace cm {




template<typename Derived>
class IOutStream {
public:
    using Status = StreamStatus;




    constexpr static StringRef LS =





        "\n";


    constexpr ~IOutStream() = default;
    constexpr IOutStream() = default;
    constexpr IOutStream(IOutStream const&) = default;
    constexpr IOutStream& operator=(IOutStream const&) = default;






    inline Derived& writeBytes(void const* data, size_t sizeBytes)
    {
        return static_cast<Derived&>(*this).writeBytes(data, sizeBytes);
    }




    inline Derived& flush() { return static_cast<Derived&>(*this).flush(); }







    inline Result<Status, Status> close() { return static_cast<Derived&>(*this)->close(); }




    inline Status status() const { return static_cast<Derived&>(*this)->status(); }




    inline bool ok() { return status() == STATUS_OK; }
# 85 "./include/commons/system/outstream.inl"
    inline void print(auto const& value)
    {
        _print('`', ArrayRef<RefWrapper<Printable const>>{RefWrapper<Printable const>(PrintableT(value))});
    }
    template<int N>
    inline void print(char const (&str)[N])
    {
        writeBytes(str, max(N - 1, 0));
    }






    inline void print(StringRef const& sFmt, auto const&... args)
    {
        this->_print(sFmt, ArrayRef<RefWrapper<Printable const>>{(RefWrapper<Printable const>(PrintableT(args)))...});
    }







    inline void println(auto const& value)
    {
        this->print(value);
        this->print(LS);
    }
    template<int N>
    inline void println(char const (&str)[N])
    {
        this->writeBytes(str, max(N - 1, 0));
        this->writeBytes(LS.data(), LS.sizeBytes());
    }






    inline void println(StringRef sFmt, auto const&... args) { print(sFmt, args...), print(LS); }


private:
    void _print(StringRef const& format, ArrayRef<RefWrapper<Printable const>> const& objects)
    {
        auto fmtIter = format.begin();
        auto objIter = objects.begin();

        while (fmtIter.isNotEnd() && *fmtIter != '`') {
            char ch = *fmtIter;
            this->writeBytes(&ch, sizeof(char));
            ++fmtIter;
        }
        while (objIter.isNotEnd()) {
            Assert(fmtIter != format.end() && *fmtIter == '`', "More arguments than specified in format string");

            String str;
            (*objIter)->output(str);
            this->writeBytes(str.cstr(), str.sizeBytes());

            ++objIter;
            ++fmtIter;

            while (fmtIter.isNotEnd() && *fmtIter != '`') {
                char ch = *fmtIter;
                this->writeBytes(&ch, sizeof(char));
                ++fmtIter;
            }
        }
    }
};

}
# 37 "./include/commons/system.hh" 2
# 1 "./include/commons/system/stringstream.inl" 1
# 18 "./include/commons/system/stringstream.inl"
namespace cm {




struct StringStream : public IOutStream<StringStream>
{
    StringStream() = delete;
    StringStream(String& to)
        : _to(to)
    {}

    inline StringStream& writeBytes(void const* data, usize sizeBytes)
    {
        _to.insert(_to.length(), String(static_cast<char const*>(data), sizeBytes));
        return *this;
    }

    inline StringStream& flush() { return *this; }

private:
    String& _to;
};

}
# 38 "./include/commons/system.hh" 2
# 1 "./include/commons/system/listdir.inl" 1
# 18 "./include/commons/system/listdir.inl"
namespace cm {

struct DirectoryListGenerator
{
    StringRef path;

    struct Iterator : public NonCopyable
    {
        isize fd;
        u8 buf[1024];
        isize nread;
        isize bpos;
        StringRef curr;
        StreamStatus status;

        Iterator();
        Iterator(StringRef const& path);
        ~Iterator();
        bool operator==(Iterator const& other) const;
        Iterator& operator++();

        inline StringRef const& operator*() const { return curr; }
    };

    Iterator begin() const { return Iterator(path); }
    Iterator end() const { return Iterator(); }
};


}
# 39 "./include/commons/system.hh" 2
# 1 "./include/commons/system/shell.inl" 1
# 18 "./include/commons/system/shell.inl"
namespace cm {
# 28 "./include/commons/system/shell.inl"
}
# 40 "./include/commons/system.hh" 2



# 1 "./include/commons/system/linux/linuxapi.inl" 1
# 201 "./include/commons/system/linux/linuxapi.inl"
struct FILE;

extern "C" int errno;
extern "C" FILE* popen(char const* __command, char const* __modes);
extern "C" int pclose(FILE* __stream);
extern "C" int fgetc(FILE* __stream);
# 44 "./include/commons/system.hh" 2

namespace cm {
# 1 "./include/commons/system/linux/linuxsyscall.inl" 1
# 22 "./include/commons/system/linux/linuxsyscall.inl"
struct
{


# 1 "./include/commons/system/linux/syscall_linux_x64.inl" 1
# 20 "./include/commons/system/linux/syscall_linux_x64.inl"
#pragma GCC diagnostic push
# 20 "./include/commons/system/linux/syscall_linux_x64.inl"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 20 "./include/commons/system/linux/syscall_linux_x64.inl"
            ;





[[maybe_unused]] [[gnu::always_inline]] inline static u64 Syscall(int n) noexcept
{
    u64 ret;
    asm volatile("syscall" : "=a"(ret) : "a"(n) : "rcx", "r11", "memory");
    return ret;
}




[[maybe_unused]] [[gnu::always_inline]] inline static u64 Syscall(int n, auto p1) noexcept
{
    u64 ret;

    auto a1 = *reinterpret_cast<u64 const*>(&p1);
    asm volatile("syscall" : "=a"(ret) : "a"(n), "D"(a1) : "rcx", "r11", "memory");
    return ret;
}




[[maybe_unused]] [[gnu::always_inline]] inline static u64 Syscall(int n, auto p1, auto p2) noexcept
{
    u64 result;

    auto a1 = *reinterpret_cast<u64 const*>(&p1);
    auto a2 = *reinterpret_cast<u64 const*>(&p2);

    asm volatile("syscall" : "=a"(result) : "a"(n), "D"(a1), "S"(a2) : "rcx", "r11", "memory");
    return result;
}




[[maybe_unused]] [[gnu::always_inline]] inline static u64 Syscall(int n, auto p1, auto p2, auto p3) noexcept
{
    u64 result;

    auto a1 = *reinterpret_cast<u64 const*>(&p1);
    auto a2 = *reinterpret_cast<u64 const*>(&p2);
    auto a3 = *reinterpret_cast<u64 const*>(&p3);

    asm volatile("syscall" : "=a"(result) : "a"(n), "D"(a1), "S"(a2), "d"(a3) : "rcx", "r11", "memory");

    return result;
}




[[maybe_unused]] [[gnu::always_inline]] inline static u64 Syscall(int n, auto p1, auto p2, auto p3, auto p4) noexcept
{
    u64 result;

    auto a1 = *reinterpret_cast<u64 const*>(&p1);
    auto a2 = *reinterpret_cast<u64 const*>(&p2);
    auto a3 = *reinterpret_cast<u64 const*>(&p3);
    auto a4 = *reinterpret_cast<u64 const*>(&p4);

    register u64 r10 asm("r10") = a4;
    asm volatile("syscall" : "=a"(result) : "a"(n), "D"(a1), "S"(a2), "d"(a3), "r"(r10) : "rcx", "r11", "memory");

    return result;
}




[[maybe_unused]] [[gnu::always_inline]] inline static u64 Syscall(int n, auto p1, auto p2, auto p3, auto p4, auto p5) noexcept
{
    u64 result;

    auto a1 = *reinterpret_cast<u64 const*>(&p1);
    auto a2 = *reinterpret_cast<u64 const*>(&p2);
    auto a3 = *reinterpret_cast<u64 const*>(&p3);
    auto a4 = *reinterpret_cast<u64 const*>(&p4);
    auto a5 = *reinterpret_cast<u64 const*>(&p5);

    register u64 r10 asm("r10") = a4;
    register u64 r8 asm("r8") = a5;

    asm volatile("syscall"
                 : "=a"(result)
                 : "a"(n), "D"(a1), "S"(a2), "d"(a3), "r"(r10), "r"(r8)
                 : "rcx", "r11", "memory");

    return result;
}




[[maybe_unused]] [[gnu::always_inline]] inline static u64 Syscall(int n, auto p1, auto p2, auto p3, auto p4, auto p5, auto p6) noexcept
{
    u64 result;

    auto a1 = *reinterpret_cast<u64 const*>(&p1);
    auto a2 = *reinterpret_cast<u64 const*>(&p2);
    auto a3 = *reinterpret_cast<u64 const*>(&p3);
    auto a4 = *reinterpret_cast<u64 const*>(&p4);
    auto a5 = *reinterpret_cast<u64 const*>(&p5);
    auto a6 = *reinterpret_cast<u64 const*>(&p6);

    register u64 r10 asm("r10") = a4;
    register u64 r8 asm("r8") = a5;
    register u64 r9 asm("r9") = a6;

    asm volatile("syscall"
                 : "=a"(result)
                 : "a"(n), "D"(a1), "S"(a2), "d"(a3), "r"(r10), "r"(r8), "r"(r9)
                 : "rcx", "r11", "memory");

    return result;
}

#pragma GCC diagnostic pop
# 143 "./include/commons/system/linux/syscall_linux_x64.inl"
          ;




enum {
    waitpid = -1,
    vm86old = -1,
    vm86 = -1,
    utrap_install = -1,
    umount = -1,
    timerfd = -1,
    sysmips = -1,
    syscall = -1,
    sys_debug_setcontext = -1,
    switch_endian = -1,
    swapcontext = -1,
    subpage_prot = -1,
    stime = -1,
    ssetmask = -1,
    spu_run = -1,
    spu_create = -1,
    socketcall = -1,
    signal = -1,
    sgetmask = -1,
    setpgrp = -1,
    sethae = -1,
    sched_set_affinity = -1,
    sched_get_affinity = -1,
    s390_sthyi = -1,
    s390_runtime_instr = -1,
    s390_pci_mmio_write = -1,
    s390_pci_mmio_read = -1,
    s390_guarded_storage = -1,
    rtas = -1,
    riscv_hwprobe = -1,
    riscv_flush_icache = -1,
    readdir = -1,
    perfctr = -1,
    osf_wait4 = -1,
    osf_utsname = -1,
    osf_utimes = -1,
    osf_usleep_thread = -1,
    osf_sysinfo = -1,
    osf_syscall = -1,
    osf_swapon = -1,
    osf_statfs64 = -1,
    osf_statfs = -1,
    osf_stat = -1,
    osf_sigstack = -1,
    osf_sigprocmask = -1,
    osf_shmat = -1,
    osf_settimeofday = -1,
    osf_setsysinfo = -1,
    osf_setitimer = -1,
    osf_set_program_attributes = -1,
    osf_select = -1,
    osf_proplist_syscall = -1,
    osf_mount = -1,
    osf_lstat = -1,
    osf_gettimeofday = -1,
    osf_getsysinfo = -1,
    osf_getrusage = -1,
    osf_getitimer = -1,
    osf_getdomainname = -1,
    osf_getdirentries = -1,
    osf_fstatfs64 = -1,
    osf_fstatfs = -1,
    osf_fstat = -1,
    or1k_atomic = -1,
    olduname = -1,
    oldumount = -1,
    oldstat = -1,
    oldolduname = -1,
    oldlstat = -1,
    oldfstat = -1,
    old_adjtimex = -1,
    multiplexer = -1,
    memory_ordering = -1,
    kern_features = -1,
    ipc = -1,
    getxuid = -1,
    getxpid = -1,
    getxgid = -1,
    getpagesize = -1,
    gethostname = -1,
    getdtablesize = -1,
    getdomainname = -1,
    fadvise64_64 = -1,
    execv = -1,
    exec_with_loader = -1,
    dipc = -1,
    cacheflush = -1,
    cachectl = -1,
    atomic_cmpxchg_32 = -1,
    atomic_barrier = -1,
    arc_usr_cmpxchg = -1,
    arc_settls = -1,
    arc_gettls = -1,
    nice = -1,
    sigaction = -1,
    sigsuspend = -1,
    sigpending = -1,
    sigreturn = -1,
    sigprocmask = -1,
    _llseek = -1,
    _newselect = -1,
    ugetrlimit = -1,
    mmap2 = -1,
    truncate64 = -1,
    ftruncate64 = -1,
    stat64 = -1,
    lstat64 = -1,
    fstat64 = -1,
    lchown32 = -1,
    getuid32 = -1,
    getgid32 = -1,
    geteuid32 = -1,
    getegid32 = -1,
    setreuid32 = -1,
    setregid32 = -1,
    getgroups32 = -1,
    setgroups32 = -1,
    fchown32 = -1,
    setresuid32 = -1,
    getresuid32 = -1,
    setresgid32 = -1,
    getresgid32 = -1,
    chown32 = -1,
    setuid32 = -1,
    setgid32 = -1,
    setfsuid32 = -1,
    setfsgid32 = -1,
    fcntl64 = -1,
    sendfile64 = -1,
    statfs64 = -1,
    fstatfs64 = -1,
    arm_fadvise64_64 = -1,
    pciconfig_iobase = -1,
    pciconfig_read = -1,
    pciconfig_write = -1,
    send = -1,
    recv = -1,
    fstatat64 = -1,
    sync_file_range2 = -1,
    clock_gettime64 = -1,
    clock_settime64 = -1,
    clock_adjtime64 = -1,
    clock_getres_time64 = -1,
    clock_nanosleep_time64 = -1,
    timer_gettime64 = -1,
    timer_settime64 = -1,
    timerfd_gettime64 = -1,
    timerfd_settime64 = -1,
    utimensat_time64 = -1,
    pselect6_time64 = -1,
    ppoll_time64 = -1,
    io_pgetevents_time64 = -1,
    recvmmsg_time64 = -1,
    mq_timedsend_time64 = -1,
    mq_timedreceive_time64 = -1,
    semtimedop_time64 = -1,
    rt_sigtimedwait_time64 = -1,
    futex_time64 = -1,
    sched_rr_get_interval_time64 = -1,
    read = 0,
    write = 1,
    open = 2,
    close = 3,
    stat = 4,
    fstat = 5,
    lstat = 6,
    poll = 7,
    lseek = 8,
    mmap = 9,
    mprotect = 10,
    munmap = 11,
    brk = 12,
    rt_sigaction = 13,
    rt_sigprocmask = 14,
    rt_sigreturn = 15,
    ioctl = 16,
    pread64 = 17,
    pwrite64 = 18,
    readv = 19,
    writev = 20,
    access = 21,
    pipe = 22,
    select = 23,
    sched_yield = 24,
    mremap = 25,
    msync = 26,
    mincore = 27,
    madvise = 28,
    shmget = 29,
    shmat = 30,
    shmctl = 31,
    dup = 32,
    dup2 = 33,
    pause = 34,
    nanosleep = 35,
    getitimer = 36,
    alarm = 37,
    setitimer = 38,
    getpid = 39,
    sendfile = 40,
    socket = 41,
    connect = 42,
    accept = 43,
    sendto = 44,
    recvfrom = 45,
    sendmsg = 46,
    recvmsg = 47,
    shutdown = 48,
    bind = 49,
    listen = 50,
    getsockname = 51,
    getpeername = 52,
    socketpair = 53,
    setsockopt = 54,
    getsockopt = 55,
    clone = 56,
    fork = 57,
    vfork = 58,
    execve = 59,
    exit = 60,
    wait4 = 61,
    kill = 62,
    uname = 63,
    semget = 64,
    semop = 65,
    semctl = 66,
    shmdt = 67,
    msgget = 68,
    msgsnd = 69,
    msgrcv = 70,
    msgctl = 71,
    fcntl = 72,
    flock = 73,
    fsync = 74,
    fdatasync = 75,
    truncate = 76,
    ftruncate = 77,
    getdents = 78,
    getcwd = 79,
    chdir = 80,
    fchdir = 81,
    rename = 82,
    mkdir = 83,
    rmdir = 84,
    creat = 85,
    link = 86,
    unlink = 87,
    symlink = 88,
    readlink = 89,
    chmod = 90,
    fchmod = 91,
    chown = 92,
    fchown = 93,
    lchown = 94,
    umask = 95,
    gettimeofday = 96,
    getrlimit = 97,
    getrusage = 98,
    sysinfo = 99,
    times = 100,
    ptrace = 101,
    getuid = 102,
    syslog = 103,
    getgid = 104,
    setuid = 105,
    setgid = 106,
    geteuid = 107,
    getegid = 108,
    setpgid = 109,
    getppid = 110,
    getpgrp = 111,
    setsid = 112,
    setreuid = 113,
    setregid = 114,
    getgroups = 115,
    setgroups = 116,
    setresuid = 117,
    getresuid = 118,
    setresgid = 119,
    getresgid = 120,
    getpgid = 121,
    setfsuid = 122,
    setfsgid = 123,
    getsid = 124,
    capget = 125,
    capset = 126,
    rt_sigpending = 127,
    rt_sigtimedwait = 128,
    rt_sigqueueinfo = 129,
    rt_sigsuspend = 130,
    sigaltstack = 131,
    utime = 132,
    mknod = 133,
    personality = 135,
    ustat = 136,
    statfs = 137,
    fstatfs = 138,
    sysfs = 139,
    getpriority = 140,
    setpriority = 141,
    sched_setparam = 142,
    sched_getparam = 143,
    sched_setscheduler = 144,
    sched_getscheduler = 145,
    sched_get_priority_max = 146,
    sched_get_priority_min = 147,
    sched_rr_get_interval = 148,
    mlock = 149,
    munlock = 150,
    mlockall = 151,
    munlockall = 152,
    vhangup = 153,
    modify_ldt = 154,
    pivot_root = 155,
    prctl = 157,
    arch_prctl = 158,
    adjtimex = 159,
    setrlimit = 160,
    chroot = 161,
    sync = 162,
    acct = 163,
    settimeofday = 164,
    mount = 165,
    umount2 = 166,
    swapon = 167,
    swapoff = 168,
    reboot = 169,
    sethostname = 170,
    setdomainname = 171,
    iopl = 172,
    ioperm = 173,
    init_module = 175,
    delete_module = 176,
    quotactl = 179,
    gettid = 186,
    readahead = 187,
    setxattr = 188,
    lsetxattr = 189,
    fsetxattr = 190,
    getxattr = 191,
    lgetxattr = 192,
    fgetxattr = 193,
    listxattr = 194,
    llistxattr = 195,
    flistxattr = 196,
    removexattr = 197,
    lremovexattr = 198,
    fremovexattr = 199,
    tkill = 200,
    time = 201,
    futex = 202,
    sched_setaffinity = 203,
    sched_getaffinity = 204,
    set_thread_area = 205,
    io_setup = 206,
    io_destroy = 207,
    io_getevents = 208,
    io_submit = 209,
    io_cancel = 210,
    get_thread_area = 211,
    lookup_dcookie = 212,
    epoll_create = 213,
    epoll_ctl_old = 214,
    epoll_wait_old = 215,
    remap_file_pages = 216,
    getdents64 = 217,
    set_tid_address = 218,
    restart_syscall = 219,
    semtimedop = 220,
    fadvise64 = 221,
    timer_create = 222,
    timer_settime = 223,
    timer_gettime = 224,
    timer_getoverrun = 225,
    timer_delete = 226,
    clock_settime = 227,
    clock_gettime = 228,
    clock_getres = 229,
    clock_nanosleep = 230,
    exit_group = 231,
    epoll_wait = 232,
    epoll_ctl = 233,
    tgkill = 234,
    utimes = 235,
    mbind = 237,
    set_mempolicy = 238,
    get_mempolicy = 239,
    mq_open = 240,
    mq_unlink = 241,
    mq_timedsend = 242,
    mq_timedreceive = 243,
    mq_notify = 244,
    mq_getsetattr = 245,
    kexec_load = 246,
    waitid = 247,
    add_key = 248,
    request_key = 249,
    keyctl = 250,
    ioprio_set = 251,
    ioprio_get = 252,
    inotify_init = 253,
    inotify_add_watch = 254,
    inotify_rm_watch = 255,
    migrate_pages = 256,
    openat = 257,
    mkdirat = 258,
    mknodat = 259,
    fchownat = 260,
    futimesat = 261,
    newfstatat = 262,
    unlinkat = 263,
    renameat = 264,
    linkat = 265,
    symlinkat = 266,
    readlinkat = 267,
    fchmodat = 268,
    faccessat = 269,
    pselect6 = 270,
    ppoll = 271,
    unshare = 272,
    set_robust_list = 273,
    get_robust_list = 274,
    splice = 275,
    tee = 276,
    sync_file_range = 277,
    vmsplice = 278,
    move_pages = 279,
    utimensat = 280,
    epoll_pwait = 281,
    signalfd = 282,
    timerfd_create = 283,
    eventfd = 284,
    fallocate = 285,
    timerfd_settime = 286,
    timerfd_gettime = 287,
    accept4 = 288,
    signalfd4 = 289,
    eventfd2 = 290,
    epoll_create1 = 291,
    dup3 = 292,
    pipe2 = 293,
    inotify_init1 = 294,
    preadv = 295,
    pwritev = 296,
    rt_tgsigqueueinfo = 297,
    perf_event_open = 298,
    recvmmsg = 299,
    fanotify_init = 300,
    fanotify_mark = 301,
    prlimit64 = 302,
    name_to_handle_at = 303,
    open_by_handle_at = 304,
    clock_adjtime = 305,
    syncfs = 306,
    sendmmsg = 307,
    setns = 308,
    getcpu = 309,
    process_vm_readv = 310,
    process_vm_writev = 311,
    kcmp = 312,
    finit_module = 313,
    sched_setattr = 314,
    sched_getattr = 315,
    renameat2 = 316,
    seccomp = 317,
    getrandom = 318,
    memfd_create = 319,
    kexec_file_load = 320,
    bpf = 321,
    execveat = 322,
    userfaultfd = 323,
    membarrier = 324,
    mlock2 = 325,
    copy_file_range = 326,
    preadv2 = 327,
    pwritev2 = 328,
    pkey_mprotect = 329,
    pkey_alloc = 330,
    pkey_free = 331,
    statx = 332,
    io_pgetevents = 333,
    rseq = 334,
    pidfd_send_signal = 424,
    io_uring_setup = 425,
    io_uring_enter = 426,
    io_uring_register = 427,
    open_tree = 428,
    move_mount = 429,
    fsopen = 430,
    fsconfig = 431,
    fsmount = 432,
    fspick = 433,
    pidfd_open = 434,
    clone3 = 435,
    close_range = 436,
    openat2 = 437,
    pidfd_getfd = 438,
    faccessat2 = 439,
    process_madvise = 440,
    epoll_pwait2 = 441,
    mount_setattr = 442,
    quotactl_fd = 443,
    landlock_create_ruleset = 444,
    landlock_add_rule = 445,
    landlock_restrict_self = 446,
    memfd_secret = 447,
    process_mrelease = 448,
    futex_waitv = 449,
    set_mempolicy_home_node = 450,
    cachestat = 451,
    fchmodat2 = 452,
    map_shadow_stack = 453,
    futex_wake = 454,
    futex_wait = 455,
    futex_requeue = 456,
    statmount = 457,
    listmount = 458,
    lsm_get_self_attr = 459,
    lsm_set_self_attr = 460,
    lsm_list_modules = 461
};
# 27 "./include/commons/system/linux/linuxsyscall.inl" 2





    [[maybe_unused]] [[gnu::always_inline]] inline static u64 operator()(int num)
    {
        return Syscall(num);
    }
    [[maybe_unused]] [[gnu::always_inline]] inline static u64 operator()(int num, u64 p1)
    {
        return Syscall(num, p1);
    }
    [[maybe_unused]] [[gnu::always_inline]] inline static u64 operator()(int num, u64 p1, u64 p2)
    {
        return Syscall(num, p1, p2);
    }
    [[maybe_unused]] [[gnu::always_inline]] inline static u64 operator()(int num, u64 p1, u64 p2, u64 p3)
    {
        return Syscall(num, p1, p2, p3);
    }
    [[maybe_unused]] [[gnu::always_inline]] inline static u64 operator()(int num, u64 p1, u64 p2, u64 p3, u64 p4)
    {
        return Syscall(num, p1, p2, p3, p4);
    }
    [[maybe_unused]] [[gnu::always_inline]] inline static u64 operator()(int num, u64 p1, u64 p2, u64 p3, u64 p4, u64 p5)
    {
        return Syscall(num, p1, p2, p3, p4, p5);
    }
    [[maybe_unused]] [[gnu::always_inline]] inline static u64 operator()(int num, u64 p1, u64 p2, u64 p3, u64 p4, u64 p5, u64 p6)
    {
        return Syscall(num, p1, p2, p3, p4, p5, p6);
    }
} inline constexpr LinuxSyscall;


namespace impl {


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wregister"
# 236 "./include/commons/system/linux/linuxsyscall.inl"
static inline int linux_is_error_result(u32 result)
{




    return result > u32(-0x1000);
}


}
# 47 "./include/commons/system.hh" 2
# 1 "./include/commons/system/linux/linuxstdout.inl" 1
# 21 "./include/commons/system/linux/linuxstdout.inl"
class LinuxStandardOutStream : public IOutStream<LinuxStandardOutStream> {
protected:
    bool _state;
    int _fd = 1;

public:
    constexpr inline LinuxStandardOutStream() = default;
    constexpr inline ~LinuxStandardOutStream() = default;
    constexpr inline LinuxStandardOutStream(LinuxStandardOutStream const&) = default;
    constexpr inline LinuxStandardOutStream& operator=(LinuxStandardOutStream const&) = default;


    LinuxStandardOutStream& writeBytes(void const* data, size_t sizeBytes)
    {
        if (_fd != 1 && _fd != 2) {
            _fd = 2;
        }

        auto r = ssize_t(LinuxSyscall(LinuxSyscall.write, usize(_fd), usize(data), usize(sizeBytes)));

        if (r < 0) {
            _state = STATUS_FAILED_FLUSH;
            return *this;
        }
        [[assume(r >= 0)]];
        if (sizeBytes != size_t(size_t(size_t(size_t(r)))) ) {
            _state = STATUS_NOT_ALL_BYTES_FLUSHED;
        }
        return *this;
    }

    constexpr inline LinuxStandardOutStream& flush() { return *this; }
};




class LinuxStandardErrOutStream final : public LinuxStandardOutStream {
public:
    constexpr inline LinuxStandardErrOutStream() { this->_fd = 2; }
};





inline Optional<LinuxStandardOutStream> const stdout = LinuxStandardOutStream();





inline Optional<LinuxStandardErrOutStream> const stderr = LinuxStandardErrOutStream();
# 48 "./include/commons/system.hh" 2
# 1 "./include/commons/system/linux/linuxfileout.inl" 1
# 21 "./include/commons/system/linux/linuxfileout.inl"
struct LinuxFileOutStream final : public IOutStream<LinuxFileOutStream>, public NonCopyable
{
    constexpr LinuxFileOutStream(LinuxFileOutStream&& other) = default;
    constexpr LinuxFileOutStream& operator=(LinuxFileOutStream&& other) = default;

    int _fd = 0;
    Status _status = STATUS_OK;
    Array<u8> _buffer;
    usize _bufferUsed = 0;
# 38 "./include/commons/system/linux/linuxfileout.inl"
    StreamStatus setStatusFromErrno(StreamStatus& status, int const& err = errno)
    {
        switch (err) {

        case 1:
        case 13: return status = STATUS_ERR_ACCESS;

        case 9: return status = STATUS_ERR_INVALID;

        case 26:
        case 16: return status = STATUS_ERR_BUSY;

        case 122: return status = STATUS_ERR_QUOTA;
        case 17: return status = STATUS_ERR_ALREADY_EXISTS;


        case 25:
        case 21:
            return status = STATUS_ERR_UNSUPPORTED;

        case 4: return status = STATUS_ERR_INTERRUPT;
        case 5: return status = STATUS_ERR_IO;
        case 40: return status = STATUS_ERR_LOOP;

        case 31:
        case 24: return status = STATUS_ERR_LIMIT;


        case 12:
        case 27:
        case 28:
            return status = STATUS_ERR_MEMORY;

        case 19:
        case 3:
            return status = STATUS_ERR_NOT_FOUND;

        default: return status = STATUS_ERR_UNKNOWN;
        }
    }






    inline LinuxFileOutStream(String const& path, Optional<usize> const& bufferCapacity = None)
        : _buffer(bufferCapacity.valueOr<usize>(4_KB))
    {

        auto result = i64(LinuxSyscall(LinuxSyscall.open, u64(path.cstr()), u64(01)));

        if (result < 0 && result > -0x1000) {
            auto err = int(-result);
            setStatusFromErrno(_status, err);
            _fd = -1;
        } else {
            _fd = int(unsigned(result));
        }
    }





    [[clang::noinline]]
    inline ~LinuxFileOutStream()
    {
        if (_fd < 3) {
            return;
        }


        this->flush();
        (void)this->close();
    }



    [[clang::noinline]]
    inline LinuxFileOutStream& writeBytes(void const* data, size_t sizeBytes)
    {

        if (_bufferUsed + sizeBytes >= _buffer.length()) {


            if (!doWrite(_buffer.data(), _bufferUsed)) {
                return *this;
            }

            _bufferUsed = 0;

            if (sizeBytes > _buffer.length()) {
                if (!doWrite(data, sizeBytes)) {
                    return *this;
                }
            }
        }

#pragma GCC diagnostic push
# 137 "./include/commons/system/linux/linuxfileout.inl"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
# 137 "./include/commons/system/linux/linuxfileout.inl"
        __builtin_memcpy(_buffer.data() + _bufferUsed, data, sizeBytes)
# 137 "./include/commons/system/linux/linuxfileout.inl"
#pragma GCC diagnostic pop
# 137 "./include/commons/system/linux/linuxfileout.inl"
                                                                               ;
        _bufferUsed += sizeBytes;
        return *this;
    }


    bool doWrite(void const* buffer, size_t size)
    {
        if (size == 0) {
            return true;
        }
        auto r = ssize_t(LinuxSyscall(LinuxSyscall.write, usize(_fd), usize(buffer), size));
        if (r < 0) {
            int err = int(-r);
            _status = setStatusFromErrno(_status, err);
            return false;
        }
        [[assume(r >= 0)]];
        if (size != usize(r)) {
            _status = STATUS_NOT_ALL_BYTES_FLUSHED;
            return false;
        }
        return true;
    };


    LinuxFileOutStream& flush()
    {
        (void)doWrite(_buffer.data(), _bufferUsed);
        return *this;
    }




    inline Result<Status, Status> close()
    {
        auto result = isize(LinuxSyscall(LinuxSyscall.close, usize(_fd)));
        if (result < 0) {
            int err = int(-result);
            return Err(setStatusFromErrno(_status, err));
        } else {
            return Ok(_status = STATUS_OK);
        }
    }





    Status status() const { return _status; }
};




struct FileOutStream : public Optional<LinuxFileOutStream>
{
    using Optional<LinuxFileOutStream>::Optional;






    FileOutStream(String const& path, Optional<usize> const& bufferCapacity = 4_KB)
        : Optional<LinuxFileOutStream>(LinuxFileOutStream(path, bufferCapacity))
    {}

    ~FileOutStream() = default;
};
# 49 "./include/commons/system.hh" 2
# 1 "./include/commons/system/linux/linuxshell.inl" 1
# 21 "./include/commons/system/linux/linuxshell.inl"
struct LinuxShell : NonCopyable
{
    constexpr LinuxShell() noexcept = default;
    constexpr LinuxShell(LinuxShell&&) noexcept = default;
    constexpr LinuxShell& operator=(LinuxShell&&) noexcept = default;
    constexpr ~LinuxShell() noexcept = default;


    inline int execute(String const& command, Optional<Function<void(void const*, usize)>> output = None)
    {

        auto s = _escapeCommand(command);

        FILE* fp = popen(s.cstr(), "r");
        if (fp == nullptr) {
            return -1;
        }
        if (output.hasValue()) {
            for (int value = fgetc(fp); value != (-1); value = fgetc(fp)) {
                auto byte = char(value);
                output.value()(&byte, 1);
            }
        }
        return pclose(fp);
    }

    inline String _escapeCommand(String const& command) { return command.replace("\"", "\\\""); }
};

inline Optional<LinuxShell> const shell = LinuxShell();
# 50 "./include/commons/system.hh" 2
# 1 "./include/commons/system/linux/linuxruntime.inl" 1
# 51 "./include/commons/system.hh" 2
# 79 "./include/commons/system.hh"
}
# 30 "./include/commons/startup.hh" 2
# 40 "./include/commons/startup.hh"
# 1 "/usr/include/signal.h" 1 3 4
# 25 "/usr/include/signal.h" 3 4
# 1 "/usr/include/features.h" 1 3 4
# 415 "/usr/include/features.h" 3 4
# 1 "/usr/include/features-time64.h" 1 3 4
# 20 "/usr/include/features-time64.h" 3 4
# 1 "/usr/include/bits/wordsize.h" 1 3 4
# 21 "/usr/include/features-time64.h" 2 3 4
# 1 "/usr/include/bits/timesize.h" 1 3 4
# 19 "/usr/include/bits/timesize.h" 3 4
# 1 "/usr/include/bits/wordsize.h" 1 3 4
# 20 "/usr/include/bits/timesize.h" 2 3 4
# 22 "/usr/include/features-time64.h" 2 3 4
# 416 "/usr/include/features.h" 2 3 4
# 502 "/usr/include/features.h" 3 4
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 503 "/usr/include/features.h" 2 3 4
# 524 "/usr/include/features.h" 3 4
# 1 "/usr/include/sys/cdefs.h" 1 3 4
# 730 "/usr/include/sys/cdefs.h" 3 4
# 1 "/usr/include/bits/wordsize.h" 1 3 4
# 731 "/usr/include/sys/cdefs.h" 2 3 4
# 1 "/usr/include/bits/long-double.h" 1 3 4
# 732 "/usr/include/sys/cdefs.h" 2 3 4
# 525 "/usr/include/features.h" 2 3 4
# 548 "/usr/include/features.h" 3 4
# 1 "/usr/include/gnu/stubs.h" 1 3 4
# 10 "/usr/include/gnu/stubs.h" 3 4
# 1 "/usr/include/gnu/stubs-64.h" 1 3 4
# 11 "/usr/include/gnu/stubs.h" 2 3 4
# 549 "/usr/include/features.h" 2 3 4
# 26 "/usr/include/signal.h" 2 3 4

extern "C" {

# 1 "/usr/include/bits/types.h" 1 3 4
# 27 "/usr/include/bits/types.h" 3 4
# 1 "/usr/include/bits/wordsize.h" 1 3 4
# 28 "/usr/include/bits/types.h" 2 3 4
# 1 "/usr/include/bits/timesize.h" 1 3 4
# 19 "/usr/include/bits/timesize.h" 3 4
# 1 "/usr/include/bits/wordsize.h" 1 3 4
# 20 "/usr/include/bits/timesize.h" 2 3 4
# 29 "/usr/include/bits/types.h" 2 3 4


typedef unsigned char __u_char;
typedef unsigned short int __u_short;
typedef unsigned int __u_int;
typedef unsigned long int __u_long;


typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef signed short int __int16_t;
typedef unsigned short int __uint16_t;
typedef signed int __int32_t;
typedef unsigned int __uint32_t;

typedef signed long int __int64_t;
typedef unsigned long int __uint64_t;






typedef __int8_t __int_least8_t;
typedef __uint8_t __uint_least8_t;
typedef __int16_t __int_least16_t;
typedef __uint16_t __uint_least16_t;
typedef __int32_t __int_least32_t;
typedef __uint32_t __uint_least32_t;
typedef __int64_t __int_least64_t;
typedef __uint64_t __uint_least64_t;



typedef long int __quad_t;
typedef unsigned long int __u_quad_t;







typedef long int __intmax_t;
typedef unsigned long int __uintmax_t;
# 141 "/usr/include/bits/types.h" 3 4
# 1 "/usr/include/bits/typesizes.h" 1 3 4
# 142 "/usr/include/bits/types.h" 2 3 4
# 1 "/usr/include/bits/time64.h" 1 3 4
# 143 "/usr/include/bits/types.h" 2 3 4


typedef unsigned long int __dev_t;
typedef unsigned int __uid_t;
typedef unsigned int __gid_t;
typedef unsigned long int __ino_t;
typedef unsigned long int __ino64_t;
typedef unsigned int __mode_t;
typedef unsigned long int __nlink_t;
typedef long int __off_t;
typedef long int __off64_t;
typedef int __pid_t;
typedef struct { int __val[2]; } __fsid_t;
typedef long int __clock_t;
typedef unsigned long int __rlim_t;
typedef unsigned long int __rlim64_t;
typedef unsigned int __id_t;
typedef long int __time_t;
typedef unsigned int __useconds_t;
typedef long int __suseconds_t;
typedef long int __suseconds64_t;

typedef int __daddr_t;
typedef int __key_t;


typedef int __clockid_t;


typedef void * __timer_t;


typedef long int __blksize_t;




typedef long int __blkcnt_t;
typedef long int __blkcnt64_t;


typedef unsigned long int __fsblkcnt_t;
typedef unsigned long int __fsblkcnt64_t;


typedef unsigned long int __fsfilcnt_t;
typedef unsigned long int __fsfilcnt64_t;


typedef long int __fsword_t;

typedef long int __ssize_t;


typedef long int __syscall_slong_t;

typedef unsigned long int __syscall_ulong_t;



typedef __off64_t __loff_t;
typedef char *__caddr_t;


typedef long int __intptr_t;


typedef unsigned int __socklen_t;




typedef int __sig_atomic_t;
# 30 "/usr/include/signal.h" 2 3 4
# 1 "/usr/include/bits/signum-generic.h" 1 3 4
# 76 "/usr/include/bits/signum-generic.h" 3 4
# 1 "/usr/include/bits/signum-arch.h" 1 3 4
# 77 "/usr/include/bits/signum-generic.h" 2 3 4
# 31 "/usr/include/signal.h" 2 3 4

# 1 "/usr/include/bits/types/sig_atomic_t.h" 1 3 4







typedef __sig_atomic_t sig_atomic_t;
# 33 "/usr/include/signal.h" 2 3 4


# 1 "/usr/include/bits/types/sigset_t.h" 1 3 4



# 1 "/usr/include/bits/types/__sigset_t.h" 1 3 4




typedef struct
{
  unsigned long int __val[(1024 / (8 * sizeof (unsigned long int)))];
} __sigset_t;
# 5 "/usr/include/bits/types/sigset_t.h" 2 3 4


typedef __sigset_t sigset_t;
# 36 "/usr/include/signal.h" 2 3 4




typedef __pid_t pid_t;





typedef __uid_t uid_t;






# 1 "/usr/include/bits/types/struct_timespec.h" 1 3 4





# 1 "/usr/include/bits/endian.h" 1 3 4
# 35 "/usr/include/bits/endian.h" 3 4
# 1 "/usr/include/bits/endianness.h" 1 3 4
# 36 "/usr/include/bits/endian.h" 2 3 4
# 7 "/usr/include/bits/types/struct_timespec.h" 2 3 4
# 1 "/usr/include/bits/types/time_t.h" 1 3 4
# 10 "/usr/include/bits/types/time_t.h" 3 4
typedef __time_t time_t;
# 8 "/usr/include/bits/types/struct_timespec.h" 2 3 4



struct timespec
{



  __time_t tv_sec;




  __syscall_slong_t tv_nsec;
# 31 "/usr/include/bits/types/struct_timespec.h" 3 4
};
# 54 "/usr/include/signal.h" 2 3 4



# 1 "/usr/include/bits/types/siginfo_t.h" 1 3 4



# 1 "/usr/include/bits/wordsize.h" 1 3 4
# 5 "/usr/include/bits/types/siginfo_t.h" 2 3 4

# 1 "/usr/include/bits/types/__sigval_t.h" 1 3 4
# 24 "/usr/include/bits/types/__sigval_t.h" 3 4
union sigval
{
  int sival_int;
  void *sival_ptr;
};

typedef union sigval __sigval_t;
# 7 "/usr/include/bits/types/siginfo_t.h" 2 3 4
# 16 "/usr/include/bits/types/siginfo_t.h" 3 4
# 1 "/usr/include/bits/siginfo-arch.h" 1 3 4
# 17 "/usr/include/bits/types/siginfo_t.h" 2 3 4
# 36 "/usr/include/bits/types/siginfo_t.h" 3 4
typedef struct
  {
    int si_signo;

    int si_errno;

    int si_code;





    int __pad0;


    union
      {
 int _pad[((128 / sizeof (int)) - 4)];


 struct
   {
     __pid_t si_pid;
     __uid_t si_uid;
   } _kill;


 struct
   {
     int si_tid;
     int si_overrun;
     __sigval_t si_sigval;
   } _timer;


 struct
   {
     __pid_t si_pid;
     __uid_t si_uid;
     __sigval_t si_sigval;
   } _rt;


 struct
   {
     __pid_t si_pid;
     __uid_t si_uid;
     int si_status;
     __clock_t si_utime;
     __clock_t si_stime;
   } _sigchld;


 struct
   {
     void *si_addr;

     short int si_addr_lsb;
     union
       {

  struct
    {
      void *_lower;
      void *_upper;
    } _addr_bnd;

  __uint32_t _pkey;
       } _bounds;
   } _sigfault;


 struct
   {
     long int si_band;
     int si_fd;
   } _sigpoll;



 struct
   {
     void *_call_addr;
     int _syscall;
     unsigned int _arch;
   } _sigsys;

      } _sifields;
  } siginfo_t ;
# 58 "/usr/include/signal.h" 2 3 4
# 1 "/usr/include/bits/siginfo-consts.h" 1 3 4
# 35 "/usr/include/bits/siginfo-consts.h" 3 4
enum
{
  SI_ASYNCNL = -60,
  SI_DETHREAD = -7,

  SI_TKILL,
  SI_SIGIO,

  SI_ASYNCIO,
  SI_MESGQ,
  SI_TIMER,





  SI_QUEUE,
  SI_USER,
  SI_KERNEL = 0x80
# 66 "/usr/include/bits/siginfo-consts.h" 3 4
};




enum
{
  ILL_ILLOPC = 1,

  ILL_ILLOPN,

  ILL_ILLADR,

  ILL_ILLTRP,

  ILL_PRVOPC,

  ILL_PRVREG,

  ILL_COPROC,

  ILL_BADSTK,

  ILL_BADIADDR

};


enum
{
  FPE_INTDIV = 1,

  FPE_INTOVF,

  FPE_FLTDIV,

  FPE_FLTOVF,

  FPE_FLTUND,

  FPE_FLTRES,

  FPE_FLTINV,

  FPE_FLTSUB,

  FPE_FLTUNK = 14,

  FPE_CONDTRAP

};


enum
{
  SEGV_MAPERR = 1,

  SEGV_ACCERR,

  SEGV_BNDERR,

  SEGV_PKUERR,

  SEGV_ACCADI,

  SEGV_ADIDERR,

  SEGV_ADIPERR,

  SEGV_MTEAERR,

  SEGV_MTESERR,

  SEGV_CPERR

};


enum
{
  BUS_ADRALN = 1,

  BUS_ADRERR,

  BUS_OBJERR,

  BUS_MCEERR_AR,

  BUS_MCEERR_AO

};




enum
{
  TRAP_BRKPT = 1,

  TRAP_TRACE,

  TRAP_BRANCH,

  TRAP_HWBKPT,

  TRAP_UNK

};




enum
{
  CLD_EXITED = 1,

  CLD_KILLED,

  CLD_DUMPED,

  CLD_TRAPPED,

  CLD_STOPPED,

  CLD_CONTINUED

};


enum
{
  POLL_IN = 1,

  POLL_OUT,

  POLL_MSG,

  POLL_ERR,

  POLL_PRI,

  POLL_HUP

};





# 1 "/usr/include/bits/siginfo-consts-arch.h" 1 3 4
# 216 "/usr/include/bits/siginfo-consts.h" 2 3 4
# 59 "/usr/include/signal.h" 2 3 4



# 1 "/usr/include/bits/types/sigval_t.h" 1 3 4
# 16 "/usr/include/bits/types/sigval_t.h" 3 4
typedef __sigval_t sigval_t;
# 63 "/usr/include/signal.h" 2 3 4



# 1 "/usr/include/bits/types/sigevent_t.h" 1 3 4



# 1 "/usr/include/bits/wordsize.h" 1 3 4
# 5 "/usr/include/bits/types/sigevent_t.h" 2 3 4
# 17 "/usr/include/bits/types/sigevent_t.h" 3 4
typedef union pthread_attr_t pthread_attr_t;




typedef struct sigevent
  {
    __sigval_t sigev_value;
    int sigev_signo;
    int sigev_notify;

    union
      {
 int _pad[((64 / sizeof (int)) - 4)];



 __pid_t _tid;

 struct
   {
     void (*_function) (__sigval_t);
     pthread_attr_t *_attribute;
   } _sigev_thread;
      } _sigev_un;
  } sigevent_t;
# 67 "/usr/include/signal.h" 2 3 4
# 1 "/usr/include/bits/sigevent-consts.h" 1 3 4
# 27 "/usr/include/bits/sigevent-consts.h" 3 4
enum
{
  SIGEV_SIGNAL = 0,

  SIGEV_NONE,

  SIGEV_THREAD,


  SIGEV_THREAD_ID = 4


};
# 68 "/usr/include/signal.h" 2 3 4




typedef void (*__sighandler_t) (int);




extern __sighandler_t __sysv_signal (int __sig, __sighandler_t __handler)
     noexcept (true);

extern __sighandler_t sysv_signal (int __sig, __sighandler_t __handler)
     noexcept (true);






extern __sighandler_t signal (int __sig, __sighandler_t __handler)
     noexcept (true);
# 112 "/usr/include/signal.h" 3 4
extern int kill (__pid_t __pid, int __sig) noexcept (true);






extern int killpg (__pid_t __pgrp, int __sig) noexcept (true);



extern int raise (int __sig) noexcept (true);



extern __sighandler_t ssignal (int __sig, __sighandler_t __handler)
     noexcept (true);
extern int gsignal (int __sig) noexcept (true);




extern void psignal (int __sig, const char *__s);


extern void psiginfo (const siginfo_t *__pinfo, const char *__s);
# 151 "/usr/include/signal.h" 3 4
extern int sigpause (int __sig) __asm__ ("__xpg_sigpause")
  __attribute__ ((__deprecated__ ("Use the sigsuspend function instead")));
# 173 "/usr/include/signal.h" 3 4
extern int sigblock (int __mask) noexcept (true) __attribute__ ((__deprecated__));


extern int sigsetmask (int __mask) noexcept (true) __attribute__ ((__deprecated__));


extern int siggetmask (void) noexcept (true) __attribute__ ((__deprecated__));
# 188 "/usr/include/signal.h" 3 4
typedef __sighandler_t sighandler_t;




typedef __sighandler_t sig_t;





extern int sigemptyset (sigset_t *__set) noexcept (true) __attribute__ ((__nonnull__ (1)));


extern int sigfillset (sigset_t *__set) noexcept (true) __attribute__ ((__nonnull__ (1)));


extern int sigaddset (sigset_t *__set, int __signo) noexcept (true) __attribute__ ((__nonnull__ (1)));


extern int sigdelset (sigset_t *__set, int __signo) noexcept (true) __attribute__ ((__nonnull__ (1)));


extern int sigismember (const sigset_t *__set, int __signo)
     noexcept (true) __attribute__ ((__nonnull__ (1)));



extern int sigisemptyset (const sigset_t *__set) noexcept (true) __attribute__ ((__nonnull__ (1)));


extern int sigandset (sigset_t *__set, const sigset_t *__left,
        const sigset_t *__right) noexcept (true) __attribute__ ((__nonnull__ (1, 2, 3)));


extern int sigorset (sigset_t *__set, const sigset_t *__left,
       const sigset_t *__right) noexcept (true) __attribute__ ((__nonnull__ (1, 2, 3)));




# 1 "/usr/include/bits/sigaction.h" 1 3 4
# 27 "/usr/include/bits/sigaction.h" 3 4
struct sigaction
  {


    union
      {

 __sighandler_t sa_handler;

 void (*sa_sigaction) (int, siginfo_t *, void *);
      }
    __sigaction_handler;







    __sigset_t sa_mask;


    int sa_flags;


    void (*sa_restorer) (void);
  };
# 230 "/usr/include/signal.h" 2 3 4


extern int sigprocmask (int __how, const sigset_t *__restrict __set,
   sigset_t *__restrict __oset) noexcept (true);






extern int sigsuspend (const sigset_t *__set) __attribute__ ((__nonnull__ (1)));


extern int sigaction (int __sig, const struct sigaction *__restrict __act,
        struct sigaction *__restrict __oact) noexcept (true);


extern int sigpending (sigset_t *__set) noexcept (true) __attribute__ ((__nonnull__ (1)));







extern int sigwait (const sigset_t *__restrict __set, int *__restrict __sig)
     __attribute__ ((__nonnull__ (1, 2)));







extern int sigwaitinfo (const sigset_t *__restrict __set,
   siginfo_t *__restrict __info) __attribute__ ((__nonnull__ (1)));







extern int sigtimedwait (const sigset_t *__restrict __set,
    siginfo_t *__restrict __info,
    const struct timespec *__restrict __timeout)
     __attribute__ ((__nonnull__ (1)));
# 292 "/usr/include/signal.h" 3 4
extern int sigqueue (__pid_t __pid, int __sig, const union sigval __val)
     noexcept (true);







# 1 "/usr/include/bits/sigcontext.h" 1 3 4
# 31 "/usr/include/bits/sigcontext.h" 3 4
struct _fpx_sw_bytes
{
  __uint32_t magic1;
  __uint32_t extended_size;
  __uint64_t xstate_bv;
  __uint32_t xstate_size;
  __uint32_t __glibc_reserved1[7];
};

struct _fpreg
{
  unsigned short significand[4];
  unsigned short exponent;
};

struct _fpxreg
{
  unsigned short significand[4];
  unsigned short exponent;
  unsigned short __glibc_reserved1[3];
};

struct _xmmreg
{
  __uint32_t element[4];
};
# 123 "/usr/include/bits/sigcontext.h" 3 4
struct _fpstate
{

  __uint16_t cwd;
  __uint16_t swd;
  __uint16_t ftw;
  __uint16_t fop;
  __uint64_t rip;
  __uint64_t rdp;
  __uint32_t mxcsr;
  __uint32_t mxcr_mask;
  struct _fpxreg _st[8];
  struct _xmmreg _xmm[16];
  __uint32_t __glibc_reserved1[24];
};

struct sigcontext
{
  __uint64_t r8;
  __uint64_t r9;
  __uint64_t r10;
  __uint64_t r11;
  __uint64_t r12;
  __uint64_t r13;
  __uint64_t r14;
  __uint64_t r15;
  __uint64_t rdi;
  __uint64_t rsi;
  __uint64_t rbp;
  __uint64_t rbx;
  __uint64_t rdx;
  __uint64_t rax;
  __uint64_t rcx;
  __uint64_t rsp;
  __uint64_t rip;
  __uint64_t eflags;
  unsigned short cs;
  unsigned short gs;
  unsigned short fs;
  unsigned short __pad0;
  __uint64_t err;
  __uint64_t trapno;
  __uint64_t oldmask;
  __uint64_t cr2;
  __extension__ union
    {
      struct _fpstate * fpstate;
      __uint64_t __fpstate_word;
    };
  __uint64_t __reserved1 [8];
};



struct _xsave_hdr
{
  __uint64_t xstate_bv;
  __uint64_t __glibc_reserved1[2];
  __uint64_t __glibc_reserved2[5];
};

struct _ymmh_state
{
  __uint32_t ymmh_space[64];
};

struct _xstate
{
  struct _fpstate fpstate;
  struct _xsave_hdr xstate_hdr;
  struct _ymmh_state ymmh;
};
# 302 "/usr/include/signal.h" 2 3 4


extern int sigreturn (struct sigcontext *__scp) noexcept (true);






# 1 "/usr/bin/../lib/clang/21/include/stddef.h" 1 3 4
# 93 "/usr/bin/../lib/clang/21/include/stddef.h" 3 4
# 1 "/usr/bin/../lib/clang/21/include/__stddef_size_t.h" 1 3 4
# 18 "/usr/bin/../lib/clang/21/include/__stddef_size_t.h" 3 4
typedef long unsigned int size_t;
# 94 "/usr/bin/../lib/clang/21/include/stddef.h" 2 3 4
# 312 "/usr/include/signal.h" 2 3 4

# 1 "/usr/include/bits/types/stack_t.h" 1 3 4
# 23 "/usr/include/bits/types/stack_t.h" 3 4
# 1 "/usr/bin/../lib/clang/21/include/stddef.h" 1 3 4
# 93 "/usr/bin/../lib/clang/21/include/stddef.h" 3 4
# 1 "/usr/bin/../lib/clang/21/include/__stddef_size_t.h" 1 3 4
# 94 "/usr/bin/../lib/clang/21/include/stddef.h" 2 3 4
# 24 "/usr/include/bits/types/stack_t.h" 2 3 4


typedef struct
  {
    void *ss_sp;
    int ss_flags;
    size_t ss_size;
  } stack_t;
# 314 "/usr/include/signal.h" 2 3 4


# 1 "/usr/include/sys/ucontext.h" 1 3 4
# 37 "/usr/include/sys/ucontext.h" 3 4
__extension__ typedef long long int greg_t;
# 46 "/usr/include/sys/ucontext.h" 3 4
typedef greg_t gregset_t[23];



enum
{
  REG_R8 = 0,

  REG_R9,

  REG_R10,

  REG_R11,

  REG_R12,

  REG_R13,

  REG_R14,

  REG_R15,

  REG_RDI,

  REG_RSI,

  REG_RBP,

  REG_RBX,

  REG_RDX,

  REG_RAX,

  REG_RCX,

  REG_RSP,

  REG_RIP,

  REG_EFL,

  REG_CSGSFS,

  REG_ERR,

  REG_TRAPNO,

  REG_OLDMASK,

  REG_CR2

};


struct _libc_fpxreg
{
  unsigned short int significand[4];
  unsigned short int exponent;
  unsigned short int __glibc_reserved1[3];
};

struct _libc_xmmreg
{
  __uint32_t element[4];
};

struct _libc_fpstate
{

  __uint16_t cwd;
  __uint16_t swd;
  __uint16_t ftw;
  __uint16_t fop;
  __uint64_t rip;
  __uint64_t rdp;
  __uint32_t mxcsr;
  __uint32_t mxcr_mask;
  struct _libc_fpxreg _st[8];
  struct _libc_xmmreg _xmm[16];
  __uint32_t __glibc_reserved1[24];
};


typedef struct _libc_fpstate *fpregset_t;


typedef struct
  {
    gregset_t gregs;

    fpregset_t fpregs;
    __extension__ unsigned long long __reserved1 [8];
} mcontext_t;


typedef struct ucontext_t
  {
    unsigned long int uc_flags;
    struct ucontext_t *uc_link;
    stack_t uc_stack;
    mcontext_t uc_mcontext;
    sigset_t uc_sigmask;
    struct _libc_fpstate __fpregs_mem;
    __extension__ unsigned long long int __ssp[4];
  } ucontext_t;
# 317 "/usr/include/signal.h" 2 3 4







extern int siginterrupt (int __sig, int __interrupt) noexcept (true)
  __attribute__ ((__deprecated__ ("Use sigaction with SA_RESTART instead")));

# 1 "/usr/include/bits/sigstack.h" 1 3 4
# 328 "/usr/include/signal.h" 2 3 4
# 1 "/usr/include/bits/sigstksz.h" 1 3 4
# 24 "/usr/include/bits/sigstksz.h" 3 4
# 1 "/usr/include/unistd.h" 1 3 4
# 27 "/usr/include/unistd.h" 3 4
extern "C" {
# 202 "/usr/include/unistd.h" 3 4
# 1 "/usr/include/bits/posix_opt.h" 1 3 4
# 203 "/usr/include/unistd.h" 2 3 4



# 1 "/usr/include/bits/environments.h" 1 3 4
# 22 "/usr/include/bits/environments.h" 3 4
# 1 "/usr/include/bits/wordsize.h" 1 3 4
# 23 "/usr/include/bits/environments.h" 2 3 4
# 207 "/usr/include/unistd.h" 2 3 4
# 220 "/usr/include/unistd.h" 3 4
typedef __ssize_t ssize_t;





# 1 "/usr/bin/../lib/clang/21/include/stddef.h" 1 3 4
# 93 "/usr/bin/../lib/clang/21/include/stddef.h" 3 4
# 1 "/usr/bin/../lib/clang/21/include/__stddef_size_t.h" 1 3 4
# 94 "/usr/bin/../lib/clang/21/include/stddef.h" 2 3 4
# 108 "/usr/bin/../lib/clang/21/include/stddef.h" 3 4
# 1 "/usr/bin/../lib/clang/21/include/__stddef_null.h" 1 3 4
# 109 "/usr/bin/../lib/clang/21/include/stddef.h" 2 3 4
# 227 "/usr/include/unistd.h" 2 3 4





typedef __gid_t gid_t;
# 243 "/usr/include/unistd.h" 3 4
typedef __off_t off_t;






typedef __off64_t off64_t;




typedef __useconds_t useconds_t;
# 267 "/usr/include/unistd.h" 3 4
typedef __intptr_t intptr_t;






typedef __socklen_t socklen_t;
# 287 "/usr/include/unistd.h" 3 4
extern int access (const char *__name, int __type) noexcept (true) __attribute__ ((__nonnull__ (1)));




extern int euidaccess (const char *__name, int __type)
     noexcept (true) __attribute__ ((__nonnull__ (1)));


extern int eaccess (const char *__name, int __type)
     noexcept (true) __attribute__ ((__nonnull__ (1)));


extern int execveat (int __fd, const char *__path, char *const __argv[],
                     char *const __envp[], int __flags)
    noexcept (true) __attribute__ ((__nonnull__ (2, 3)));






extern int faccessat (int __fd, const char *__file, int __type, int __flag)
     noexcept (true) __attribute__ ((__nonnull__ (2))) ;
# 339 "/usr/include/unistd.h" 3 4
extern __off_t lseek (int __fd, __off_t __offset, int __whence) noexcept (true);
# 350 "/usr/include/unistd.h" 3 4
extern __off64_t lseek64 (int __fd, __off64_t __offset, int __whence)
     noexcept (true);






extern int close (int __fd);




extern void closefrom (int __lowfd) noexcept (true);







extern ssize_t read (int __fd, void *__buf, size_t __nbytes)
                                                  ;





extern ssize_t write (int __fd, const void *__buf, size_t __n)
                                         ;
# 389 "/usr/include/unistd.h" 3 4
extern ssize_t pread (int __fd, void *__buf, size_t __nbytes,
        __off_t __offset)
                                                  ;






extern ssize_t pwrite (int __fd, const void *__buf, size_t __n,
         __off_t __offset)
                                         ;
# 422 "/usr/include/unistd.h" 3 4
extern ssize_t pread64 (int __fd, void *__buf, size_t __nbytes,
   __off64_t __offset)
                                                  ;


extern ssize_t pwrite64 (int __fd, const void *__buf, size_t __n,
    __off64_t __offset)
                                         ;







extern int pipe (int __pipedes[2]) noexcept (true) ;




extern int pipe2 (int __pipedes[2], int __flags) noexcept (true) ;
# 452 "/usr/include/unistd.h" 3 4
extern unsigned int alarm (unsigned int __seconds) noexcept (true);
# 464 "/usr/include/unistd.h" 3 4
extern unsigned int sleep (unsigned int __seconds);







extern __useconds_t ualarm (__useconds_t __value, __useconds_t __interval)
     noexcept (true);






extern int usleep (__useconds_t __useconds);
# 489 "/usr/include/unistd.h" 3 4
extern int pause (void);



extern int chown (const char *__file, __uid_t __owner, __gid_t __group)
     noexcept (true) __attribute__ ((__nonnull__ (1))) ;



extern int fchown (int __fd, __uid_t __owner, __gid_t __group) noexcept (true) ;




extern int lchown (const char *__file, __uid_t __owner, __gid_t __group)
     noexcept (true) __attribute__ ((__nonnull__ (1))) ;






extern int fchownat (int __fd, const char *__file, __uid_t __owner,
       __gid_t __group, int __flag)
     noexcept (true) __attribute__ ((__nonnull__ (2))) ;



extern int chdir (const char *__path) noexcept (true) __attribute__ ((__nonnull__ (1))) ;



extern int fchdir (int __fd) noexcept (true) ;
# 531 "/usr/include/unistd.h" 3 4
extern char *getcwd (char *__buf, size_t __size) noexcept (true) ;





extern char *get_current_dir_name (void) noexcept (true);







extern char *getwd (char *__buf)
     noexcept (true) __attribute__ ((__nonnull__ (1))) __attribute__ ((__deprecated__))
                                       ;




extern int dup (int __fd) noexcept (true) ;


extern int dup2 (int __fd, int __fd2) noexcept (true);




extern int dup3 (int __fd, int __fd2, int __flags) noexcept (true);



extern char **__environ;

extern char **environ;





extern int execve (const char *__path, char *const __argv[],
     char *const __envp[]) noexcept (true) __attribute__ ((__nonnull__ (1, 2)));




extern int fexecve (int __fd, char *const __argv[], char *const __envp[])
     noexcept (true) __attribute__ ((__nonnull__ (2)));




extern int execv (const char *__path, char *const __argv[])
     noexcept (true) __attribute__ ((__nonnull__ (1, 2)));



extern int execle (const char *__path, const char *__arg, ...)
     noexcept (true) __attribute__ ((__nonnull__ (1, 2)));



extern int execl (const char *__path, const char *__arg, ...)
     noexcept (true) __attribute__ ((__nonnull__ (1, 2)));



extern int execvp (const char *__file, char *const __argv[])
     noexcept (true) __attribute__ ((__nonnull__ (1, 2)));




extern int execlp (const char *__file, const char *__arg, ...)
     noexcept (true) __attribute__ ((__nonnull__ (1, 2)));




extern int execvpe (const char *__file, char *const __argv[],
      char *const __envp[])
     noexcept (true) __attribute__ ((__nonnull__ (1, 2)));





extern int nice (int __inc) noexcept (true) ;




extern void _exit (int __status) __attribute__ ((__noreturn__));





# 1 "/usr/include/bits/confname.h" 1 3 4
# 24 "/usr/include/bits/confname.h" 3 4
enum
  {
    _PC_LINK_MAX,

    _PC_MAX_CANON,

    _PC_MAX_INPUT,

    _PC_NAME_MAX,

    _PC_PATH_MAX,

    _PC_PIPE_BUF,

    _PC_CHOWN_RESTRICTED,

    _PC_NO_TRUNC,

    _PC_VDISABLE,

    _PC_SYNC_IO,

    _PC_ASYNC_IO,

    _PC_PRIO_IO,

    _PC_SOCK_MAXBUF,

    _PC_FILESIZEBITS,

    _PC_REC_INCR_XFER_SIZE,

    _PC_REC_MAX_XFER_SIZE,

    _PC_REC_MIN_XFER_SIZE,

    _PC_REC_XFER_ALIGN,

    _PC_ALLOC_SIZE_MIN,

    _PC_SYMLINK_MAX,

    _PC_2_SYMLINKS

  };


enum
  {
    _SC_ARG_MAX,

    _SC_CHILD_MAX,

    _SC_CLK_TCK,

    _SC_NGROUPS_MAX,

    _SC_OPEN_MAX,

    _SC_STREAM_MAX,

    _SC_TZNAME_MAX,

    _SC_JOB_CONTROL,

    _SC_SAVED_IDS,

    _SC_REALTIME_SIGNALS,

    _SC_PRIORITY_SCHEDULING,

    _SC_TIMERS,

    _SC_ASYNCHRONOUS_IO,

    _SC_PRIORITIZED_IO,

    _SC_SYNCHRONIZED_IO,

    _SC_FSYNC,

    _SC_MAPPED_FILES,

    _SC_MEMLOCK,

    _SC_MEMLOCK_RANGE,

    _SC_MEMORY_PROTECTION,

    _SC_MESSAGE_PASSING,

    _SC_SEMAPHORES,

    _SC_SHARED_MEMORY_OBJECTS,

    _SC_AIO_LISTIO_MAX,

    _SC_AIO_MAX,

    _SC_AIO_PRIO_DELTA_MAX,

    _SC_DELAYTIMER_MAX,

    _SC_MQ_OPEN_MAX,

    _SC_MQ_PRIO_MAX,

    _SC_VERSION,

    _SC_PAGESIZE,


    _SC_RTSIG_MAX,

    _SC_SEM_NSEMS_MAX,

    _SC_SEM_VALUE_MAX,

    _SC_SIGQUEUE_MAX,

    _SC_TIMER_MAX,




    _SC_BC_BASE_MAX,

    _SC_BC_DIM_MAX,

    _SC_BC_SCALE_MAX,

    _SC_BC_STRING_MAX,

    _SC_COLL_WEIGHTS_MAX,

    _SC_EQUIV_CLASS_MAX,

    _SC_EXPR_NEST_MAX,

    _SC_LINE_MAX,

    _SC_RE_DUP_MAX,

    _SC_CHARCLASS_NAME_MAX,


    _SC_2_VERSION,

    _SC_2_C_BIND,

    _SC_2_C_DEV,

    _SC_2_FORT_DEV,

    _SC_2_FORT_RUN,

    _SC_2_SW_DEV,

    _SC_2_LOCALEDEF,


    _SC_PII,

    _SC_PII_XTI,

    _SC_PII_SOCKET,

    _SC_PII_INTERNET,

    _SC_PII_OSI,

    _SC_POLL,

    _SC_SELECT,

    _SC_UIO_MAXIOV,

    _SC_IOV_MAX = _SC_UIO_MAXIOV,

    _SC_PII_INTERNET_STREAM,

    _SC_PII_INTERNET_DGRAM,

    _SC_PII_OSI_COTS,

    _SC_PII_OSI_CLTS,

    _SC_PII_OSI_M,

    _SC_T_IOV_MAX,



    _SC_THREADS,

    _SC_THREAD_SAFE_FUNCTIONS,

    _SC_GETGR_R_SIZE_MAX,

    _SC_GETPW_R_SIZE_MAX,

    _SC_LOGIN_NAME_MAX,

    _SC_TTY_NAME_MAX,

    _SC_THREAD_DESTRUCTOR_ITERATIONS,

    _SC_THREAD_KEYS_MAX,

    _SC_THREAD_STACK_MIN,

    _SC_THREAD_THREADS_MAX,

    _SC_THREAD_ATTR_STACKADDR,

    _SC_THREAD_ATTR_STACKSIZE,

    _SC_THREAD_PRIORITY_SCHEDULING,

    _SC_THREAD_PRIO_INHERIT,

    _SC_THREAD_PRIO_PROTECT,

    _SC_THREAD_PROCESS_SHARED,


    _SC_NPROCESSORS_CONF,

    _SC_NPROCESSORS_ONLN,

    _SC_PHYS_PAGES,

    _SC_AVPHYS_PAGES,

    _SC_ATEXIT_MAX,

    _SC_PASS_MAX,


    _SC_XOPEN_VERSION,

    _SC_XOPEN_XCU_VERSION,

    _SC_XOPEN_UNIX,

    _SC_XOPEN_CRYPT,

    _SC_XOPEN_ENH_I18N,

    _SC_XOPEN_SHM,


    _SC_2_CHAR_TERM,

    _SC_2_C_VERSION,

    _SC_2_UPE,


    _SC_XOPEN_XPG2,

    _SC_XOPEN_XPG3,

    _SC_XOPEN_XPG4,


    _SC_CHAR_BIT,

    _SC_CHAR_MAX,

    _SC_CHAR_MIN,

    _SC_INT_MAX,

    _SC_INT_MIN,

    _SC_LONG_BIT,

    _SC_WORD_BIT,

    _SC_MB_LEN_MAX,

    _SC_NZERO,

    _SC_SSIZE_MAX,

    _SC_SCHAR_MAX,

    _SC_SCHAR_MIN,

    _SC_SHRT_MAX,

    _SC_SHRT_MIN,

    _SC_UCHAR_MAX,

    _SC_UINT_MAX,

    _SC_ULONG_MAX,

    _SC_USHRT_MAX,


    _SC_NL_ARGMAX,

    _SC_NL_LANGMAX,

    _SC_NL_MSGMAX,

    _SC_NL_NMAX,

    _SC_NL_SETMAX,

    _SC_NL_TEXTMAX,


    _SC_XBS5_ILP32_OFF32,

    _SC_XBS5_ILP32_OFFBIG,

    _SC_XBS5_LP64_OFF64,

    _SC_XBS5_LPBIG_OFFBIG,


    _SC_XOPEN_LEGACY,

    _SC_XOPEN_REALTIME,

    _SC_XOPEN_REALTIME_THREADS,


    _SC_ADVISORY_INFO,

    _SC_BARRIERS,

    _SC_BASE,

    _SC_C_LANG_SUPPORT,

    _SC_C_LANG_SUPPORT_R,

    _SC_CLOCK_SELECTION,

    _SC_CPUTIME,

    _SC_THREAD_CPUTIME,

    _SC_DEVICE_IO,

    _SC_DEVICE_SPECIFIC,

    _SC_DEVICE_SPECIFIC_R,

    _SC_FD_MGMT,

    _SC_FIFO,

    _SC_PIPE,

    _SC_FILE_ATTRIBUTES,

    _SC_FILE_LOCKING,

    _SC_FILE_SYSTEM,

    _SC_MONOTONIC_CLOCK,

    _SC_MULTI_PROCESS,

    _SC_SINGLE_PROCESS,

    _SC_NETWORKING,

    _SC_READER_WRITER_LOCKS,

    _SC_SPIN_LOCKS,

    _SC_REGEXP,

    _SC_REGEX_VERSION,

    _SC_SHELL,

    _SC_SIGNALS,

    _SC_SPAWN,

    _SC_SPORADIC_SERVER,

    _SC_THREAD_SPORADIC_SERVER,

    _SC_SYSTEM_DATABASE,

    _SC_SYSTEM_DATABASE_R,

    _SC_TIMEOUTS,

    _SC_TYPED_MEMORY_OBJECTS,

    _SC_USER_GROUPS,

    _SC_USER_GROUPS_R,

    _SC_2_PBS,

    _SC_2_PBS_ACCOUNTING,

    _SC_2_PBS_LOCATE,

    _SC_2_PBS_MESSAGE,

    _SC_2_PBS_TRACK,

    _SC_SYMLOOP_MAX,

    _SC_STREAMS,

    _SC_2_PBS_CHECKPOINT,


    _SC_V6_ILP32_OFF32,

    _SC_V6_ILP32_OFFBIG,

    _SC_V6_LP64_OFF64,

    _SC_V6_LPBIG_OFFBIG,


    _SC_HOST_NAME_MAX,

    _SC_TRACE,

    _SC_TRACE_EVENT_FILTER,

    _SC_TRACE_INHERIT,

    _SC_TRACE_LOG,


    _SC_LEVEL1_ICACHE_SIZE,

    _SC_LEVEL1_ICACHE_ASSOC,

    _SC_LEVEL1_ICACHE_LINESIZE,

    _SC_LEVEL1_DCACHE_SIZE,

    _SC_LEVEL1_DCACHE_ASSOC,

    _SC_LEVEL1_DCACHE_LINESIZE,

    _SC_LEVEL2_CACHE_SIZE,

    _SC_LEVEL2_CACHE_ASSOC,

    _SC_LEVEL2_CACHE_LINESIZE,

    _SC_LEVEL3_CACHE_SIZE,

    _SC_LEVEL3_CACHE_ASSOC,

    _SC_LEVEL3_CACHE_LINESIZE,

    _SC_LEVEL4_CACHE_SIZE,

    _SC_LEVEL4_CACHE_ASSOC,

    _SC_LEVEL4_CACHE_LINESIZE,



    _SC_IPV6 = _SC_LEVEL1_ICACHE_SIZE + 50,

    _SC_RAW_SOCKETS,


    _SC_V7_ILP32_OFF32,

    _SC_V7_ILP32_OFFBIG,

    _SC_V7_LP64_OFF64,

    _SC_V7_LPBIG_OFFBIG,


    _SC_SS_REPL_MAX,


    _SC_TRACE_EVENT_NAME_MAX,

    _SC_TRACE_NAME_MAX,

    _SC_TRACE_SYS_MAX,

    _SC_TRACE_USER_EVENT_MAX,


    _SC_XOPEN_STREAMS,


    _SC_THREAD_ROBUST_PRIO_INHERIT,

    _SC_THREAD_ROBUST_PRIO_PROTECT,


    _SC_MINSIGSTKSZ,


    _SC_SIGSTKSZ

  };


enum
  {
    _CS_PATH,


    _CS_V6_WIDTH_RESTRICTED_ENVS,



    _CS_GNU_LIBC_VERSION,

    _CS_GNU_LIBPTHREAD_VERSION,


    _CS_V5_WIDTH_RESTRICTED_ENVS,



    _CS_V7_WIDTH_RESTRICTED_ENVS,



    _CS_LFS_CFLAGS = 1000,

    _CS_LFS_LDFLAGS,

    _CS_LFS_LIBS,

    _CS_LFS_LINTFLAGS,

    _CS_LFS64_CFLAGS,

    _CS_LFS64_LDFLAGS,

    _CS_LFS64_LIBS,

    _CS_LFS64_LINTFLAGS,


    _CS_XBS5_ILP32_OFF32_CFLAGS = 1100,

    _CS_XBS5_ILP32_OFF32_LDFLAGS,

    _CS_XBS5_ILP32_OFF32_LIBS,

    _CS_XBS5_ILP32_OFF32_LINTFLAGS,

    _CS_XBS5_ILP32_OFFBIG_CFLAGS,

    _CS_XBS5_ILP32_OFFBIG_LDFLAGS,

    _CS_XBS5_ILP32_OFFBIG_LIBS,

    _CS_XBS5_ILP32_OFFBIG_LINTFLAGS,

    _CS_XBS5_LP64_OFF64_CFLAGS,

    _CS_XBS5_LP64_OFF64_LDFLAGS,

    _CS_XBS5_LP64_OFF64_LIBS,

    _CS_XBS5_LP64_OFF64_LINTFLAGS,

    _CS_XBS5_LPBIG_OFFBIG_CFLAGS,

    _CS_XBS5_LPBIG_OFFBIG_LDFLAGS,

    _CS_XBS5_LPBIG_OFFBIG_LIBS,

    _CS_XBS5_LPBIG_OFFBIG_LINTFLAGS,


    _CS_POSIX_V6_ILP32_OFF32_CFLAGS,

    _CS_POSIX_V6_ILP32_OFF32_LDFLAGS,

    _CS_POSIX_V6_ILP32_OFF32_LIBS,

    _CS_POSIX_V6_ILP32_OFF32_LINTFLAGS,

    _CS_POSIX_V6_ILP32_OFFBIG_CFLAGS,

    _CS_POSIX_V6_ILP32_OFFBIG_LDFLAGS,

    _CS_POSIX_V6_ILP32_OFFBIG_LIBS,

    _CS_POSIX_V6_ILP32_OFFBIG_LINTFLAGS,

    _CS_POSIX_V6_LP64_OFF64_CFLAGS,

    _CS_POSIX_V6_LP64_OFF64_LDFLAGS,

    _CS_POSIX_V6_LP64_OFF64_LIBS,

    _CS_POSIX_V6_LP64_OFF64_LINTFLAGS,

    _CS_POSIX_V6_LPBIG_OFFBIG_CFLAGS,

    _CS_POSIX_V6_LPBIG_OFFBIG_LDFLAGS,

    _CS_POSIX_V6_LPBIG_OFFBIG_LIBS,

    _CS_POSIX_V6_LPBIG_OFFBIG_LINTFLAGS,


    _CS_POSIX_V7_ILP32_OFF32_CFLAGS,

    _CS_POSIX_V7_ILP32_OFF32_LDFLAGS,

    _CS_POSIX_V7_ILP32_OFF32_LIBS,

    _CS_POSIX_V7_ILP32_OFF32_LINTFLAGS,

    _CS_POSIX_V7_ILP32_OFFBIG_CFLAGS,

    _CS_POSIX_V7_ILP32_OFFBIG_LDFLAGS,

    _CS_POSIX_V7_ILP32_OFFBIG_LIBS,

    _CS_POSIX_V7_ILP32_OFFBIG_LINTFLAGS,

    _CS_POSIX_V7_LP64_OFF64_CFLAGS,

    _CS_POSIX_V7_LP64_OFF64_LDFLAGS,

    _CS_POSIX_V7_LP64_OFF64_LIBS,

    _CS_POSIX_V7_LP64_OFF64_LINTFLAGS,

    _CS_POSIX_V7_LPBIG_OFFBIG_CFLAGS,

    _CS_POSIX_V7_LPBIG_OFFBIG_LDFLAGS,

    _CS_POSIX_V7_LPBIG_OFFBIG_LIBS,

    _CS_POSIX_V7_LPBIG_OFFBIG_LINTFLAGS,


    _CS_V6_ENV,

    _CS_V7_ENV

  };
# 631 "/usr/include/unistd.h" 2 3 4


extern long int pathconf (const char *__path, int __name)
     noexcept (true) __attribute__ ((__nonnull__ (1)));


extern long int fpathconf (int __fd, int __name) noexcept (true);


extern long int sysconf (int __name) noexcept (true);



extern size_t confstr (int __name, char *__buf, size_t __len) noexcept (true)
                                                  ;




extern __pid_t getpid (void) noexcept (true);


extern __pid_t getppid (void) noexcept (true);


extern __pid_t getpgrp (void) noexcept (true);


extern __pid_t __getpgid (__pid_t __pid) noexcept (true);

extern __pid_t getpgid (__pid_t __pid) noexcept (true);






extern int setpgid (__pid_t __pid, __pid_t __pgid) noexcept (true);
# 682 "/usr/include/unistd.h" 3 4
extern int setpgrp (void) noexcept (true);






extern __pid_t setsid (void) noexcept (true);



extern __pid_t getsid (__pid_t __pid) noexcept (true);



extern __uid_t getuid (void) noexcept (true);


extern __uid_t geteuid (void) noexcept (true);


extern __gid_t getgid (void) noexcept (true);


extern __gid_t getegid (void) noexcept (true);




extern int getgroups (int __size, __gid_t __list[]) noexcept (true)
                                                  ;


extern int group_member (__gid_t __gid) noexcept (true);






extern int setuid (__uid_t __uid) noexcept (true) ;




extern int setreuid (__uid_t __ruid, __uid_t __euid) noexcept (true) ;




extern int seteuid (__uid_t __uid) noexcept (true) ;






extern int setgid (__gid_t __gid) noexcept (true) ;




extern int setregid (__gid_t __rgid, __gid_t __egid) noexcept (true) ;




extern int setegid (__gid_t __gid) noexcept (true) ;





extern int getresuid (__uid_t *__ruid, __uid_t *__euid, __uid_t *__suid)
     noexcept (true);



extern int getresgid (__gid_t *__rgid, __gid_t *__egid, __gid_t *__sgid)
     noexcept (true);



extern int setresuid (__uid_t __ruid, __uid_t __euid, __uid_t __suid)
     noexcept (true) ;



extern int setresgid (__gid_t __rgid, __gid_t __egid, __gid_t __sgid)
     noexcept (true) ;






extern __pid_t fork (void) noexcept (true);







extern __pid_t vfork (void) noexcept (true);






extern __pid_t _Fork (void) noexcept (true);





extern char *ttyname (int __fd) noexcept (true);



extern int ttyname_r (int __fd, char *__buf, size_t __buflen)
     noexcept (true) __attribute__ ((__nonnull__ (2)))
                                                   ;



extern int isatty (int __fd) noexcept (true);




extern int ttyslot (void) noexcept (true);




extern int link (const char *__from, const char *__to)
     noexcept (true) __attribute__ ((__nonnull__ (1, 2))) ;




extern int linkat (int __fromfd, const char *__from, int __tofd,
     const char *__to, int __flags)
     noexcept (true) __attribute__ ((__nonnull__ (2, 4))) ;




extern int symlink (const char *__from, const char *__to)
     noexcept (true) __attribute__ ((__nonnull__ (1, 2))) ;




extern ssize_t readlink (const char *__restrict __path,
    char *__restrict __buf, size_t __len)
     noexcept (true) __attribute__ ((__nonnull__ (1, 2)))
                                                   ;





extern int symlinkat (const char *__from, int __tofd,
        const char *__to) noexcept (true) __attribute__ ((__nonnull__ (1, 3))) ;


extern ssize_t readlinkat (int __fd, const char *__restrict __path,
      char *__restrict __buf, size_t __len)
     noexcept (true) __attribute__ ((__nonnull__ (2, 3)))
                                                   ;



extern int unlink (const char *__name) noexcept (true) __attribute__ ((__nonnull__ (1)));



extern int unlinkat (int __fd, const char *__name, int __flag)
     noexcept (true) __attribute__ ((__nonnull__ (2)));



extern int rmdir (const char *__path) noexcept (true) __attribute__ ((__nonnull__ (1)));



extern __pid_t tcgetpgrp (int __fd) noexcept (true);


extern int tcsetpgrp (int __fd, __pid_t __pgrp_id) noexcept (true);






extern char *getlogin (void);







extern int getlogin_r (char *__name, size_t __name_len) __attribute__ ((__nonnull__ (1)))
                                                  ;




extern int setlogin (const char *__name) noexcept (true) __attribute__ ((__nonnull__ (1)));







# 1 "/usr/include/bits/getopt_posix.h" 1 3 4
# 27 "/usr/include/bits/getopt_posix.h" 3 4
# 1 "/usr/include/bits/getopt_core.h" 1 3 4
# 28 "/usr/include/bits/getopt_core.h" 3 4
extern "C" {







extern char *optarg;
# 50 "/usr/include/bits/getopt_core.h" 3 4
extern int optind;




extern int opterr;



extern int optopt;
# 91 "/usr/include/bits/getopt_core.h" 3 4
extern int getopt (int ___argc, char *const *___argv, const char *__shortopts)
       noexcept (true) __attribute__ ((__nonnull__ (2, 3)));

}
# 28 "/usr/include/bits/getopt_posix.h" 2 3 4

extern "C" {
# 49 "/usr/include/bits/getopt_posix.h" 3 4
}
# 904 "/usr/include/unistd.h" 2 3 4







extern int gethostname (char *__name, size_t __len) noexcept (true) __attribute__ ((__nonnull__ (1)))
                                                  ;






extern int sethostname (const char *__name, size_t __len)
     noexcept (true) __attribute__ ((__nonnull__ (1))) ;



extern int sethostid (long int __id) noexcept (true) ;





extern int getdomainname (char *__name, size_t __len)
     noexcept (true) __attribute__ ((__nonnull__ (1)))
                                                   ;
extern int setdomainname (const char *__name, size_t __len)
     noexcept (true) __attribute__ ((__nonnull__ (1))) ;




extern int vhangup (void) noexcept (true);


extern int revoke (const char *__file) noexcept (true) __attribute__ ((__nonnull__ (1))) ;







extern int profil (unsigned short int *__sample_buffer, size_t __size,
     size_t __offset, unsigned int __scale)
     noexcept (true) __attribute__ ((__nonnull__ (1)));





extern int acct (const char *__name) noexcept (true);



extern char *getusershell (void) noexcept (true);
extern void endusershell (void) noexcept (true);
extern void setusershell (void) noexcept (true);





extern int daemon (int __nochdir, int __noclose) noexcept (true) ;






extern int chroot (const char *__path) noexcept (true) __attribute__ ((__nonnull__ (1))) ;



extern char *getpass (const char *__prompt) __attribute__ ((__nonnull__ (1)));







extern int fsync (int __fd);





extern int syncfs (int __fd) noexcept (true);






extern long int gethostid (void);


extern void sync (void) noexcept (true);





extern int getpagesize (void) noexcept (true) __attribute__ ((__const__));




extern int getdtablesize (void) noexcept (true);
# 1026 "/usr/include/unistd.h" 3 4
extern int truncate (const char *__file, __off_t __length)
     noexcept (true) __attribute__ ((__nonnull__ (1))) ;
# 1038 "/usr/include/unistd.h" 3 4
extern int truncate64 (const char *__file, __off64_t __length)
     noexcept (true) __attribute__ ((__nonnull__ (1))) ;
# 1049 "/usr/include/unistd.h" 3 4
extern int ftruncate (int __fd, __off_t __length) noexcept (true) ;
# 1059 "/usr/include/unistd.h" 3 4
extern int ftruncate64 (int __fd, __off64_t __length) noexcept (true) ;
# 1070 "/usr/include/unistd.h" 3 4
extern int brk (void *__addr) noexcept (true) ;





extern void *sbrk (intptr_t __delta) noexcept (true);
# 1091 "/usr/include/unistd.h" 3 4
extern long int syscall (long int __sysno, ...) noexcept (true);
# 1114 "/usr/include/unistd.h" 3 4
extern int lockf (int __fd, int __cmd, __off_t __len) ;
# 1124 "/usr/include/unistd.h" 3 4
extern int lockf64 (int __fd, int __cmd, __off64_t __len) ;
# 1142 "/usr/include/unistd.h" 3 4
ssize_t copy_file_range (int __infd, __off64_t *__pinoff,
    int __outfd, __off64_t *__poutoff,
    size_t __length, unsigned int __flags);





extern int fdatasync (int __fildes);
# 1162 "/usr/include/unistd.h" 3 4
extern char *crypt (const char *__key, const char *__salt)
     noexcept (true) __attribute__ ((__nonnull__ (1, 2)));







extern void swab (const void *__restrict __from, void *__restrict __to,
    ssize_t __n) noexcept (true) __attribute__ ((__nonnull__ (1, 2)))

                                          ;
# 1201 "/usr/include/unistd.h" 3 4
int getentropy (void *__buffer, size_t __length)
                                          ;
# 1211 "/usr/include/unistd.h" 3 4
extern int close_range (unsigned int __fd, unsigned int __max_fd,
   int __flags) noexcept (true);
# 1221 "/usr/include/unistd.h" 3 4
# 1 "/usr/include/bits/unistd_ext.h" 1 3 4
# 34 "/usr/include/bits/unistd_ext.h" 3 4
extern __pid_t gettid (void) noexcept (true);



# 1 "/usr/include/linux/close_range.h" 1 3 4
# 39 "/usr/include/bits/unistd_ext.h" 2 3 4
# 1222 "/usr/include/unistd.h" 2 3 4

}
# 25 "/usr/include/bits/sigstksz.h" 2 3 4
# 329 "/usr/include/signal.h" 2 3 4
# 1 "/usr/include/bits/ss_flags.h" 1 3 4
# 27 "/usr/include/bits/ss_flags.h" 3 4
enum
{
  SS_ONSTACK = 1,

  SS_DISABLE

};
# 330 "/usr/include/signal.h" 2 3 4



extern int sigaltstack (const stack_t *__restrict __ss,
   stack_t *__restrict __oss) noexcept (true);




# 1 "/usr/include/bits/types/struct_sigstack.h" 1 3 4
# 23 "/usr/include/bits/types/struct_sigstack.h" 3 4
struct sigstack
  {
    void *ss_sp;
    int ss_onstack;
  };
# 340 "/usr/include/signal.h" 2 3 4







extern int sigstack (struct sigstack *__ss, struct sigstack *__oss)
     noexcept (true) __attribute__ ((__deprecated__));






extern int sighold (int __sig) noexcept (true)
  __attribute__ ((__deprecated__ ("Use the sigprocmask function instead")));


extern int sigrelse (int __sig) noexcept (true)
  __attribute__ ((__deprecated__ ("Use the sigprocmask function instead")));


extern int sigignore (int __sig) noexcept (true)
  __attribute__ ((__deprecated__ ("Use the signal function instead")));


extern __sighandler_t sigset (int __sig, __sighandler_t __disp) noexcept (true)
  __attribute__ ((__deprecated__ ("Use the signal and sigprocmask functions instead")));






# 1 "/usr/include/bits/pthreadtypes.h" 1 3 4
# 23 "/usr/include/bits/pthreadtypes.h" 3 4
# 1 "/usr/include/bits/thread-shared-types.h" 1 3 4
# 44 "/usr/include/bits/thread-shared-types.h" 3 4
# 1 "/usr/include/bits/pthreadtypes-arch.h" 1 3 4
# 21 "/usr/include/bits/pthreadtypes-arch.h" 3 4
# 1 "/usr/include/bits/wordsize.h" 1 3 4
# 22 "/usr/include/bits/pthreadtypes-arch.h" 2 3 4
# 45 "/usr/include/bits/thread-shared-types.h" 2 3 4

# 1 "/usr/include/bits/atomic_wide_counter.h" 1 3 4
# 25 "/usr/include/bits/atomic_wide_counter.h" 3 4
typedef union
{
  __extension__ unsigned long long int __value64;
  struct
  {
    unsigned int __low;
    unsigned int __high;
  } __value32;
} __atomic_wide_counter;
# 47 "/usr/include/bits/thread-shared-types.h" 2 3 4




typedef struct __pthread_internal_list
{
  struct __pthread_internal_list *__prev;
  struct __pthread_internal_list *__next;
} __pthread_list_t;

typedef struct __pthread_internal_slist
{
  struct __pthread_internal_slist *__next;
} __pthread_slist_t;
# 76 "/usr/include/bits/thread-shared-types.h" 3 4
# 1 "/usr/include/bits/struct_mutex.h" 1 3 4
# 22 "/usr/include/bits/struct_mutex.h" 3 4
struct __pthread_mutex_s
{
  int __lock;
  unsigned int __count;
  int __owner;

  unsigned int __nusers;



  int __kind;

  short __spins;
  short __elision;
  __pthread_list_t __list;
# 53 "/usr/include/bits/struct_mutex.h" 3 4
};
# 77 "/usr/include/bits/thread-shared-types.h" 2 3 4
# 89 "/usr/include/bits/thread-shared-types.h" 3 4
# 1 "/usr/include/bits/struct_rwlock.h" 1 3 4
# 23 "/usr/include/bits/struct_rwlock.h" 3 4
struct __pthread_rwlock_arch_t
{
  unsigned int __readers;
  unsigned int __writers;
  unsigned int __wrphase_futex;
  unsigned int __writers_futex;
  unsigned int __pad3;
  unsigned int __pad4;

  int __cur_writer;
  int __shared;
  signed char __rwelision;




  unsigned char __pad1[7];


  unsigned long int __pad2;


  unsigned int __flags;
# 55 "/usr/include/bits/struct_rwlock.h" 3 4
};
# 90 "/usr/include/bits/thread-shared-types.h" 2 3 4




struct __pthread_cond_s
{
  __atomic_wide_counter __wseq;
  __atomic_wide_counter __g1_start;
  unsigned int __g_size[2] ;
  unsigned int __g1_orig_size;
  unsigned int __wrefs;
  unsigned int __g_signals[2];
  unsigned int __unused_initialized_1;
  unsigned int __unused_initialized_2;
};

typedef unsigned int __tss_t;
typedef unsigned long int __thrd_t;

typedef struct
{
  int __data ;
} __once_flag;
# 24 "/usr/include/bits/pthreadtypes.h" 2 3 4



typedef unsigned long int pthread_t;




typedef union
{
  char __size[4];
  int __align;
} pthread_mutexattr_t;




typedef union
{
  char __size[4];
  int __align;
} pthread_condattr_t;



typedef unsigned int pthread_key_t;



typedef int pthread_once_t;


union pthread_attr_t
{
  char __size[56];
  long int __align;
};






typedef union
{
  struct __pthread_mutex_s __data;
  char __size[40];
  long int __align;
} pthread_mutex_t;


typedef union
{
  struct __pthread_cond_s __data;
  char __size[48];
  __extension__ long long int __align;
} pthread_cond_t;





typedef union
{
  struct __pthread_rwlock_arch_t __data;
  char __size[56];
  long int __align;
} pthread_rwlock_t;

typedef union
{
  char __size[8];
  long int __align;
} pthread_rwlockattr_t;





typedef volatile int pthread_spinlock_t;




typedef union
{
  char __size[32];
  long int __align;
} pthread_barrier_t;

typedef union
{
  char __size[4];
  int __align;
} pthread_barrierattr_t;
# 376 "/usr/include/signal.h" 2 3 4
# 1 "/usr/include/bits/sigthread.h" 1 3 4
# 31 "/usr/include/bits/sigthread.h" 3 4
extern int pthread_sigmask (int __how,
       const __sigset_t *__restrict __newmask,
       __sigset_t *__restrict __oldmask)noexcept (true);


extern int pthread_kill (pthread_t __threadid, int __signo) noexcept (true);



extern int pthread_sigqueue (pthread_t __threadid, int __signo,
        const union sigval __value) noexcept (true);
# 377 "/usr/include/signal.h" 2 3 4






extern int __libc_current_sigrtmin (void) noexcept (true);

extern int __libc_current_sigrtmax (void) noexcept (true);





# 1 "/usr/include/bits/signal_ext.h" 1 3 4
# 29 "/usr/include/bits/signal_ext.h" 3 4
extern int tgkill (__pid_t __tgid, __pid_t __tid, int __signal);
# 392 "/usr/include/signal.h" 2 3 4

}
# 41 "./include/commons/startup.hh" 2

extern "C" void* malloc(size_t);
extern "C" void* aligned_alloc(size_t, size_t);
extern "C" void free(void*);


inline u32 cm::FastPRNG::_state;
;
inline u32 cm::ND_PRNG::_state;
;


namespace cm {

[[noreturn]]
inline void ::cm::panic(char const* message, char const* reason, SourceLocation src)
{
    if (message == nullptr)
        message = "";

    if (reason == nullptr)
        reason = "";

    auto write = [](char const* str, usize len) {
        LinuxSyscall(LinuxSyscall.write, 2, u64(str), len);
    };
    write("\x1B[31m", sizeof("\x1B[31m"));
    write(message, CArrays::stringLen(message));

    if (src.function() != nullptr || src.file() != nullptr) {
        write((" in "), 4);
    }
    if (src.function() != nullptr) {
        write((src.function()), CArrays::stringLen(src.function()));
    }
    if (src.file() != nullptr) {
        write((" at \""), 5);
        write(src.file(), CArrays::stringLen(src.file()));
        write("\"", 1);
    }
    if (reason != nullptr) {
        write(".\n\treason: ", 11);
        write(reason, CArrays::stringLen(reason));
    }
    Profiler::printStackTrace();
    write("\x1B[0m", sizeof("\x1B[0m"));
    __builtin_trap();

}

inline void _emergencyPrint(char const* msg)
{
    auto write = [](char const* str, usize len) {
        LinuxSyscall(LinuxSyscall.write, 2, u64(str), len);
    };
    write(msg, CArrays::stringLen(msg));
}

}



namespace cm {
namespace startup {

constexpr static auto MAX_PROFILER_STACK_FRAMES = 65_KB;
static Array<::cm::Profiler::StackFrame, MAX_PROFILER_STACK_FRAMES> s_stackFrames;
static unsigned int s_currentStackFrameIndex{};
static bool s_printed = false;
static Array<char, 1024> s_stackTraceBuffer;
}

[[gnu::no_instrument_function]]
inline void Profiler::printStackTrace()
{
    if (startup::s_printed)
        return;

    startup::s_printed = true;
    _emergencyPrint("\nStack Trace:\n");
# 150 "./include/commons/startup.hh"
}






extern "C" [[maybe_unused, gnu::no_instrument_function]]
inline void __cyg_profile_func_enter(void* funcAddr, void* callAddr)
{
    startup::s_currentStackFrameIndex++;
    if (startup::s_currentStackFrameIndex >= startup::MAX_PROFILER_STACK_FRAMES) {
        __builtin_trap();
    } else {
        startup::s_stackFrames[startup::s_currentStackFrameIndex] = {funcAddr, callAddr, 0};
    }
}







extern "C" [[maybe_unused, gnu::no_instrument_function]]
inline void __cyg_profile_func_exit(void* funcAddr, void* callAddr)
{
    (void)funcAddr;
    (void)callAddr;
    if (startup::s_currentStackFrameIndex <= 0) {
        __builtin_trap();
    } else {
        startup::s_stackFrames[startup::s_currentStackFrameIndex].tElapsed = 0;
        startup::s_currentStackFrameIndex--;
    }
}





[[gnu::constructor(100), gnu::no_instrument_function]]
inline void signalHandlers()
{

    struct sigaction const segFaultHandler{



        .__sigaction_handler.sa_handler




        = [] [[gnu::no_instrument_function]] (int) -> void {
            _emergencyPrint("\x1B[31mSegmentation Fault\n");
            Profiler::printStackTrace();
            _emergencyPrint("\x1B[0m");
            raise(9);
        },
        .sa_mask = {0},
        .sa_flags = 0,
        .sa_restorer = nullptr};

    struct sigaction const trapHandler{


        .__sigaction_handler.sa_handler



        = [] [[gnu::no_instrument_function]] (int) -> void {
            _emergencyPrint("\x1B[31mPanic: Program halted (CPU trap)\n");
            Profiler::printStackTrace();
            _emergencyPrint("\x1B[0m");
            raise(9);
        },
        .sa_mask = {0},
        .sa_flags = 0,
        .sa_restorer = nullptr};

    sigaction(11, &segFaultHandler, nullptr);
    sigaction(4, &trapHandler, nullptr);
}


}


constexpr std::align_val_t DEFAULT_ALIGNMENT = std::align_val_t(8);


inline void* newImpl(std::size_t size, std::align_val_t alignment) noexcept
{
    void* ptr;
    if (alignment == DEFAULT_ALIGNMENT) {
        ptr = malloc(size);
    } else {
        ptr = aligned_alloc(static_cast<size_t>(alignment), size);
    }
    ::cm::Assert(ptr);
    return ptr;
}

inline void* newImplNothrow(std::size_t size, std::align_val_t alignment) noexcept
{
    return aligned_alloc(static_cast<size_t>(alignment), size);

}

inline void deleteImpl(void* ptr)
{

    free(ptr);
    (void)ptr;
}






void* operator new(std::size_t size) { return newImpl(size, DEFAULT_ALIGNMENT); }

void* operator new[](std::size_t size) { return newImpl(size, DEFAULT_ALIGNMENT); }

void* operator new(std::size_t size, std::align_val_t al) { return newImpl(size, al); }

void* operator new[](std::size_t size, std::align_val_t al) { return newImpl(size, al); }







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






void operator delete(void* ptr) noexcept { return deleteImpl(ptr); }

void operator delete[](void* ptr) noexcept { return deleteImpl(ptr); }

void operator delete(void* ptr, long unsigned int) noexcept { return deleteImpl(ptr); }

void operator delete[](void* ptr, long unsigned int) noexcept { return deleteImpl(ptr); }

extern "C" [[noreturn]]
void __cxa_pure_virtual()
{
    __builtin_trap();
}
# 2 "./test/blank.cc" 2

