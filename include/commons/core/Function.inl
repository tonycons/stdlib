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
#ifndef __inline_core_header__
#warning Do not include this file directly; include "core.hh" instead
#else

namespace cm {

///
/// A base interface for a callable function object.
///
template<typename>
struct IFunction;
template<typename ReturnType_, typename... Args>
struct IFunction<ReturnType_(Args...)>
{
protected:
    constexpr IFunction() = default;
    constexpr IFunction(IFunction const&) = default;
    constexpr IFunction& operator=(IFunction const&) = default;
    constexpr IFunction(IFunction&&) = default;
    constexpr IFunction& operator=(IFunction&&) = default;

public:
    using ReturnType = ReturnType_;
    virtual constexpr ~IFunction() = default;
    virtual constexpr ReturnType operator()(Args&&... args) const = 0;
};

///
/// A wrapper for declaring C-style function pointer types.
/// Unlike Function, it can't keep a pointer to a lambda function that has captures.
///
template<typename>
class CFunction;
template<typename ReturnType_, typename... Args>
class CFunction<ReturnType_(Args...)> : IFunction<ReturnType_(Args...)>, IEquatable<CFunction<ReturnType_(Args...)>> {
public:
    using ReturnType = ReturnType_;
    using PtrType = ReturnType (*)(Args...);

    constexpr CFunction(CFunction const&) = default;
    constexpr CFunction& operator=(CFunction const&) = default;
    constexpr CFunction(CFunction&&) = default;
    constexpr CFunction& operator=(CFunction&&) = default;

    constexpr CFunction() noexcept
        : _func(nullptr)
    {}

    constexpr CFunction(ReturnType (*funcPtr)(Args...)) noexcept  // NOLINT
        : _func(funcPtr)
    {}

    constexpr CFunction& operator=(ReturnType (*funcPtr)(Args...)) noexcept
    {
        _func = funcPtr;
        return *this;
    }

    constexpr bool equals(CFunction const& other) const { return _func == other._func; }
    constexpr bool equals(ReturnType (*funcPtr)(Args...)) const { return _func == funcPtr; }
    constexpr ReturnType operator()(Args&&... args) const override { return _func(Forward<Args>(args)...); }
    constexpr bool hasValue() const { return _func != nullptr; }
    constexpr PtrType toPointer() const { return _func; }

private:
    ReturnType (*_func)(Args...);
};

///
/// An extension of CFunction that can store capturing lambdas.
///
template<usize, typename>
struct Closure;
template<usize Size, typename ReturnType, typename... Args>
struct Closure<Size, ReturnType(Args...)> : IFunction<ReturnType(Args...)>, NonCopyable, NonMovable
{
private:
    struct Callable
    {
        virtual constexpr ~Callable() = default;
        virtual constexpr ReturnType invoke(Args...) = 0;
    };

    template<typename T>
    struct CallableT final : Callable
    {
        T t_;
        constexpr CallableT(T const& t)
            : t_(t)
        {}
        constexpr ~CallableT() override = default;
        constexpr ReturnType invoke(Args... args) override { return t_(args...); }
    };

    // Data storage for the lambda captures
    u8 _data[Size]{};
    // Instance of the callable
    Callable* _callable{};

public:
    template<typename T>
    constexpr explicit Closure(T&& t) requires (IsCallableAndReturns<T, ReturnType, Args...>)
        : _callable(new (_data) CallableT<T>(t))
    {
        static_assert(sizeof(t) <= sizeof(_data));
    }

    constexpr ~Closure() override = default;
    constexpr ReturnType operator()(Args&&... args) const override { return _callable->invoke(Forward<Args>(args)...); }
};


///
/// Utilities for higher order functions
///
struct Functions
{
    ///
    /// An identity function that returns the Nth value in a variable number of arguments passed to it
    ///
    template<long N>
    struct NthIdentityT
    {
    private:
        // Initialize the first row and column
        // If one string is empty, the distance is the length of the other string

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

        ///
        /// A function that takes in a variable number of arguments, and returns the Nth one cast to T.
        /// example:
        ///
        /// constexpr auto x = Cast<int>::FromNth<1>;
        /// constexpr auto i = x(1.0f, 2.0f);
        /// static_assert(i == 2);
        ///
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


}  // namespace cm
#endif
