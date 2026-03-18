/*
   Copyright 2025 Anthony A. Constantinescu.

   Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
   in compliance with the License. You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software distributed under the License
   is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
   or implied. See the License for the specific language governing permissions and limitations under
   the License.

   File: commons/datastructs.hh
   Description: Exports all of the data structures
*/

#pragma once

#include "core.hh"

#define __inline_core_header__  // NOLINT

#include "datastructs/Collection.inl"  // IWYU pragma: keep
#include "datastructs/FixedStack.inl"  // IWYU pragma: keep
#include "datastructs/ByteVector.inl"  // IWYU pragma: keep
#include "datastructs/String.inl"      // IWYU pragma: keep
#include "datastructs/LinkedList.inl"  // IWYU pragma: keep
#include "datastructs/FixedMap.inl"    // IWYU pragma: keep

#undef __inline_core_header__

// clang-format on
