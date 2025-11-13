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
#ifdef __inline_core_header__

struct Nothing
{};

struct SafeBinaryMoveable
{};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      HELPERS
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// clang-format off

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

// clang-format on

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      TYPE MANIPULATION TOOLS
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
using ConstRemoved = typename TConstRemoved<T>::Type;

template<typename T>
using VolatileRemoved = typename TVolatileRemoved<T>::Type;

template<typename T>
using CVRemoved = typename TConstRemoved<typename TVolatileRemoved<T>::Type>::Type;

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


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      TYPE CATEGORIZATION TRAITS
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if __has_builtin(__is_same)
template<typename A, typename B>
concept IsSame = __is_same(A, B);
#else

template<typename A, typename B>
constexpr inline bool is_same_v = false;

template<typename A>
constexpr inline bool is_same_v<A, A> = true;

template<typename A, typename B>
concept IsSame = is_same_v<A, B>;

#endif

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

#if __has_builtin(__is_pointer)
template<typename T>
concept IsPointer = __is_pointer(T);
#else

template<typename A>
constexpr inline bool is_pointer_v = false;
template<typename A>
constexpr inline bool is_pointer_v<A*> = true;
template<typename A>
constexpr inline bool is_pointer_v<A* const> = true;
template<typename A>
constexpr inline bool is_pointer_v<A* volatile> = true;
template<typename A>
constexpr inline bool is_pointer_v<A* const volatile> = true;

template<typename T>
concept IsPointer = is_pointer_v<T>;
#endif

static_assert(IsPointer<int volatile*>);
static_assert(IsPointer<int***>);
static_assert(IsPointer<int** const* const volatile>);
static_assert(IsPointer<void (*)(void*) noexcept>);
static_assert(!IsPointer<int>);


template<typename T>
concept IsReference = bool{!IsSame<T, RefRemoved<T>>};

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

#if __has_builtin(__is_convertible)
template<typename From, typename To>
concept ConvertibleTo = __is_convertible(From, To);
#elif _MSC_VER
template<typename From, typename To>
concept ConvertibleTo = __is_convertible_to(From, To);
#else
#error "Not implemented"
#endif


template<typename T>
concept TypeIsBool = ConvertibleTo<T, bool>;

template<auto value>
concept ValueIsBool = TypeIsBool<decltype(value)>;

// The dumbasses at microsoft decided __underlying_type should fail if T is a primitive non-enum type, whereas GCC and
// clang simply return the same type. Now this extra boilerplate needs to be written. Thanks, microsoft!

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
}  // namespace detail

template<typename T>
using UnderlyingTypeOf = typename detail::GetUnderlyingTypeOf<T, IsEnum<T>>::Type;

template<typename T, typename U>
concept IsUnderlyingType = IsSame<UnderlyingTypeOf<T>, U>;

template<typename T, typename... A>
concept IsUnderlyingTypeOneOf = ((IsSame<UnderlyingTypeOf<T>, A>) || ...);


#if __has_builtin(__is_integral)
template<typename T>
concept IsInteger = __is_integral(UnderlyingTypeOf<T>);

template<typename T>
concept IsIntegerPrimitiveType = __is_integral(T);

#else
template<typename T>
concept IsInteger = IsUnderlyingTypeOneOf<
    T, bool, char, signed char, unsigned char, short, unsigned short, int, unsigned int, long, unsigned long, long long,
    unsigned long long, char8_t, char16_t, char32_t, wchar_t>;

template<typename T>
concept IsIntegerPrimitiveType = IsSameAsOneOf<
    CVRefRemoved<T>, bool, char, signed char, unsigned char, short, unsigned short, int, unsigned int, long,
    unsigned long, long long, unsigned long long, char8_t, char16_t, char32_t, wchar_t>;
#endif

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
concept IsBool = IsUnderlyingTypeOneOf<T, bool>;

