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

///
/// Some dedicated error types in development, for use as the error values of Results or as exception values.
///
namespace Errors {
struct Overflow
{};
struct BadFormat
{};
};  // namespace Errors

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
#define throws(...)

///
/// For constexpr functions that might need to throw, this can be used instead of throw, and if the function is
/// constant-evaluated, it will simply stop the compiler.
/// Try and catch cannot work in constexpr, so this is merely a hack to allow simple functions that throw to be declared
/// constexpr.
///
#define constexpr_throw(E)                                                                                             \
    do {                                                                                                               \
        if consteval {                                                                                                 \
            while (bool(#E))                                                                                           \
                ;                                                                                                      \
        } else {                                                                                                       \
            throw E;                                                                                                   \
        }                                                                                                              \
    } while (0)

}  // namespace cm
#endif
