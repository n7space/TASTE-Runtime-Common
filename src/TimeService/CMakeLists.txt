project(TimeService VERSION 1.0.0 LANGUAGES C)

add_library(TimeService STATIC)
target_sources(TimeService
    PRIVATE     timeservice-utils.h
                timeservice-dummy.h
                timeservice-linux.h
                timeservice-rtems.h
                timeservice-api.c
    PUBLIC      timeservice-api.h)

target_include_directories(TimeService
    PUBLIC      ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(TimeService
    PRIVATE     common_build_options)

target_compile_definitions(TimeService PUBLIC STANDARD_SPACE_PACKET=1)

add_format_target(TimeService)

add_library(TASTE::TimeService ALIAS TimeService)
