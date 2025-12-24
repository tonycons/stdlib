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

// clang-format off

#ifndef HEADER
#define __module_path_str_helper(x) #x
#define __module_path_str(x) __module_path_str_helper(x)
#define __base_path0 commons
#define HEADER(file) __module_path_str(__base_path0/file)
#endif

#include HEADER(core.hh)  // IWYU pragma: keep

#define __inline_core_header__

#include HEADER(datastructs/collection.hh) // IWYU pragma: keep
#include HEADER(datastructs/array.hh)         // IWYU pragma: keep
#include HEADER(datastructs/fixed_stack.hh) // IWYU pragma: keep
#include HEADER(datastructs/array_list.hh)    // IWYU pragma: keep
#include HEADER(datastructs/queue.hh)         // IWYU pragma: keep
#include HEADER(datastructs/string.hh)        // IWYU pragma: keep
#include HEADER(datastructs/linked_list.hh)   // IWYU pragma: keep
#include HEADER(datastructs/sparse_array.hh)  // IWYU pragma: keep
#include HEADER(datastructs/map.hh)           // IWYU pragma: keep

#undef __inline_core_header__

// clang-format on
