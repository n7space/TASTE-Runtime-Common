cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(TASTE-Runtime-Common VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build/lib)
set(TESTS_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build/tests)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include(MessageColors)
include(LoggingHelper)

add_library(common_build_options INTERFACE)
include(CommonBuildOptions)
include(Sanitizers)

option(TASTE_RUNTIME_COMMON_WARNINGS_AS_ERRORS
       "Treat compiler warnings as errors"
       TRUE)

if(TASTE_RUNTIME_COMMON_WARNINGS_AS_ERRORS)
    log_option_enabled("warnings as errors")
    set(CLANG_WARNINGS -Werror)
endif()

set(CLANG_WARNINGS ${CLANG_WARNINGS}
                   -Wall
                   -Wextra
                   -Wpedantic
                   -Wshadow
                   -Wcast-align
                   -Wunused
                   -Wsign-conversion
                   -Wnull-dereference
                   -Wdouble-promotion
                   -Wformat=2)

set(GCC_WARNINGS ${CLANG_WARNINGS}
                 -Wmisleading-indentation
                 -Wduplicated-cond
                 -Wduplicated-branches
                 -Wlogical-op)

if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    set(PROJECT_WARNINGS ${CLANG_WARNINGS})
elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set(PROJECT_WARNINGS ${GCC_WARNINGS})
endif()

target_compile_options(common_build_options INTERFACE ${PROJECT_WARNINGS})

include(CppCheck)
include(ClangTidy)
include(ClangFormat)
include(Doxygen)

add_subdirectory(src)

option(TASTE_RUNTIME_COMMON_BUILD_TESTS "Build tests" TRUE)
if(TASTE_RUNTIME_COMMON_BUILD_TESTS)
    enable_testing()

    set(STD_C ON CACHE BOOL "cpputest - Use the standard C library")
    set(STD_CPP OFF CACHE BOOL "cpputest - Use the standard C++ library")
    set(CPPUTEST_FLAGS ON CACHE BOOL "cpputest - Use the CFLAGS/CXXFLAGS/LDFLAGS set by CppUTest")
    set(MEMORY_LEAK_DETECTION OFF CACHE BOOL "cpputest - Enable memory leak detection")
    set(EXTENSIONS OFF CACHE BOOL "cpputest - Use the CppUTest extension library")
    set(LONGLONG OFF CACHE BOOL "cpputest - Support long long")
    set(MAP_FILE OFF CACHE BOOL "cpputest - Enable the creation of a map file")
    set(COVERAGE OFF CACHE BOOL "cpputest - Enable running with coverage")
    set(C++11 OFF CACHE BOOL "cpputest - Compile with C++11 support")
    set(WERROR OFF CACHE BOOL "cpputest - Compile with warnings as errors")
    set(TESTS OFF CACHE BOOL "cpputest - Compile and make tests for the code?")
    set(TESTS_DETAILED OFF CACHE BOOL "cpputest - Run each test separately instead of grouped?")
    set(TESTS_BUILD_DISCOVER OFF CACHE BOOL "cpputest - Build time test discover")
    set(EXAMPLES OFF CACHE BOOL "cpputest - Compile and make examples?")
    set(VERBOSE_CONFIG OFF CACHE BOOL "cpputest - Print configuration to stdout during generation")

    add_subdirectory(src/ThirdParty/cpputest)

    add_subdirectory(tests)
endif()
