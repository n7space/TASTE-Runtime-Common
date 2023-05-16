/**@file
 * This file is part of the TASTE Runtime Common.
 *
 * @copyright 2023 N7 Space Sp. z o.o.
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

#include "DeviceProvidedPacketizer.h"

void
DeviceProvidedPacketizer_init(Packetizer* const self, const enum SystemBus busId)
{
    packetizer_init_function packetizer_init = getDeviceProvidedPacketizerInitFunction(busId);

    packetizer_init(self, busId);
}

size_t
DeviceProvidedPacketizer_packetize(Packetizer* const self,
                                   const Packetizer_PacketType packetType,
                                   const enum SystemBus busId,
                                   const uint16_t source,
                                   const uint16_t destination,
                                   uint8_t* const packetPointer,
                                   const size_t dataOffset,
                                   const size_t dataSize)
{
    packetizer_packetize_function packetizer_packetize = getDeviceProvidedPacketizerPacketizeFunction(busId);

    return packetizer_packetize(self, packetType, busId, source, destination, packetPointer, dataOffset, dataSize);
}

bool
DeviceProvidedPacketizer_depacketize(const Packetizer* const self,
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
    packetizer_depacketize_function packetizer_depacketize = getDeviceProvidedPacketizerDepacketizeFunction(busId);

    return packetizer_depacketize(
            self, packetType, packetPointer, packetSize, busId, source, destination, dataOffset, dataSize, errorCode);
}
