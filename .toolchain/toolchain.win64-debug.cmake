set(ATLAS_PLATFORM "Windows")
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)

set(CMAKE_C_FLAGS "")
set(CMAKE_CXX_FLAGS "")
set(CMAKE_SHARED_LINKER_FLAGS "")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "")
set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "")
set(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO "")
set(CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL"")
set(CMAKE_EXE_LINKER_FLAGS_INIT "")

set(CMAKE_SYSTEM_NAME "Windows")
set(CMAKE_MSVC_RUNTIME_LIBRARY "")
set(CMAKE_EXECUTABLE_SUFFIX ".exe")
set(CMAKE_EXECUTABLE_SUFFIX_CXX ".exe")
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR x86_64)
set(CMAKE_CXX_COMPILER_TARGET "x86_64-pc-windows-none")
set(CMAKE_CXX_FLAGS_RELEASE "-Os")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Os -g")
set(CMAKE_CXX_COMPILE_FEATURES "")
set(ATLAS_EMBEDDED FALSE)
 
add_compile_options(
    --target=x86_64-pc-windows-none
    -mthread-model posix
    -Wall
    -fno-rtti
    -fno-exceptions
    -fno-threadsafe-statics
    -mavx2
    -mfma
    -Wno-inconsistent-dllimport
    -fuse-ld=clang
    -ffreestanding
    -Wl,--entry-point=mainCRTStartup
    -fsanitize=address
    -fno-inline # different from win64-rel
)

add_link_options(
    #-lmsvcrt
    #-lclang_rt.asan_dynamic-x86_64
    -lntdll
    -fsanitize=address
    #-lkernel32
    #-luser32
    #-lvcruntime140
)


set(WS ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(PLATFORM_SRC 
    ${WS}/system/windows/WinMain-Freestanding.cc 
    ${WS}/system/windows/WinStandardStreams.cc 
    ${WS}/system/windows/WinHeap.cc
    ${WS}/system/windows/WinChkstk.cc
)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER)