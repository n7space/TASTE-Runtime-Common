project(RuntimeCommon VERSION 1.0.0 LANGUAGES C)

add_library(RuntimeCommon STATIC)
target_sources(RuntimeCommon
    PRIVATE     DriverHelper.c
    PUBLIC      DriverHelper.h)

target_include_directories(RuntimeCommon
    PUBLIC      ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(RuntimeCommon
    PRIVATE     common_build_options
                TASTE::RuntimeMocks)

add_format_target(RuntimeCommon)

add_library(TASTE::RuntimeCommon ALIAS RuntimeCommon)
