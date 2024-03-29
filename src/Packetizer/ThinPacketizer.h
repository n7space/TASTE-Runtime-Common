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

#ifndef THIN_PACKETIZER_H
#define THIN_PACKETIZER_H

/**
 * @file    ThinPacketizer.h
 * @brief   Main header file for Space Packet thin packetizer.
 */

#include "Packetizer.h"

/**
 * @brief   Initialize Packetizer struct
 *
 * @param[in]   self         Pointer to a structure representing Packetizer
 * @param[in]   busId        Bus ID of the device
 * @param[out]  headerSize   Primary header size of the Packetizer
 */
void ThinPacketizer_init(Packetizer* const self,
                         const enum SystemBus busId,
                         size_t* const headerSize);

/**
 * @brief   Packetize given data with Space Packet header and CRC.
 *
 * This function assumes that the user is passing a pointer to array
 * that contains data which is properly offsetted for header to fit
 * and is also padded to for CRC.
 *
 * @param[in,out]   self            Pointer to a structure representing Packetizer
 * @param[in]       packetType      Type of packet that should be created
 * @param[in]       busId           Bus ID of the device
 * @param[in]       source          Source application ID
 * @param[in]       destination     Destination application ID
 * @param[in,out]   packetPointer   Pointer to an array with the data
 * @param[in]       dataOffset      Offset to where data starts
 * @param[in]       dataSize        Size of the data
 *
 * @returns Packet size.
 */
size_t ThinPacketizer_packetize(Packetizer* const self,
                                const Packetizer_PacketType packetType,
                                const enum SystemBus busId,
                                const uint16_t source,
                                const uint16_t destination,
                                uint8_t* const packetPointer,
                                const size_t dataOffset,
                                const size_t dataSize);

/**
 * @brief   Extracts data from given packet.
 *
 * @param[in]   self            Pointer to a structure representing Packetizer
 * @param[in]   packetType      Expected packet type
 * @param[in]   packetPointer   Pointer to an array with the packet data
 * @param[in]   packetSize      Size of the packet data
 * @param[in]   busId           Bus ID of the device
 * @param[out]  source          Source application ID
 * @param[out]  destination     Destination application ID
 * @param[out]  dataOffset      Offset to where data starts
 * @param[out]  dataSize        Size of the data
 * @param[out]  errorCode       Error code if depacketization failed
 *
 * @returns  True is depacketization succeeded, false otherwise.
 */
bool ThinPacketizer_depacketize(const Packetizer* const self,
                                const Packetizer_PacketType packetType,
                                const uint8_t* const packetPointer,
                                const size_t packetSize,
                                const enum SystemBus busId,
                                uint16_t* const source,
                                uint16_t* const destination,
                                size_t* const dataOffset,
                                size_t* const dataSize,
                                int32_t* const errorCode);

#endif // THIN_PACKETIZER_H
