/**@file
 * This file is part of the TASTE Runtime Common.
 *
 * @copyright 2021 N7 Space Sp. z o.o.
 *
 * TASTE Runtime Common was developed under a programme of,
 * and funded by, the European Space Agency (the "ESA").
 *
 * Licensed under the ESA Public License (ESA-PL) Permissive,
 * Version 2.3 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://essr.esa.int/license/list
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Packetizer.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "Crc16.h"
#include "PacketizerInternal.h"
#include "SpacePacketInternal.h"

void
Packetizer_init(Packetizer* const self)
{
    self->packetSequenceCount = 0;
}

size_t
Packetizer_packetize(Packetizer* const self,
                     const Packetizer_PacketType packetType,
                     const uint16_t source,
                     const uint16_t destination,
                     uint8_t* const packetPointer,
                     const size_t dataOffset,
                     const size_t dataSize)
{
    // Unused in this implementation
    (void)source;
    (void)dataOffset;

    assert(self->packetSequenceCount <= SPACE_PACKET_MAX_PACKET_SEQUENCE_COUNT);
    assert(destination <= SPACE_PACKET_MAX_APID);
    assert(packetPointer != NULL);
    assert(dataOffset == SPACE_PACKET_PRIMARY_HEADER_SIZE);
#ifdef STANDARD_SPACE_PACKET
    assert(dataSize >= 1);
#endif
    assert(dataSize < SPACE_PACKET_MAX_PACKET_DATA_SIZE);

    memset(packetPointer, 0, SPACE_PACKET_PRIMARY_HEADER_SIZE);

    writePacketId(packetPointer, packetType, destination);
    writePacketSequenceControl(packetPointer, self);
    writePacketDataLength(packetPointer, dataSize);
    writeCrc(packetPointer, dataSize);

    // Increase sequence counter, it should wrap to zero after 2^14-1
    ++self->packetSequenceCount;
    if(self->packetSequenceCount > SPACE_PACKET_MAX_PACKET_SEQUENCE_COUNT) {
        self->packetSequenceCount = 0; // Counter should wrap to zero
    }

    return SPACE_PACKET_PRIMARY_HEADER_SIZE + dataSize + SPACE_PACKET_ERROR_CONTROL_SIZE;
}

bool
Packetizer_depacketize(const Packetizer* const self,
                       const Packetizer_PacketType packetType,
                       const uint8_t* const packetPointer,
                       const size_t packetSize,
                       uint16_t* const source,
                       uint16_t* const destination,
                       size_t* const dataOffset,
                       size_t* const dataSize,
                       int32_t* const errorCode)
{
    // Unused in this implementation
    (void)self;
    (void)source;

    assert(packetPointer != NULL);
    assert(dataOffset != NULL);
    assert(dataSize != NULL);
    assert(destination != NULL);

#ifdef STANDARD_SPACE_PACKET
    if(packetSize <= SPACE_PACKET_PRIMARY_HEADER_SIZE + SPACE_PACKET_ERROR_CONTROL_SIZE) {
        // the packet is smaller than expected (header + 1 byte of payload + checksum)
        return false;
    }
#else
    if(packetSize < SPACE_PACKET_PRIMARY_HEADER_SIZE + SPACE_PACKET_ERROR_CONTROL_SIZE) {
        // the packet is smaller than expected (header + checksum)
        return false;
    }
#endif

    // Get and check data size
    const size_t receivedDataSize = readPacketDataLength(packetPointer);
    if(packetSize != SPACE_PACKET_PRIMARY_HEADER_SIZE + receivedDataSize + SPACE_PACKET_ERROR_CONTROL_SIZE) {
        if(errorCode != NULL) {
            *errorCode = Packetizer_ErrorCode_IncorrectPacketSize;
        }
        return false;
    }

    // Check if CRC matches
    uint16_t receivedCrc = packetPointer[SPACE_PACKET_PRIMARY_HEADER_SIZE + receivedDataSize + 1]
                           | (packetPointer[SPACE_PACKET_PRIMARY_HEADER_SIZE + receivedDataSize] << 8);
    uint16_t expectedCrc = calculateCrc16(packetPointer, packetSize - SPACE_PACKET_ERROR_CONTROL_SIZE);

    if(receivedCrc != expectedCrc) {
        if(errorCode != NULL) {
            *errorCode = Packetizer_ErrorCode_IncorrectCrc16;
        }
        return false;
    }

    // Check packet type
    const Packetizer_PacketType receivedPacketType =
            (packetPointer[0] & SPACE_PACKET_TYPE_MASK) >> SPACE_PACKET_TYPE_OFFSET;
    if(receivedPacketType != packetType) {
        if(errorCode != NULL) {
            *errorCode = Packetizer_ErrorCode_IncorrectPacketType;
        }
        return false;
    }

    // Save the results
    *destination = packetPointer[1]
                   | (packetPointer[0] & SPACE_PACKET_APID_HIGH_BITS_MASK) << (8u - SPACE_PACKET_APID_HIGH_BITS_OFFSET);
    *dataOffset = SPACE_PACKET_PRIMARY_HEADER_SIZE;
    *dataSize = receivedDataSize;

    return true;
}

void
writePacketId(uint8_t* const packetPointer, const Packetizer_PacketType packetType, const uint16_t destination)
{
    // 4th bit - Packet type (1 bit)
    packetPointer[0] |= packetType << SPACE_PACKET_TYPE_OFFSET;

    // 5th bit - Secondary header flag, always set (1 bit)
    packetPointer[0] |= 1 << SPACE_PACKET_SECONDARY_HEADER_FLAG_OFFSET;

    // 6th bit - Application process ID (11 bits) - BIG ENDIAN
    packetPointer[0] |=
            (destination & PACKETIZER_DESTINATION_HIGH_BITS_MASK) >> (8u - SPACE_PACKET_APID_HIGH_BITS_OFFSET);
    packetPointer[1] |= destination & 0xFF;
}

void
writePacketSequenceControl(uint8_t* const packetPointer, const Packetizer* const packetizer)
{
    // 1st bit - Sequence flags, both bits always set (2 bits)
    packetPointer[2] |= 1 << SPACE_PACKET_SEQUENCE_FLAGS_FIRST_OFFSET;
    packetPointer[2] |= 1 << SPACE_PACKET_SEQUENCE_FLAGS_SECOND_OFFSET;

    // 3rd bit - Packet sequence count (14 bits)
    packetPointer[2] |= (packetizer->packetSequenceCount & PACKETIZER_PACKET_SEQUENCE_CONTROL_HIGH_BITS_MASK)
                        >> (8u - SPACE_PACKET_SEQUENCE_CONTROL_HIGH_BITS_OFFSET);
    packetPointer[3] |= packetizer->packetSequenceCount & 0xFF;
}

void
writePacketDataLength(uint8_t* const packetPointer, const size_t dataSize)
{
#ifdef STANDARD_SPACE_PACKET
    packetPointer[4] = ((dataSize - 1) >> 8) & 0xFF;
    packetPointer[5] = (dataSize - 1) & 0xFF;
#else
    if(dataSize != 0) {
        packetPointer[4] = ((dataSize - 1) >> 24) & 0xFF;
        packetPointer[5] = ((dataSize - 1) >> 16) & 0xFF;
        packetPointer[6] = ((dataSize - 1) >> 8) & 0xFF;
        packetPointer[7] = (dataSize - 1) & 0xFF;
    } else {
        packetPointer[4] = 0xFF;
        packetPointer[5] = 0xFF;
        packetPointer[6] = 0xFF;
        packetPointer[7] = 0xFF;
    }
#endif
}

void
writeCrc(uint8_t* const packetPointer, const size_t dataSize)
{
    uint16_t crc = calculateCrc16(packetPointer, SPACE_PACKET_PRIMARY_HEADER_SIZE + dataSize);

    packetPointer[SPACE_PACKET_PRIMARY_HEADER_SIZE + dataSize] = (crc >> 8) & 0xFF;
    packetPointer[SPACE_PACKET_PRIMARY_HEADER_SIZE + dataSize + 1] = crc & 0xFF;
}

size_t
readPacketDataLength(const uint8_t* const packetPointer)
{
#ifdef STANDARD_SPACE_PACKET
    return ((size_t)(packetPointer[4] << 8u) | packetPointer[5]) + 1;
#else
    const uint32_t packetSize =
            ((size_t)(packetPointer[4] << 24u) | packetPointer[5] << 16u | packetPointer[6] << 8u | packetPointer[7]);
    const uint32_t zeroPacketSize = 0xffffffff;
    if(packetSize == zeroPacketSize) {
        return 0;
    }
    return (size_t)packetSize + 1;
#endif
}
