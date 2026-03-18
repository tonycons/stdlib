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
#define __inline_core_header__  // NOLINT

#include "core/BaseDefs.inl"                // IWYU pragma: keep
#include "resources/OneCharStringTable.hh"  // IWYU pragma: keep
#include "core/CString.inl"                 // IWYU pragma: keep
#include "core/Range.inl"                   // IWYU pragma: keep
#include "core/TypeReflection.inl"          // IWYU pragma: keep
#include "core/Comparable.inl"              // IWYU pragma: keep
#include "core/Generator.inl"               // IWYU pragma: keep
#include "core/initializer_list.hh"         // IWYU pragma: keep
#include "core/Tuple.inl"                   // IWYU pragma: keep
#include "core/FunctionReflection.inl"      // IWYU pragma: keep
#include "core/Function.inl"                // IWYU pragma: keep
#include "core/IntDefs.inl"                 // IWYU pragma: keep
#include "core/Atomic.inl"                  // IWYU pragma: keep
#include "core/Assert.inl"                  // IWYU pragma: keep
#include "core/ComputedProperty.inl"        // IWYU pragma: keep
#include "core/Pointers.inl"                // IWYU pragma: keep
#include "core/Class.inl"                   // IWYU pragma: keep
#include "core/Union.inl"                   // IWYU pragma: keep
#include "core/result.hh"                   // IWYU pragma: keep
#include "core/Optional.inl"                // IWYU pragma: keep
#include "core/DelegateUnion.inl"           // IWYU pragma: keep
#include "core/Errors.inl"                  // IWYU pragma: keep
#include "core/Profiler.inl"                // IWYU pragma: keep
#include "core/ArrayIterator.inl"           // IWYU pragma: keep
#include "core/Index.inl"                   // IWYU pragma: keep
#include "core/Iterable.hh"                 // IWYU pragma: keep
#include "core/Array.inl"                   // IWYU pragma: keep
#include "core/IntMath.inl"                 // IWYU pragma: keep
#include "core/FloatMath.inl"               // IWYU pragma: keep
#include "core/DoubleMath.inl"              // IWYU pragma: keep
#include "core/StringRef.inl"               // IWYU pragma: keep
#include "core/Random.inl"                  // IWYU pragma: keep
#include "core/Hashing.inl"                 // IWYU pragma: keep
#include "core/Search.inl"                  // IWYU pragma: keep
#include "core/Predicates.inl"              // IWYU pragma: keep
#include "core/Allocator.hh"                // IWYU pragma: keep

namespace cm {}  // namespace cm

#undef __inline_core_header__
