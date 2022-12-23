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

#ifndef PACKETIZER_ISOCHECKSUM_H
#define PACKETIZER_ISOCHECKSUM_H

#include <stddef.h>
#include <stdint.h>

/**
 * @brief   Calculates ISO checksum for given data block.
 *          Checksum is calculated according to as ECSS-E-70-41A Annex A.
 *
 * @param[in] data     Pointer to start of data block. Can't be NULL.
 * @param[in] length   Data block length in bytes.
 *
 * @return  Calculated checksum.
 */
uint16_t IsoChecksum_calculate(const uint8_t* const data, const size_t length);

#endif // PACKETIZER_ISOCHECKSUM_H
