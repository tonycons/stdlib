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
#define __inline_sys_header__

#include "core.hh"                // IWYU pragma: keep
#include "datastructs/string.hh"  // IWYU pragma: keep
#include "system/Stream.hh"       // IWYU pragma: keep


// TODO don't use these
extern "C" int putchar(int);
extern "C" int puts(char const*);

namespace cm::io {


void _emergencyPrint(char const* str);

}  // namespace cm::io

#undef __inline_sys_header__
