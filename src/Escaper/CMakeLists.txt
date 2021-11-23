project(Escaper VERSION 1.0.0 LANGUAGES C)

add_library(Escaper STATIC)
target_sources(Escaper
  PRIVATE    Escaper.c
  PUBLIC     Escaper.h)

target_include_directories(Escaper
  PUBLIC     ${CMAKE_CURRENT_SOURCE_DIR})
  
target_link_libraries(Escaper
  PRIVATE    common_build_options)

add_format_target(Escaper)
add_library(TASTE::Escaper ALIAS Escaper)
