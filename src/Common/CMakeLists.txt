project(Common VERSION 1.0.0 LANGUAGES C)

add_library(Common STATIC)
target_sources(Common
    PRIVATE     DriverHelper.c
    PUBLIC      DriverHelper.h)

target_include_directories(Common
    PUBLIC      ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Common
    PRIVATE     common_build_options)

target_link_libraries(Common
  PUBLIC      TASTE::Mocks)

add_format_target(Common)

add_library(TASTE::Common ALIAS Common)
