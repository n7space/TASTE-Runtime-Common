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

#ifndef ESCAPER_H
#define ESCAPER_H

#include <stdint.h>

/// @brief Enumeration listing possible parser states
enum Escaper_PacketParseState
{
    STATE_WAIT,
    STATE_DATA_BYTE,
    STATE_ESCAPE_BYTE,
};

/// @brief Structure representing escaper
typedef struct
{
    enum Escaper_PacketParseState m_parse_state;
    bool m_encode_started;
    bool m_escape;
	bool m_encode_finished;
	uint8_t m_message_buffer[BROKER_BUFFER_SIZE];
    size_t m_message_buffer_index;
} Escaper;

/**
 * @file    Escaper.h
 * @brief   Main header file for escaping mode
 *
 */
void
Escaper_parse_recv_buffer(Escaper * this, uint8_t* buffer, const size_t length);

#endif
