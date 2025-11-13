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


template<typename>
class CFunction;


/**
 * @brief A wrapper for declaring C-style function pointer types.
 * Unlike Function, it can't keep a pointer to a lambda function that has captures.
 */
template<typename ReturnType_, typename... Args>
class CFunction<ReturnType_(Args...)> {
public:
    using ReturnType = ReturnType_;
    using PtrType = ReturnType (*)(Args...);

    constexpr CFunction()
        : _func(nullptr)
    {}

    constexpr CFunction(ReturnType (*funcPtr)(Args...))
        : _func(funcPtr)
    {}

    constexpr CFunction& operator=(ReturnType (*funcPtr)(Args...))
    {
        _func = funcPtr;
        return *this;
    }

    constexpr ReturnType operator()(Args... args) const { return _func(args...); }

    constexpr operator bool() const { return _func != nullptr; }

    constexpr operator PtrType() const { return _func; }

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
    struct CallableT : public Callable
    {
    private:
        T t_;

    public:
        CallableT(T const& t)
            : t_(t)
        {}

        ~CallableT() override = default;

        ReturnType invoke(Args... args) override { return t_(args...); }
    };

    Callable& callable_;

public:
    template<typename T>
    Function(T t) requires (IsCallableAndReturns<T, ReturnType, Args...>)
        : callable_(*(new CallableT<T>(t)))
    {}

    Function(Function<ReturnType(Args...)> const&) = delete;
    Function operator=(Function<ReturnType(Args...) const&>) = delete;

    ~Function() { delete &callable_; }

    // template<typename T>
    // Function& operator=(T t) {
    //     callable_ = std::make_unique<CallableT<T>>(t);
    //     return *this;
    // }

    ReturnType operator()(Args... args) const { return callable_.invoke(args...); }
};


///
/// Utilities for higher order functions
///
struct Functions
{

    ///
    /// Returns a pointer to a function that does nothing but return the value x
    ///
    // constexpr static auto identity(auto const& x)
    // {
    //     return [&x](...) {
    //         return x;
    //     };
    // }


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
};

}  // namespace cm
#endif
