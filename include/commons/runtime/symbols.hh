/*
   Copyright 2025 Anthony A. Constantinescu.

   Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
   in compliance with the License. You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software distributed under the License
   is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
   or implied. See the License for the specific language governing permissions and limitations under
   the License.

   File: runtime/symbols.hh
   Description: Functions that retrieve symbol names from pointers.
*/

#pragma once
#include "commons/core.hh"  // IWYU pragma: keep


#if __linux__
#include <dlfcn.h>
#include <cxxabi.h>
#endif

namespace cm::debug {

///
/// @return The symbol name of the function or static variable at the given address, or None if no symbol is found.
/// @param addr The address
/// @param demangle Whether to assume the name is from C++ and demangle it
///
inline auto getSymbolName(void const* addr, bool demangle = true)
    -> Optional<FixedString<LibraryConfig::MAX_SYMBOL_LENGTH>>
{
    Dl_info info{};

    if (dladdr(addr, &info) != 0) {
        char const* sname = "?";
        int status = -1;

        if (info.dli_sname) {
            char* demangled = abi::__cxa_demangle(info.dli_sname, nullptr, nullptr, &status);
            if (demangle && status == 0) {
                sname = demangled;
            } else {
                sname = info.dli_sname;
            }
        }
        DEFER
        {
            if (status == 0) {
                __builtin_free(const_cast<char*>(sname));
            }
        };
        return FixedString<LibraryConfig::MAX_SYMBOL_LENGTH>(sname);
    }
    return None;
}
}  // namespace cm::debug
