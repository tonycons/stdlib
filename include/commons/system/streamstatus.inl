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

#include "linux/linuxapi.inl"

namespace cm {

struct StreamStatus : IEquatable<StreamStatus>
{
    int code = 0;
    StringRef codeName = "";  // NOLINT
    StringRef extMessage = "";

    constexpr StreamStatus(StreamStatus const&) = default;
    constexpr StreamStatus(StreamStatus&&) = default;
    constexpr StreamStatus& operator=(StreamStatus const&) = default;
    constexpr StreamStatus& operator=(StreamStatus&&) = default;
    constexpr explicit StreamStatus(int const code)
        : code(code), codeName(linuxErrCodes[code].valueOr("")), extMessage([&] {
              auto const cstr = strerror(code);
              auto const len = strlen(cstr);
              return StringRef(cstr, len);
          }())
    {}
    constexpr StreamStatus(int const code, StringRef codeName, StringRef extMessage)
        : code(code), codeName(move(codeName)), extMessage(move(extMessage))
    {}

    [[nodiscard]]
    constexpr bool equals(StreamStatus const& other) const
    {
        return other.code == code;
    }
};

constexpr inline auto STATUS_OK = StreamStatus(0, "Ok", "Ok");


}  // namespace cm

#endif
