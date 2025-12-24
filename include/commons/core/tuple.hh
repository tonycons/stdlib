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

#if __has_builtin(__make_integer_seq)

template<typename T, T... Index>
struct IntegerSequence
{
    constexpr static auto size() { return sizeof...(Index); }
};

template<typename T, auto N>
using MakeIntegerSequence = __make_integer_seq<IntegerSequence, T, N>;

#else

template<typename T, T... Ints>
struct IntegerSequence
{
    constexpr static auto size() { return sizeof...(Ints); }

    template<T Next>
    using Append = IntegerSequence<T, Ints..., Next>;
};

namespace detail {
template<typename T, int N>
struct MakeIntegerSequence
{
    using Type = typename MakeIntegerSequence<T, N - 1>::Type::template Append<N>;
};

template<typename T>
struct MakeIntegerSequence<T, -1>
{
    using Type = IntegerSequence<T>;
};
}  // namespace detail

template<typename T, int N>
using MakeIntegerSequence = typename detail::MakeIntegerSequence<T, N - 1>::Type;

#endif  // __has_builtin(__make_integer_seq)


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

// Recursive specialization
template<unsigned I, typename H, typename... T>
struct TupleImpl<I, H, T...> : public TupleLeaf<I, H>, public TupleImpl<I + 1, T...>
{
    // using Next = TupleImpl<I + 1, T...>;
protected:
    constexpr TupleImpl(H h, T... t)
        : TupleLeaf<I, H>{h}, TupleImpl<I + 1, T...>{t...}
    {}
};

///
/// Tuple
///
template<typename... Args>
struct Tuple : TupleImpl<0, Args...>
{

    ///
    /// Initialize tuple with values for elements.
    ///
    constexpr Tuple(auto&&... args)
        : TupleImpl<0, Args...>{Forward<decltype(args)>(args)...}
    {}

    ///
    /// TODO: Disable if elements aren't copy constructible
    ///
    constexpr Tuple(Tuple const&) = default;

    ///
    /// Number of elements in the tuple.
    ///
    consteval auto size() const noexcept { return sizeof...(Args); }

    ///
    /// Return the value of an element in the tuple.
    ///
    template<unsigned I>
    requires (I < sizeof...(Args))
    constexpr auto const& get() const noexcept
    {
        return get_<0, I, Args...>();
    }

    ///
    /// Initialize an instance of T with the elements stored in the tuple.
    ///
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

template<__SIZE_TYPE__, typename>
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
}  // namespace std


#endif
