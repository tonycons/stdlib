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
#ifdef __inline_core_header__


namespace cm {
struct ResultSuccessTag
{};

struct ResultErrorTag
{};

///
/// Result is an error handling class exactly similar in function to Rust result:
/// https://doc.rust-lang.org/std/result/.
/// It can store either an error value of type 'ErrorType' or a success value of type 'SuccessType'
/// but not both. And it can be known whether it stores an ErrorType or SuccessType.
/// This is useful in allowing a function to return different types if it succeeded or failed,
/// such as a 'Resource' on success and an 'error code' or failure.
///
template<typename SuccessType_, typename ErrorType_>
class Result {
public:
    using SuccessType = SuccessType_;
    using ErrorType = ErrorType_;

    template<typename... Args>
    requires (Constructible<SuccessType, Args...>)
    explicit constexpr Result(ResultSuccessTag, Args&&... args)
        : _u{.success = SuccessType(Forward<Args>(args)...)}, _error(false)
    {}

    template<typename... Args>
    requires (Constructible<ErrorType, Args...>)
    explicit constexpr Result(ResultErrorTag, Args&&... args)
        : _u{.error = ErrorType(Forward<Args>(args)...)}, _error(true)
    {}

    constexpr inline ~Result()
    {
        if constexpr (Destructible<ErrorType>) {
            if (_error)
                _u.error.~ErrorType();
        }
        if constexpr (Destructible<SuccessType>) {
            if (!_error)
                _u.success.~SuccessType();
        }
    }

    FORCEINLINE constexpr operator bool() const noexcept { return !_error; }


    FORCEINLINE constexpr bool isOk() const noexcept { return !_error; }
    FORCEINLINE constexpr bool isErr() const noexcept { return _error; }

    FORCEINLINE constexpr SuccessType const& unwrap() const noexcept
    {
        if (_error) {
            CPU.trap();
        }
        return _u.success;
    }

    FORCEINLINE constexpr auto then(auto func)
    {
        if (isOk()) {
            func(_u.success);
        }
    }

    FORCEINLINE constexpr auto then(auto func) const
    {
        if (isOk()) {
            func(_u.success);
        }
    }

private:
    union {
        SuccessType success;
        ErrorType error;
    } _u;
    bool _error;
};

// DO:  template<typename T> instead

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

}  // namespace cm
#endif
