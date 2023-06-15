add_library(RuntimeMocks STATIC)
target_sources(RuntimeMocks
  PRIVATE      system_spec.c
               device_provided_packetizer.c
               routing.c
  PUBLIC       system_spec.h
               device_provided_packetizer.h
               routing.h)

target_include_directories(RuntimeMocks
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(RuntimeMocks
  PRIVATE     common_build_options
  PUBLIC      TASTE::Packetizer)

add_format_target(RuntimeMocks)

add_library(TASTE::RuntimeMocks ALIAS RuntimeMocks)
