// TODO: ADD LICENSE

#include "Packetizer.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "SpacePacket.h"

static void writePacketId(uint8_t* const dataPointer, const Packetizer_PacketType packetType, const uint16_t source);
static void writePacketSequenceControl(uint8_t* const dataPointer, const Packetizer* const packetizer);
static void writePacketDataLength(uint8_t* const dataPointer, const uint32_t dataSize);

uint32_t
Packetizer_packetize(Packetizer* const self,
                     const Packetizer_PacketType packetType,
                     const uint16_t source,
                     const uint16_t destination,
                     uint8_t* const dataPointer,
                     const uint32_t dataOffset,
                     const uint32_t dataSize)
{
    // Unused in this implementation
    (void)source;
    (void)dataOffset;

    assert(self->packetSequenceCount <= SPACE_PACKET_MAX_PACKET_SEQUENCE_COUNT);
    assert(destination <= SPACE_PACKET_MAX_APID_SIZE);
    assert(dataPointer != NULL);
    assert(dataOffset == SPACE_PACKET_PRIMARY_HEADER_SIZE_BYTES);
    assert(dataSize >= 1);
    assert(dataSize < SPACE_PACKET_MAX_PACKET_DATA_SIZE);

    memset(dataPointer, 0, SPACE_PACKET_PRIMARY_HEADER_SIZE_BYTES);

    writePacketId(dataPointer, packetType, destination);
    writePacketSequenceControl(dataPointer, self);
    writePacketDataLength(dataPointer, dataSize);

    // TODO: Calculate CRC16

    // Increase sequence counter, it should wrap to zero after 2^14-1
    ++self->packetSequenceCount;
    if(self->packetSequenceCount > SPACE_PACKET_MAX_PACKET_SEQUENCE_COUNT) {
        self->packetSequenceCount = 0; // Counter should wrap to zero
    }

    return 0;
}

bool
Packetizer_depacketize(const Packetizer* const self,
                       const Packetizer_PacketType packetType,
                       const uint8_t* const packetPointer,
                       const uint32_t packetSize,
                       uint16_t* const source,
                       uint16_t* const destination,
                       uint32_t* const dataOffset,
                       uint32_t* const dataSize,
                       uint32_t* const errorCode)
{
    // Unused in this implementation
    (void)self;
    (void)source;

    (void)packetSize;

    assert(packetPointer != NULL);
    assert(packetSize > SPACE_PACKET_PRIMARY_HEADER_SIZE_BYTES + SPACE_PACKET_ERROR_CONTROL_SIZE_BYTES);
    assert(dataOffset != NULL);
    assert(dataSize != NULL);
    assert(destination != NULL);

    // TODO Check if CRC16 is correct

    // Check packet type
    const Packetizer_PacketType receivedPacketType = (packetPointer[0] & PACKETIZER_PACKET_TYPE_MASK) != 0;
    if(receivedPacketType != packetType) {
        if(errorCode != NULL) {
            *errorCode = Packetizer_ErrorCode_IncorrectPacketType;
        }
        return false;
    }

    // Save the results
    *destination = ((packetPointer[0] & PACKETIZER_APID_HIGH_BITS_MASK) << (8u - PACKETIZER_APID_HIGH_BITS_OFFSET))
                   | packetPointer[1];
    *dataOffset = SPACE_PACKET_PRIMARY_HEADER_SIZE_BYTES;
    *dataSize = (uint32_t)(packetPointer[4] << 8u) | packetPointer[5];

    return true;
}

void
writePacketId(uint8_t* const dataPointer, const Packetizer_PacketType packetType, const uint16_t destination)
{
    // 4th bit - Packet type (1 bit)
    dataPointer[0] |= packetType << PACKETIZER_PACKET_TYPE_OFFSET;

    // 5th bit - Secondary header flag, always set (1 bit)
    dataPointer[0] |= 1 << PACKETIZER_SECONDARY_HEADER_FLAG_OFFSET;

    // 6th bit - Application process ID (11 bits) - BIG ENDIAN
    dataPointer[0] |=
            (destination & PACKETIZER_DESTINATION_HIGH_BITS_MASK) >> (8u - PACKETIZER_DESTINATION_HIGH_BITS_OFFSET);
    dataPointer[1] |= destination & 0xFF;
}

void
writePacketSequenceControl(uint8_t* const dataPointer, const Packetizer* const packetizer)
{
    // 1st bit - Sequence flags, both bits always set (2 bits)
    dataPointer[2] |= 1 << PACKETIZER_SEQUENCE_FLAGS_FIRST_OFFSET;
    dataPointer[2] |= 1 << PACKETIZER_SEQUENCE_FLAGS_SECOND_OFFSET;

    // 3rd bit - Packet sequence count (14 bits)
    dataPointer[2] |= (packetizer->packetSequenceCount & PACKETIZER_SEQUENCE_COUNT_HIGH_BITS_MASK)
                      >> (8u - PACKETIZER_SEQUENCE_COUNT_HIGH_BITS_OFFSET);
    dataPointer[3] |= packetizer->packetSequenceCount & 0xFF;
}

void
writePacketDataLength(uint8_t* const dataPointer, const uint32_t dataSize)
{
    dataPointer[4] = ((dataSize - 1) >> 8) & 0xFF;
    dataPointer[5] = (dataSize - 1) & 0xFF;
}
