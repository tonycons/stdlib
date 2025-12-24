cmake_minimum_required(VERSION 3.28)
project(commons C CXX)

cmake_policy(SET CMP0048 NEW)
cmake_policy(SET CMP0072 NEW)
cmake_policy(SET CMP0076 NEW)
cmake_policy(SET CMP0087 NEW)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_OBJECTS ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(WS ${CMAKE_CURRENT_SOURCE_DIR})

##############################################################################################
# Warning flags

add_executable(commons-test)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(commons-test PUBLIC
        -Weverything
        -fsafe-buffer-usage-suggestions
        -Wnonnull
        -Wno-global-constructors
        -Wno-missing-prototypes
        -Wno-extra-semi
        -Wno-extra-semi-stmt
        -Wno-c++20-compat
        -Wno-c++98-compat
        -Wno-c++98-compat-pedantic
        -Wno-c99-designator
        -Wno-unused-macros
        -Wno-gnu-case-range
        -Wno-shadow-field-in-constructor
        -Wno-dollar-in-identifier-extension
        -Wno-padded
        -Wno-weak-vtables
        -Wno-exit-time-destructors
        -Wno-reserved-identifier
        -Wno-extra-qualification
        -Wno-bit-int-extension
        -Wno-header-hygiene
    )
elseif (CMAKECXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(commons-test PUBLIC 
        /W4
    )
endif()

target_include_directories(commons-test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_sources(commons-test PUBLIC ${WS}/test/main.cc)
