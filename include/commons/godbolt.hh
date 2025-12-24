
// For use in compiler explorer


#pragma once

#define __module_path_str_helper(x) #x
#define __module_path_str(x) __module_path_str_helper(x)

// clang-format off
#ifdef __GODBOLT__
#define __base_path0 https:/
#define __base_path1 raw.githubusercontent.com/tonycons/stdlib/refs/heads/main/include/commons
#define HEADER(file) __module_path_str(__base_path0/__base_path1/file)
#else
#define __base_path0 commons
#define HEADER(file) __module_path_str(__base_path0/file)
#endif
//clang-format on


#include HEADER(core.hh) // IWYU pragma: keep
#include HEADER(system.hh) // IWYU pragma: keep
#include HEADER(datastructs.hh) // IWYU pragma: keep
#include HEADER(startup.hh) // IWYU pragma: keep
