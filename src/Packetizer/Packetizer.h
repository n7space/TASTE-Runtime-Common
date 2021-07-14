#ifndef PACKETIZER_H
#define PACKETIZER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define PACKETIZER_PACKET_TYPE_MASK 0x08
#define PACKETIZER_APID_HIGH_BITS_MASK 0xE0
#define PACKETIZER_DESTINATION_HIGH_BITS_MASK 0x700
#define PACKETIZER_SEQUENCE_COUNT_HIGH_BITS_MASK 0x3F00

#define PACKETIZER_PACKET_TYPE_OFFSET 3u
#define PACKETIZER_SECONDARY_HEADER_FLAG_OFFSET 4u
#define PACKETIZER_APID_OFFSET 5u
#define PACKETIZER_APID_HIGH_BITS_OFFSET 5u
#define PACKETIZER_DESTINATION_HIGH_BITS_OFFSET 5u
#define PACKETIZER_SEQUENCE_FLAGS_FIRST_OFFSET 0u
#define PACKETIZER_SEQUENCE_FLAGS_SECOND_OFFSET 1u
#define PACKETIZER_SEQUENCE_COUNT_HIGH_BITS_OFFSET 2u

typedef enum {
    Packetizer_PacketType_Telemetry = 0,  ///< Telemetry packet
    Packetizer_PacketType_Telecommand = 1 ///< Telecommand packet
} Packetizer_PacketType;

typedef enum {
    Packetizer_ErrorCode_IncorrectCrc16 = 1,
    Packetizer_ErrorCode_IncorrectPacketType = 2
} Packetizer_ErrorCode;

typedef struct {
    uint16_t packetSequenceCount;
} Packetizer;

uint32_t Packetizer_packetize(Packetizer* const self,
                              const Packetizer_PacketType packetType,
                              const uint16_t source,
                              const uint16_t destination,
                              uint8_t* const dataPointer,
                              const uint32_t dataOffset,
                              const uint32_t dataSize);

bool Packetizer_depacketize(const Packetizer* const self,
                            const Packetizer_PacketType packetType,
                            const uint8_t* const packetPointer,
                            const uint32_t packetSize,
                            uint16_t* const source,
                            uint16_t* const destination,
                            uint32_t* const dataOffset,
                            uint32_t* const dataSize,
                            uint32_t* const errorCode);

#endif // PACKETIZER_H