template<typename T>
concept IsMutable = requires (T a, T b) {
    {
        a = b
    };
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      OBJECT LIFETIME TYPE TRAITS
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<class T>
concept DefaultConstructible = requires { T{}; };

template<class T, typename... Args>
concept Constructible = __is_constructible(CVRefRemoved<T>, Args...);

template<class T>
concept CopyConstructible = Constructible<T, T const&>;

template<typename T>
concept TriviallyCopyConstructible = __is_trivially_copyable(T);

template<typename T>
concept TriviallyDefaultConstructible = __is_trivially_constructible(T);

template<typename T, typename U = T>
concept TriviallyAssignable =
    IsFloatingPoint<T> || IsInteger<T> || IsPointer<T> ||
    /* ---> This shit right here is false for primitive types. why? ---> */ __is_trivially_assignable(T, U);

template<typename T>
concept TriviallyDestructible = __is_trivially_destructible(T);

template<typename T>
concept Destructible = __is_destructible(T);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      FUNCTION CALLING TYPE TRAITS
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


template<typename Func, typename... Args>
concept IsCallableWith = requires (Func func, Args... args) {
    {
        func(args...)
    };
};

template<typename F, typename ReturnType, typename... Args>
concept IsCallableAndReturns = requires (F f, Args... args) {
    {
        f(args...)
    } -> IsSame<ReturnType>;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      OPERATOR CHECKING TYPE TRAITS
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


template<typename T>
concept IsIncrementable = requires (T a) {
    {
        (++a, a++)
    };
};

template<typename T>
concept IsDecrementable = requires (T a) {
    {
        (a--, --a)
    };
};

template<typename T>
concept IsIncrementableDecrementable = IsIncrementable<T> && IsDecrementable<T>;


template<typename T1, typename T2>
concept IsComparableLT = requires (T1 t1, T2 t2) {
    {
        t1 < t2
    };
};

template<typename T1, typename T2>
concept IsComparableGT = requires (T1 t1, T2 t2) {
    {
        t1 > t2
    };
};

template<typename T1, typename T2>
concept IsComparableLE = requires (T1 t1, T2 t2) {
    {
        t1 <= t2
    };
};

template<typename T1, typename T2>
concept IsComparableGE = requires (T1 t1, T2 t2) {
    {
        t1 >= t2
    };
};

template<typename T1, typename T2>
concept IsComparable =
    IsComparableLT<T1, T2> && IsComparableGT<T1, T2> && IsComparableLE<T1, T2> && IsComparableGE<T1, T2>;

template<typename T1, typename T2 = T1>
concept IsEquatable = requires (T1 t1, T2 t2) {
    {
        t1 == t2
    };
};

template<typename T1, typename T2>
concept IsDivideable = requires (T1 t1, T2 t2) {
    {
        t1 / t2
    };
};

template<typename T1, typename T2>
concept IsMultipliable = requires (T1 t1, T2 t2) {
    {
        t1* t2
    };
};

template<typename T1, typename T2>
concept IsAddable = requires (T1 t1, T2 t2) {
    {
        t1 + t2
    };
};

template<typename T1, typename T2>
concept IsSubtractable = requires (T1 t1, T2 t2) {
    {
        t1 - t2
    };
};

template<typename T, typename IndexType>
concept IsIndexable = requires (T t, IndexType i) {
    {
        t[i]
    };
};

template<typename T>
concept HasOperatorAdd = requires {
    {
        &T::operator+
    };
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      OTHER STUFF
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
concept IsPrimitiveData = bool{
    TriviallyDestructible<T> && TriviallyCopyConstructible<T> && TriviallyDefaultConstructible<T> &&
    TriviallyAssignable<T>};

static_assert(TriviallyDestructible<char>);
static_assert(TriviallyAssignable<char>);

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
}  // namespace detail

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


/**
 * @brief TODO
 */
class NonCopyable {
protected:
    NonCopyable() = default;
    ~NonCopyable() = default;

public:
    NonCopyable(NonCopyable const&) = delete;
    NonCopyable& operator=(NonCopyable const&) = delete;

    NonCopyable(NonCopyable&&) noexcept(true) = default;
    NonCopyable& operator=(NonCopyable&&) noexcept(true) = default;
};

template<typename T, unsigned ID>
class Wrapper {
    T val_;

public:
    // NOLINTNEXTLINE
    Wrapper(T val)
        : val_(val)
    {}

    // NOLINTNEXTLINE
    operator T() const noexcept { return val_; }
};


///
/// Four-function arithmetic interface.
/// Derived must provide: add, sub, mul, div
///
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
    {
        a + b
    } -> ConvertibleTo<T>;
    {
        a += b
    };
    {
        a - b
    } -> ConvertibleTo<T>;
    {
        a -= b
    };
    {
        a* b
    } -> ConvertibleTo<T>;
    {
        a *= b
    };
    {
        a / b
    } -> ConvertibleTo<T>;
    {
        a /= b
    };
};

template<typename T>
concept IsIterator = requires (T a) {
    {
        ++a
    };
    {
        *a
    };
    {
        a == a
    } -> IsBool;
    {
        a != a
    } -> IsBool;
};


template<typename T>
concept IsIterable = requires (T a) {
    {
        a.begin()
    } -> IsIterator;
    {
        a.end()
    } -> IsIterator;
};

// namespace {

// template<typename T, T... Index>
// struct DummyIntSeq {};

// template<typename T, auto N>
// using MakeDummyIntSeq = __make_integer_seq<DummyIntSeq, T, N>;

// template<unsigned I, typename Item>
// struct DummyTupleLeaf {
//     Item value;
// };

// template<unsigned I, typename... Items>
// struct DummyTupleImpl;

// template<unsigned I>
// struct DummyTupleImpl<I> {};

// // Recursive specialization
// template<unsigned I, typename H, typename... T>
// struct DummyTupleImpl<I, H, T...> : public DummyTupleLeaf<I, H>, public DummyTupleImpl<I + 1, T...> {
//     using Next = DummyTupleImpl<I + 1, T...>;
// };

// template<typename... Args>
// struct DummyTuple : DummyTupleImpl<0, Args...> {
//     template<unsigned I>
//     requires (I < sizeof...(Args))
//     constexpr auto get() const noexcept {
//         return get_<0, I, Args...>();
//     }

// template<typename T>
// constexpr T construct() const noexcept {
//     return construct_<T>(MakeDummyIntSeq<unsigned, sizeof...(Args)>{});
// }

// private:
//     template<typename T, unsigned... Indices>
//     constexpr inline T construct_(DummyIntSeq<unsigned, Indices...>) const noexcept {
//         return T(get<Indices>()...);
//     }

// template<unsigned I, unsigned N, typename H_, typename... T_>
// constexpr auto get_() const noexcept {
//     if constexpr (I != N) {
//         return get_<I + 1, N, T_...>();
//     }
//     else {
//         return static_cast<DummyTupleImpl<I, H_, T_...> const&>(*this).DummyTupleLeaf<I, H_>::value;
//     }
// }
// };

// template<class F, int = (F{}(), 0)>
// constexpr bool isExpressionConstexpr(F) {
//     return true;
// }

// constexpr bool isExpressionConstexpr(...) {
//     return false;
// }

// }  // namespace

// // clang-format off
//  template<typename T, typename... Args>
// concept ConstexprConstructible = __is_constructible(T, Args...) && //
// isExpressionConstexpr([] { //
//     struct Dummy {
//         DummyTuple<Args...> v;

// consteval T x() {
//     if constexpr (sizeof...(Args) == 0) {
//         return T{};
//     }
//     else {
//         return v.template construct<T>();
//     }
// }
// };
// Dummy{}.x();
// });
// // clang-format on

// template<typename T, typename... Args>
// concept NotConstexprConstructible = !ConstexprConstructible<T, Args...>;

// namespace test {

// struct TestA {
//     constexpr TestA() = default;

// constexpr TestA(int x, int y) {}
// };

// struct TestB {
//     TestB() { p = static_cast<char*>(__builtin_malloc(5)); }

// explicit TestB(int z) { p = static_cast<char*>(__builtin_malloc(z)); }

// char* p;
// };


// }  // namespace test
#endif
