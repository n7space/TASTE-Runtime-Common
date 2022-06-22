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

#include "Packetizer.h"

#include <assert.h>
#include <string.h>

#include "PacketizerInternal.h"
#include "SpacePacketInternal.h"

#define THIN_PACKET_PRIMARY_HEADER_SIZE 4u

uint16_t
readPacketId(const uint8_t* const packetPointer);

void
Packetizer_init(Packetizer* const self)
{
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
    (void)self;
    (void)packetType;
    (void)source;
    (void)dataOffset;

    assert(destination <= SPACE_PACKET_MAX_APID);
    assert(packetPointer != NULL);
    assert(dataOffset == THIN_PACKET_PRIMARY_HEADER_SIZE);
    assert(dataSize >= 1);
    assert(dataSize < SPACE_PACKET_MAX_PACKET_DATA_SIZE);

    memset(packetPointer, 0, THIN_PACKET_PRIMARY_HEADER_SIZE);

    writePacketId(packetPointer, packetType, destination);
    writePacketDataLength(packetPointer, dataSize);

    return THIN_PACKET_PRIMARY_HEADER_SIZE + dataSize;
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
    (void)packetType;
    (void)source;

    assert(packetPointer != NULL);
    assert(dataOffset != NULL);
    assert(dataSize != NULL);
    assert(destination != NULL);

    if(packetSize < THIN_PACKET_PRIMARY_HEADER_SIZE) {
        // the packet is smaller than expected (at least header)
        return false;
    }

    // Get and check data size
    const size_t receivedDataSize = readPacketDataLength(packetPointer);
    if(packetSize != THIN_PACKET_PRIMARY_HEADER_SIZE + receivedDataSize) {
        if(errorCode != NULL) {
            *errorCode = Packetizer_ErrorCode_IncorrectPacketSize;
        }
        return false;
    }

    // Save the results
    *destination = readPacketId(packetPointer);
    *dataOffset = THIN_PACKET_PRIMARY_HEADER_SIZE;
    *dataSize = receivedDataSize;

    return true;
}

void
writePacketId(uint8_t* const packetPointer, const Packetizer_PacketType packetType, const uint16_t destination)
{
    packetPointer[0] |= (destination >> 8u) & 0xFF;
    packetPointer[1] |= destination & 0xFF;
}

uint16_t
readPacketId(const uint8_t* const packetPointer)
{
    return (uint16_t)(packetPointer[0] << 8u) | packetPointer[1];
}

void
writePacketDataLength(uint8_t* const packetPointer, const size_t dataSize)
{
    packetPointer[2] = ((dataSize - 1) >> 8) & 0xFF;
    packetPointer[3] = (dataSize - 1) & 0xFF;
}

size_t
readPacketDataLength(const uint8_t* const packetPointer)
{
    return ((size_t)(packetPointer[2] << 8u) | packetPointer[3]) + 1;
}
