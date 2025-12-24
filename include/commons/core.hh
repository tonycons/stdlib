/*
   Copyright 2025 Anthony A. Constantinescu.

   Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
   in compliance with the License. You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software distributed under the License
   is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
   or implied. See the License for the specific language governing permissions and limitations under
   the License.

   File: commons/core.hh
   Description: Exports the core features of this library
*/

#pragma once
#define __inline_core_header__

// clang-format off

#ifndef HEADER
#define __module_path_str_helper(x) #x
#define __module_path_str(x) __module_path_str_helper(x)
#define __base_path0 commons
#define HEADER(file) __module_path_str(__base_path0/file)
#endif

#include HEADER(core/base_definitions.hh)     // IWYU pragma: keep
#include HEADER(resources/OneCharStringTable.hh) // IWYU pragma: keep
#include HEADER(core/cstring.hh)              // IWYU pragma: keep
#include HEADER(core/range.hh)                // IWYU pragma: keep
#include HEADER(core/reflection_type.hh)      // IWYU pragma: keep
#include HEADER(core/comparable.hh)           // IWYU pragma: keep
#include HEADER(core/generator.hh)            // IWYU pragma: keep
#include HEADER(core/initializer_list.hh)     // IWYU pragma: keep
#include HEADER(core/tuple.hh)                // IWYU pragma: keep
#include HEADER(core/reflection_function.hh)  // IWYU pragma: keep
#include HEADER(core/function.hh)             // IWYU pragma: keep
#include HEADER(core/intbase.hh)              // IWYU pragma: keep
#include HEADER(core/assert.hh)               // IWYU pragma: keep
#include HEADER(core/union.hh)                // IWYU pragma: keep
#include HEADER(core/result.hh)               // IWYU pragma: keep
#include HEADER(core/optional.hh)             // IWYU pragma: keep
#include HEADER(core/errors.hh)               // IWYU pragma: keep
#include HEADER(core/profiler.hh)             // IWYU pragma: keep

#include HEADER(core/array_iterator.hh)       // IWYU pragma: keep
#include HEADER(core/index.hh)                // IWYU pragma: keep
#include HEADER(core/iterable.hh)             // IWYU pragma: keep
#include HEADER(core/arrayref.hh)             // IWYU pragma: keep
#include HEADER(core/string_ref.hh)           // IWYU pragma: keep
#include HEADER(core/math_int.hh)             // IWYU pragma: keep
#include HEADER(core/math_float.hh)           // IWYU pragma: keep
#include HEADER(core/math_double.hh)           // IWYU pragma: keep
#include HEADER(core/pointer.hh)              // IWYU pragma: keep
#include HEADER(core/property.hh)             // IWYU pragma: keep
#include HEADER(core/rng.hh)                  // IWYU pragma: keep
#include HEADER(core/hash.hh)                 // IWYU pragma: keep
#include HEADER(core/class.hh)                // IWYU pragma: keep

#include HEADER(core/search.hh)               // IWYU pragma: keep
#include HEADER(core/predicates.hh)           // IWYU pragma: keep


// clang-format on

namespace cm {}  // namespace cm

#undef __inline_core_header__
