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

#ifndef SPACE_PACKET_H
#define SPACE_PACKET_H

#ifdef STANDARD_SPACE_PACKET
/// @brief  Size of the Space Packet header, in bytes
#define SPACE_PACKET_PRIMARY_HEADER_SIZE 6u
#else
/// @brief  Size of the Space Packet header, in bytes
#define SPACE_PACKET_PRIMARY_HEADER_SIZE 8u
#endif

/// @brief  Size of the packet CRC, in bytes
#define SPACE_PACKET_ERROR_CONTROL_SIZE 2u

#ifdef STANDARD_SPACE_PACKET
/// @brief  Max size of the packet data, in bytes
#define SPACE_PACKET_MAX_PACKET_DATA_SIZE 65536u // 2^16
#else
/// @brief  Max size of the packet data, in bytes
#define SPACE_PACKET_MAX_PACKET_DATA_SIZE 4294967295u // 2^32
#endif

/// @brief  Max value of the packed APID (destination)
#define SPACE_PACKET_MAX_APID 2047u // 2^11-1
/// @brief  Max value of the packet sequence counter
#define SPACE_PACKET_MAX_PACKET_SEQUENCE_COUNT 16383u // 2^14-1

#endif // SPACE_PACKET_H
