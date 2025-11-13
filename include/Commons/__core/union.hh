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

namespace cm {

/**
 * Basically a tagged union similar to that of Rust Enum.
 * It follows the exact same memory layout to the Rust Enums-
 * Essentially "struct { u8 tag; union { .... } u; };"
 * Except while Rust uses a 16-bit integer as a tag, this can use a byte as the tag if there are less than 255 types
 * See https://patshaughnessy.net/2018/3/15/how-rust-implements-tagged-unions
 */
template<typename... Types>
class Union {
public:
    template<typename T, int I>
    struct InitSuccess
    {
        using WhichOne = T;
        constexpr static auto Tag = I;
    };
    struct InitFailure
    {};


private:
    UintRanged<sizeof...(Types)> _tag;  // the Nth type stored in the union
    alignas(max(alignof(Types)...)) u8 _data[max(sizeof(Types)...)];


    struct Strong
    {
        template<typename From, typename To>
        constexpr static bool match()
        {
            return IsSame<From, To>
                   // Two floating point numbers, where sizeof(To) >= sizeof(From)
                   || (IsFloatingPoint<From> && IsFloatingPoint<To> && sizeof(To) >= sizeof(From))
                   // Two Integers of the same sign, where sizeof(To) >= sizeof(From)
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
        constexpr static auto match(void* data, From const& value)
        {
            if constexpr (matchFn::template match<From, T1>()) {
                new (data) T1(value);
                return InitSuccess<T1, I>{};
            } else {
                return Initializer<matchFn, I + 1, From, Tn...>::match(data, value);
            }
        }
    };

    template<typename matchFn, int I, typename From, typename To>
    struct Initializer<matchFn, I, From, To>
    {
        constexpr static auto match(void* data, From const& value)
        {
            if constexpr (matchFn::template match<From, To>()) {
                new (data) To(value);
                return InitSuccess<To, I>{};
            } else {
                return InitFailure{};
            }
        }
    };

    template<typename matchFn, int I, typename U>
    struct Initializer<matchFn, I, U>
    {
        constexpr static auto match(void*, U const&) { return InitFailure{}; }
    };

    /**
     * Attempts to initialize the union value of type T1 with a given value. If the union value of type T1 cannot be
     * initialized by the given value, then attempts to initialize the union value with the next type in Tn.
     */
    template<int I, typename U, typename... Tn>
    struct TryInit
    {
        constexpr static auto next(void* data, U const& value)
        {
            // attemps to initialize the union with the first type that is a strong match to value.
            auto r1 = Initializer<Strong, I, U, Tn...>::match(data, value);

            if constexpr (IsSame<decltype(r1), InitFailure>) {
                // otherwise, the first type that is a medium match to value.
                auto r2 = Initializer<Medium, I, U, Tn...>::match(data, value);
                if constexpr (IsSame<decltype(r2), InitFailure>) {
                    // otherwise, the first type that is a weak match to value.
                    auto r3 = Initializer<Weak, I, U, Tn...>::match(data, value);
                    return r3;
                } else {
                    return r2;
                }
            } else {
                return r1;
            }
        }
    };


public:
    template<typename GivenType>
    constexpr Union(GivenType const& t1)
        // "None of the union types can be initialized by the given type."
        requires (!IsSame<decltype(TryInit<0, GivenType, Types...>::next(_data, t1)), InitFailure>)
    {
        using Init = decltype(TryInit<0, GivenType, Types...>::next(_data, t1));
        TryInit<0, GivenType, Types...>::next(_data, t1);
        _tag = static_cast<unsigned char>(Init::Tag);
    }

    template<typename GivenType>
    constexpr Union& operator=(GivenType const& t1)
    {
        new (this) Union(t1);
        return *this;
    }

    template<typename T>
    constexpr bool is() const noexcept
    {
        using Init = decltype(TryInit<0, T, Types...>::next(const_cast<u8*>(_data), _ref<T>()));
        if constexpr (IsSame<Init, InitFailure>) {
            return false;
        } else {
            return _tag == Init::Tag;
        }
    }

    template<typename T>
    NODISCARD FORCEINLINE auto const& _ref() const noexcept
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
    NODISCARD FORCEINLINE auto get() const noexcept
    {
        Assert(is<T>());
        return const_cast<ConstRemoved<decltype(_ref<T>())>>(_ref<T>());
    }

    NODISCARD FORCEINLINE auto tag() const noexcept { return _tag; }

    /**
     *
     * For each provided closure, the closure that will be called is the one where the type of its first argument
     * matches the type stored in the union
     */
    FORCEINLINE auto match(auto func, auto... funcs) const
    {
        using T = FunctionTraits<decltype(func)>::template Arg<0>::Type;
        if (this->is<T>() || this->is<RefRemoved<T>>()) {
            return func(this->get<T>());
        } else {
            return this->match(funcs...);
        }
    }

    FORCEINLINE auto match(auto func) const
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

}  // namespace cm
#endif
