/**@file
 * This file is part of the TASTE Runtime Common.
 *
 * @copyright 2022 N7 Space Sp. z o.o.
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

#include "CCSDSPacketizer.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "IsoChecksum.h"
#include "PacketizerInternal.h"
#include "SpacePacketInternal.h"

void
CCSDS_Packetizer_init(Packetizer* const self)
{
    self->packetSequenceCount = 0;
}

size_t
CCSDS_Packetizer_packetize(Packetizer* const self,
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
    assert(dataSize >= 1);
    assert(dataSize < SPACE_PACKET_MAX_PACKET_DATA_SIZE);
    const uint16_t rawDestination = destination;
    assert(rawDestination <= SPACE_PACKET_MAX_APID);

    memset(packetPointer, 0, SPACE_PACKET_PRIMARY_HEADER_SIZE);

    writeCCSDSPacketId(packetPointer, packetType, rawDestination);
    writeCCSDSPacketSequenceControl(packetPointer, self);
    writeCCSDSPacketDataLength(packetPointer, dataSize);
    writeChecksum(packetPointer, dataSize);

    // Increase sequence counter, it should wrap to zero after 2^14-1
    ++self->packetSequenceCount;
    if(self->packetSequenceCount > SPACE_PACKET_MAX_PACKET_SEQUENCE_COUNT) {
        self->packetSequenceCount = 0; // Counter should wrap to zero
    }

    // Add SPACE_PACKET_ERROR_CONTROL_SIZE here, because checksum is a part of payload
    return SPACE_PACKET_PRIMARY_HEADER_SIZE + dataSize + SPACE_PACKET_ERROR_CONTROL_SIZE;
}

bool
CCSDS_Packetizer_depacketize(const Packetizer* const self,
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

    if(packetSize <= SPACE_PACKET_PRIMARY_HEADER_SIZE + SPACE_PACKET_ERROR_CONTROL_SIZE) {
        // the packet is smaller than expected (header + 1 byte of payload + checksum)
        return false;
    }

    // Get and check data size
    const size_t receivedDataSize = readCCSDSPacketDataLength(packetPointer);
    if(packetSize != SPACE_PACKET_PRIMARY_HEADER_SIZE + receivedDataSize) {
        if(errorCode != NULL) {
            *errorCode = CCSDS_Packetizer_ErrorCode_IncorrectPacketSize;
        }
        return false;
    }

    // Check if CRC matches
    const uint16_t receivedChecksum = packetPointer[SPACE_PACKET_PRIMARY_HEADER_SIZE + receivedDataSize - 1]
                                      | (packetPointer[SPACE_PACKET_PRIMARY_HEADER_SIZE + receivedDataSize - 2] << 8);

    const uint16_t expectedChecksum =
            IsoChecksum_calculate(packetPointer, packetSize - SPACE_PACKET_ERROR_CONTROL_SIZE);

    if(receivedChecksum != expectedChecksum) {
        if(errorCode != NULL) {
            *errorCode = CCSDS_Packetizer_ErrorCode_IncorrectChecksum;
        }
        return false;
    }

    // Check packet type
    const Packetizer_PacketType receivedPacketType =
            (packetPointer[0] & CCSDS_SPACE_PACKET_TYPE_MASK) >> CCSDS_SPACE_PACKET_TYPE_OFFSET;
    if(receivedPacketType != packetType) {
        if(errorCode != NULL) {
            *errorCode = CCSDS_Packetizer_ErrorCode_IncorrectPacketType;
        }
        return false;
    }

    // Save the results
    *destination = packetPointer[1] | (packetPointer[0] & CCSDS_SPACE_PACKET_APID_HIGH_BITS_MASK) << 8u;
    *dataOffset = SPACE_PACKET_PRIMARY_HEADER_SIZE;
    *dataSize = receivedDataSize - SPACE_PACKET_ERROR_CONTROL_SIZE;

    return true;
}

void
writeCCSDSPacketId(uint8_t* const packetPointer, const Packetizer_PacketType packetType, const uint16_t destination)
{
    // 4th bit - Packet type (1 bit)
    packetPointer[0] |= packetType << CCSDS_SPACE_PACKET_TYPE_OFFSET;

    // 5th bit - Secondary header flag, always set (1 bit)
    packetPointer[0] |= 1 << CCSDS_SPACE_PACKET_SECONDARY_HEADER_FLAG_OFFSET;

    // 6th bit - Application process ID (11 bits) - BIG ENDIAN
    packetPointer[0] |= (destination & PACKETIZER_DESTINATION_HIGH_BITS_MASK) >> 8u;
    packetPointer[1] |= destination & 0xFF;
}

void
writeCCSDSPacketSequenceControl(uint8_t* const packetPointer, const Packetizer* const packetizer)
{
    // 1st bit - Sequence flags, both bits always set (2 bits)
    packetPointer[2] |= 1 << CCSDS_SPACE_PACKET_SEQUENCE_FLAGS_FIRST_OFFSET;
    packetPointer[2] |= 1 << CCSDS_SPACE_PACKET_SEQUENCE_FLAGS_SECOND_OFFSET;

    // 3rd bit - Packet sequence count (14 bits)
    packetPointer[2] |= (packetizer->packetSequenceCount & PACKETIZER_PACKET_SEQUENCE_CONTROL_HIGH_BITS_MASK) >> 8u;
    packetPointer[3] |= packetizer->packetSequenceCount & 0xFF;
}

void
writeCCSDSPacketDataLength(uint8_t* const packetPointer, const size_t dataSize)
{
    const size_t payloadSize = dataSize + SPACE_PACKET_ERROR_CONTROL_SIZE;
    packetPointer[4] = ((payloadSize - 1) >> 8) & 0xFF;
    packetPointer[5] = (payloadSize - 1) & 0xFF;
}

void
writeChecksum(uint8_t* const packetPointer, const size_t dataSize)
{
    const size_t checksumInputSize = SPACE_PACKET_PRIMARY_HEADER_SIZE + dataSize;
    const uint16_t checksum = IsoChecksum_calculate(packetPointer, checksumInputSize);

    packetPointer[checksumInputSize] = (checksum >> 8) & 0xFF;
    packetPointer[checksumInputSize + 1] = checksum & 0xFF;
}

size_t
readCCSDSPacketDataLength(const uint8_t* const packetPointer)
{
    return ((size_t)(packetPointer[4] << 8u) | packetPointer[5]) + 1;
}
