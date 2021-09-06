project(Packetizer VERSION 1.0.0 LANGUAGES C)

add_library(Packetizer STATIC)
target_sources(Packetizer
    PRIVATE     Crc16.c
                Packetizer.c
                Crc16.h
                PacketizerInternal.h
                SpacePacketInternal.h
    PUBLIC      Packetizer.h
                SpacePacket.h)

target_include_directories(Packetizer
    PUBLIC      ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Packetizer
    PRIVATE     common_build_options)

target_compile_definitions(Packetizer PUBLIC STANDARD_SPACE_PACKET=1)

add_format_target(Packetizer)

add_library(TASTE::Packetizer ALIAS Packetizer)
