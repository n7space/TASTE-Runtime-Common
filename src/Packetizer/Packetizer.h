#ifndef PACKETIZER_H
#define PACKETIZER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define PACKETIZER_DESTINATION_HIGH_BITS_MASK 0x0700
#define PACKETIZER_PACKET_SEQUENCE_CONTROL_HIGH_BITS_MASK 0x3F00

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

size_t Packetizer_packetize(Packetizer* const self,
                            const Packetizer_PacketType packetType,
                            const uint16_t source,
                            const uint16_t destination,
                            uint8_t* const dataPointer,
                            const size_t dataOffset,
                            const size_t dataSize);

bool Packetizer_depacketize(const Packetizer* const self,
                            const Packetizer_PacketType packetType,
                            const uint8_t* const packetPointer,
                            const size_t packetSize,
                            uint16_t* const source,
                            uint16_t* const destination,
                            size_t* const dataOffset,
                            size_t* const dataSize,
                            uint32_t* const errorCode);

#endif // PACKETIZER_H
