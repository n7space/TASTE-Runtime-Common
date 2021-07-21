project(Packetizer VERSION 1.0.0 LANGUAGES C)

add_library(Packetizer STATIC)
target_sources(Packetizer
    PRIVATE     main.c
                Crc16.c
                Packetizer.c
                Crc16.h
                PacketizerInternal.h
    PUBLIC      Packetizer.h
                SpacePacket.h)

target_include_directories(Packetizer
    PUBLIC      ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Packetizer
    PRIVATE     common_build_options)

add_format_target(Packetizer)

add_library(TASTE::Packetizer ALIAS Packetizer)
