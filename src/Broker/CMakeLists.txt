project(Broker VERSION 1.0.0 LANGUAGES C)

add_library(Broker STATIC)
target_sources(Broker
    PRIVATE     Broker.c
    PUBLIC      Broker.h)

target_include_directories(Broker
  PUBLIC      ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Broker
  PRIVATE     common_build_options
  PUBLIC      TASTE::Packetizer
              TASTE::RuntimeMocks)

add_format_target(Broker)

add_library(TASTE::Broker ALIAS Broker)
