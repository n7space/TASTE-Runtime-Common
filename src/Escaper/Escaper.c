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

#include "Escaper.h"
#include "Broker.h"

#include <stdbool.h>
#include <stddef.h>

#define START_BYTE (uint8_t)0x00
#define STOP_BYTE (uint8_t)0xFF
#define ESCAPE_BYTE (uint8_t)0xFE

void
Escaper_parse_recv_buffer(Escaper * this, uint8_t* buffer, const size_t length)
{
    for(size_t i = 0; i < length; ++i) {
        switch(this->m_parse_state) {
            case STATE_WAIT:
                if(buffer[i] == START_BYTE) {
                    this->m_parse_state = STATE_DATA_BYTE;
                }
                break;
            case STATE_DATA_BYTE:
                if(buffer[i] == STOP_BYTE) {
                    Broker_receive_packet(this->m_message_buffer, this->m_message_buffer_index);
                    this->m_message_buffer_index = 0;
                    this->m_parse_state = STATE_WAIT;
                } else if(buffer[i] == ESCAPE_BYTE) {
                    this->m_parse_state = STATE_ESCAPE_BYTE;
                } else if(buffer[i] == START_BYTE) {
                    this->m_message_buffer_index = 0;
                    this->m_parse_state = STATE_DATA_BYTE;
                } else {
                    this->m_message_buffer[this->m_message_buffer_index] = buffer[i];
                    ++this->m_message_buffer_index;
                }
                break;
            case STATE_ESCAPE_BYTE:
                this->m_message_buffer[this->m_message_buffer_index] = buffer[i];
                ++this->m_message_buffer_index;
                this->m_parse_state = STATE_DATA_BYTE;
                break;
        }
    }
}
