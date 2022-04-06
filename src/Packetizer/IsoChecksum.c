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

#include "IsoChecksum.h"

#define MAKE_UINT16(hi, lo) (uint16_t)(((hi) << 8u) | (lo))
#define REDUCE(x) (((x)&0xFFu) + ((x) >> 8u))
#define CHUNK_SIZE 20u

static uint16_t
fletcher16(const uint8_t* const data, const size_t length)
{
    uint32_t sum1 = 0xffu;
    uint32_t sum2 = 0xffu;
    size_t dataLength = length;
    const uint8_t* it = data;

    while(dataLength > 0u) {
        size_t chunkLength = (dataLength > CHUNK_SIZE) ? CHUNK_SIZE : dataLength;
        dataLength -= chunkLength;
        do {
            sum1 += *it++;
            sum2 += sum1;
        } while(--chunkLength > 0);
        sum1 = REDUCE(sum1);
        sum2 = REDUCE(sum2);
    }

    return MAKE_UINT16(REDUCE(sum2), REDUCE(sum1));
}

uint16_t
IsoChecksum_calculate(const uint8_t* const data, const size_t length)
{
    const uint16_t sum = fletcher16(data, length);

    const uint32_t f0 = sum & 0xFFu;
    const uint32_t f1 = (sum >> 8u) & 0xFFu;

    const uint32_t c0 = 0xFFu - ((f0 + f1) % 0xFFu);
    const uint32_t c1 = 0xFFu - ((f0 + c0) % 0xFFu);

    return MAKE_UINT16(c0, c1);
}
