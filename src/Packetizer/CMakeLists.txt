project(Packetizer VERSION 1.0.0 LANGUAGES C)

add_library(Packetizer STATIC)
target_sources(Packetizer
    PRIVATE     Crc16.c
                IsoChecksum.c
                Packetizer.c
                CCSDSPacketizer.c
                ThinPacketizer.c
                Crc16.h
                IsoChecksum.h
                PacketizerInternal.h
                SpacePacketInternal.h
                DeviceProvidedPacketizer.c
                PassthroughPacketizer.c
    PUBLIC      Packetizer.h
                CCSDSPacketizer.h
                DeviceProvidedPacketizer.h
                SpacePacket.h
                PassthroughPacketizer.h)

target_include_directories(Packetizer
    PUBLIC      ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Packetizer
    PRIVATE     common_build_options
    PUBLIC      TASTE::RuntimeMocks)

add_format_target(Packetizer)

add_library(TASTE::Packetizer ALIAS Packetizer)
