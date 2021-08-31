add_library(Mocks STATIC)
target_sources(Mocks
  PRIVATE      system_spec.c
  PUBLIC       system_spec.h)

target_include_directories(Mocks
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Mocks
  PRIVATE common_build_options)

add_format_target(Mocks)

add_library(TASTE::Mocks ALIAS Mocks)
