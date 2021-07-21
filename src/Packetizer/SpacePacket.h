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

#define SPACE_PACKET_PRIMARY_HEADER_SIZE_BYTES 6u
#define SPACE_PACKET_ERROR_CONTROL_SIZE_BYTES 2u
#define SPACE_PACKET_MAX_PACKET_DATA_SIZE 65536u      // 2^16
#define SPACE_PACKET_MAX_APID_SIZE 2047u              // 2^11-1
#define SPACE_PACKET_MAX_PACKET_SEQUENCE_COUNT 16383u // 2^14-1

#define SPACE_PACKET_VERSION_NUMBER_MASK 0x07
#define SPACE_PACKET_TYPE_MASK 0x08
#define SPACE_PACKET_APID_HIGH_BITS_MASK 0xE0
#define SPACE_PACKET_SECONDARY_HEADER_FLAG_MASK 0x16
#define SPACE_PACKET_SEQUENCE_FLAGS_FIRST_MASK 0x01
#define SPACE_PACKET_SEQUENCE_FLAGS_SECOND_MASK 0x02

#define SPACE_PACKET_VERSION_NUMBER_OFFSET 0u
#define SPACE_PACKET_TYPE_OFFSET 3u
#define SPACE_PACKET_APID_HIGH_BITS_OFFSET 5u
#define SPACE_PACKET_SECONDARY_HEADER_FLAG_OFFSET 4u
#define SPACE_PACKET_SEQUENCE_FLAGS_FIRST_OFFSET 0u
#define SPACE_PACKET_SEQUENCE_FLAGS_SECOND_OFFSET 1u
#define SPACE_PACKET_SEQUENCE_CONTROL_HIGH_BITS_OFFSET 2u

#endif // SPACE_PACKET_H
