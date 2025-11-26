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

#include "core/base_definitions.hh"     // IWYU pragma: keep
#include "core/reflection_type.hh"      // IWYU pragma: keep
#include "core/initializer_list.hh"     // IWYU pragma: keep
#include "core/tuple.hh"                // IWYU pragma: keep
#include "core/reflection_function.hh"  // IWYU pragma: keep
#include "core/function.hh"             // IWYU pragma: keep
#include "core/intbase.hh"              // IWYU pragma: keep
#include "core/result.hh"               // IWYU pragma: keep
#include "core/optional.hh"             // IWYU pragma: keep
#include "core/errors.hh"               // IWYU pragma: keep
#include "core/assert.hh"               // IWYU pragma: keep
#include "core/profiler.hh"             // IWYU pragma: keep
#include "core/union.hh"                // IWYU pragma: keep
#include "core/defer.hh"                // IWYU pragma: keep

#include "core/arrayref.hh"    // IWYU pragma: keep
#include "core/math_int.hh"    // IWYU pragma: keep
#include "core/math_float.hh"  // IWYU pragma: keep
#include "core/box.hh"         // IWYU pragma: keep

#include "core/property.hh"  // IWYU pragma: keep
#include "core/rng.hh"       // IWYU pragma: keep
#include "core/pointer.hh"   // IWYU pragma: keep

#include "core/hash.hh"            // IWYU pragma: keep
#include "core/data_integrity.hh"  // IWYU pragma: keep
#include "core/class.hh"           // IWYU pragma: keep

namespace cm {}  // namespace cm

#undef __inline_core_header__
