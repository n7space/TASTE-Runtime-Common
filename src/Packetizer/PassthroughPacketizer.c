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

#include "PassthroughPacketizer.h"

#include <assert.h>
#include <string.h>

#include "routing.h"
#include "PacketizerInternal.h"
#include "SpacePacketInternal.h"

void
PassthroughPacketizer_init(Packetizer* const self, const enum SystemBus busId, size_t* const headerSize)
{
    // Unused in this implementation
    (void)self;
    (void)busId;

    *headerSize = 0;
}

size_t
PassthroughPacketizer_packetize(Packetizer* const self,
                                const Packetizer_PacketType packetType,
                                const enum SystemBus busId,
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
    (void)busId;
    (void)dataOffset;
    (void)destination;

    assert(packetPointer != NULL);
    assert(dataSize >= 1);

    return dataSize;
}

bool
PassthroughPacketizer_depacketize(const Packetizer* const self,
                                  const Packetizer_PacketType packetType,
                                  const uint8_t* const packetPointer,
                                  const size_t packetSize,
                                  const enum SystemBus busId,
                                  uint16_t* const source,
                                  uint16_t* const destination,
                                  size_t* const dataOffset,
                                  size_t* const dataSize,
                                  int32_t* const errorCode)
{
    // Unused in this implementation
    (void)self;
    (void)packetType;
    (void)busId;
    (void)source;
    (void)errorCode;

    assert(packetPointer != NULL);
    assert(dataOffset != NULL);
    assert(dataSize != NULL);
    assert(destination != NULL);

    // Save the results
    *destination = bus_to_unique_port_map[busId];
    *dataOffset = 0;
    *dataSize = packetSize;

    return true;
}
