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

#ifndef PACKETIZER_INTERNAL_H
#define PACKETIZER_INTERNAL_H

#define PACKETIZER_DESTINATION_HIGH_BITS_MASK 0x0700
#define PACKETIZER_PACKET_SEQUENCE_CONTROL_HIGH_BITS_MASK 0x3F00

void writePacketId(uint8_t* const packetPointer, const Packetizer_PacketType packetType, const uint16_t source);
void writePacketSequenceControl(uint8_t* const packetPointer, const Packetizer* const packetizer);
void writePacketDataLength(uint8_t* const packetPointer, const size_t dataSize);
void writeCrc(uint8_t* const packetPointer, const size_t dataSize);
size_t readPacketDataLength(const uint8_t* const packetPointer);

#endif // PACKETIZER_INTERNAL_H
