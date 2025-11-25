set(ATLAS_PLATFORM "Linux")
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

set(CMAKE_SYSTEM_NAME "Linux")
set(CMAKE_MSVC_RUNTIME_LIBRARY "")
set(CMAKE_EXECUTABLE_SUFFIX "")
set(CMAKE_EXECUTABLE_SUFFIX_CXX "")
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR x86_64)
set(CMAKE_CXX_COMPILER_TARGET "x86_64-redhat-linux-gnu")
set(CMAKE_CXX_FLAGS_DEBUG "-O3 -glldb")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os")
set(CMAKE_CXX_FLAGS_RELEASE "-Os")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Os -glldb")
set(CMAKE_CXX_COMPILE_FEATURES "")
set(ATLAS_EMBEDDED FALSE)



add_compile_options(
    --target=x86_64-redhat-linux-gnu
    -fcolor-diagnostics
    -fansi-escape-codes
    -mthread-model posix
    -Wall
    #-nodefaultlibs
    #-nostdlib
    -fno-exceptions
    -fno-rtti
    -fno-threadsafe-statics
    # optimizations
    -mavx2
    -mfma
    #-fpointer-tbaa
    -fvectorize
    -fwrapv
    #-fwrapv-pointer
    #
    -fverbose-asm
    -fstack-clash-protection
    
    #-fsanitize=dataflow
    #-mllvm=-dfsan-track-origins=0
    # -mllvm=-dfsan-combine-pointer-labels-on-load=false
    # -mllvm=-dfsan-combine-pointer-labels-on-store=false
    # -mllvm=-dfsan-combine-offset-labels-on-gep=true
    # -mllvm=-dfsan-track-select-control-flow=false
    # -finstrument-functions-after-inlining

    -fsanitize=address
    #-fno-inline
    -fno-omit-frame-pointer
    #-fsanitize=undefined -fsanitize-minimal-runtime
    #-fno-sanitize=vptr
)

add_compile_options(
    #-flto=full
    #-fvirtual-function-elimination
    #-fwhole-program-vtables
)

add_link_options(
    #-flto=full
    -fsanitize=address
    #--color-diagnostics=always

    #-dfsan-abilist=${CMAKE_CURRENT_SOURCE_DIR}/toolchain/dfsan-ignorelist.txt
#-fsanitize=undefined -fsanitize-minimal-runtime
    #-nostdlib
    #-nostdlib++
)

#link_directories(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/SFML/lib)

set(WS ${CMAKE_CURRENT_LIST_DIR}/../src)
set(PLATFORM_SRC 
    ${WS}/system/linux/linux_runtime.cc
    ${WS}/system/linux/linux_streams.cc 
    ${WS}/system/linux/linux_shell.cc 
)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER)