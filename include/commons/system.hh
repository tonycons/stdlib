/*
   Copyright 2025 Anthony A. Constantinescu.

   Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
   in compliance with the License. You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software distributed under the License
   is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
   or implied. See the License for the specific language governing permissions and limitations under
   the License.

   File: commons/system.hh
   Description: Exports all of the OS-related functionality.
*/


#pragma once

#include "core.hh"                // IWYU pragma: keep
#include "datastructs/array.hh"   // IWYU pragma: keep
#include "datastructs/string.hh"  // IWYU pragma: keep

#define __inline_sys_header__
#include "system/streamstatus.inl"   // IWYU pragma: keep
#include "system/outstream.inl"      // IWYU pragma: keep
#include "system/stringstream.inl"   // IWYU pragma: keep
#include "system/fileoutstream.inl"  // IWYU pragma: keep
#include "system/listdir.inl"        // IWYU pragma: keep
#include "system/shell.inl"          // IWYU pragma: keep

#ifdef __linux__
#include "system/linux/linuxapi.inl"  // IWYU pragma: keep

namespace cm {
#include "system/linux/linuxsyscall.inl"  // IWYU pragma: keep
#include "system/linux/linuxstdout.inl"   // IWYU pragma: keep
#include "system/linux/linuxfileout.inl"  // IWYU pragma: keep
#include "system/linux/linuxshell.inl"    // IWYU pragma: keep
#include "system/linux/linuxruntime.inl"  // IWYU pragma: keep
#else
namespace cm {

// For an unknown operating system, set all the stdout and shell objects to None.

///
/// An Optional standard output stream.
/// It may be set to None on systems that don't have a standard output stream.
///
inline Optional<OutStream&> const stdout = None;

///
/// An Optional standard error stream.
/// It may be set to None on systems that don't have a standard error stream.
///
inline Optional<OutStream&> const stderr = None;

///
/// An Optional system shell e.g. Bash.
/// It may be set to None on systems that don't have a shell.
///
inline Optional<Shell&> const shell = None;

void _emergencyPrint(char const* str) {}

}  // namespace cm
#endif

}  // namespace cm

#undef __inline_sys_header__
