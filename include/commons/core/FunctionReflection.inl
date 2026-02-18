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
/// Provides static reflection on functions.
///
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

}  // namespace cm

#endif
