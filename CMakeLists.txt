cmake_minimum_required(VERSION 3.28)
project(Commons C CXX)

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

add_library(Commons)

##############################################################################################
# Warning flags

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(Commons PUBLIC
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
    )
elseif (CMAKECXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(Commons PUBLIC 
        /W4
    )
endif()

###############################################################################################
# Include directories

target_include_directories(Commons PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

###############################################################################################
# Sources

target_sources(Commons PUBLIC
    ${PLATFORM_SRC}
    ${WS}/src/core/core_assert.cc
    ${WS}/src/core/core_cstring.cc
    ${WS}/src/core/core_memory.cc
    ${WS}/src/core/core_newdelete.cc
    ${WS}/src/core/core_profiler.cc
    ${WS}/src/core/core_random.cc
    ${WS}/src/core/core_stream.cc

    ${WS}/src/algorithm/parser_alphabet.cc
    ${WS}/src/algorithm/parser_tokenizer.cc


    ${WS}/src/datastructs/ds_array.cc
    ${WS}/src/datastructs/ds_bytevector.cc
    ${WS}/src/datastructs/ds_llist.cc
    #${WS}/src/datastructs/ds_sparsearray.cc
    ${WS}/src/datastructs/ds_string.cc

    
    # ${WS}/src/dp/variant.cc
    # ${WS}/src/gc/PointerTracking.cc
    # ${WS}/src/gc/GC.cc
    # ${WS}/src/gc/GCAllocation.cc
)

if(NOT DEFINED DONT_BUILD_Commons_TEST)
    add_executable(Commons-test)
    target_include_directories(Commons-test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(Commons-test Commons)
    target_sources(Commons-test PUBLIC ${WS}/test/main.cc ${WS}/test/test_tokenizer.cc)
endif()
