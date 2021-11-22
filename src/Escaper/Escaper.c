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

void
Escaper_parse_recv_buffer(Escaper* const self, uint8_t* buffer, const size_t length)
{
    for(size_t i = 0; i < length; ++i) {
        switch(self->m_parse_state) {
            case STATE_WAIT:
                if(buffer[i] == START_BYTE) {
                    self->m_parse_state = STATE_DATA_BYTE;
                }
                break;
            case STATE_DATA_BYTE:
                if(buffer[i] == STOP_BYTE) {
                    Broker_receive_packet(self->m_recv_packet_buffer, self->m_recv_packet_buffer_index);
                    self->m_recv_packet_buffer_index = 0;
                    self->m_parse_state = STATE_WAIT;
                } else if(buffer[i] == ESCAPE_BYTE) {
                    self->m_parse_state = STATE_ESCAPE_BYTE;
                } else if(buffer[i] == START_BYTE) {
                    self->m_recv_packet_buffer_index = 0;
                    self->m_parse_state = STATE_DATA_BYTE;
                } else {
                    self->m_recv_packet_buffer[self->m_recv_packet_buffer_index] = buffer[i];
                    ++self->m_recv_packet_buffer_index;
                }
                break;
            case STATE_ESCAPE_BYTE:
                self->m_recv_packet_buffer[self->m_recv_packet_buffer_index] = buffer[i];
                ++self->m_recv_packet_buffer_index;
                self->m_parse_state = STATE_DATA_BYTE;
                break;
        }
    }
}

void
Escaper_init_parser(Escaper* const self)
{
    self->m_parse_state = STATE_WAIT;
}

void
Escaper_init_encode(Escaper* const self)
{
    self->m_encode_finished = false;
    self->m_escape = false;
    self->m_encode_started = false;
}

size_t
Escaper_encode_packet(Escaper* const self, const uint8_t* const data, const size_t length, size_t* const index)
{
    size_t send_buffer_index = 0;

    if(!self->m_encode_started) {
        self->m_send_packet_buffer[0] = START_BYTE;
        ++send_buffer_index;
        self->m_encode_started = true;
    }

    while(*index < length) {
        if(self->m_escape) {
            self->m_send_packet_buffer[send_buffer_index] = data[*index];
            ++send_buffer_index;
            ++*index;
            self->m_escape = false;
        } else if(data[*index] == START_BYTE) {
            self->m_send_packet_buffer[send_buffer_index] = ESCAPE_BYTE;
            ++send_buffer_index;
            self->m_escape = true;
        } else if(data[*index] == ESCAPE_BYTE) {
            self->m_send_packet_buffer[send_buffer_index] = ESCAPE_BYTE;
            ++send_buffer_index;
            self->m_escape = true;
        } else if(data[*index] == STOP_BYTE) {
            self->m_send_packet_buffer[send_buffer_index] = ESCAPE_BYTE;
            ++send_buffer_index;
            self->m_escape = true;
        } else {
            self->m_send_packet_buffer[send_buffer_index] = data[*index];
            ++send_buffer_index;
            ++*index;
        }

        if(send_buffer_index == PACKET_MAX_SIZE) {
            return send_buffer_index;
        }
    }

    if(*index == length) {
        self->m_send_packet_buffer[send_buffer_index] = STOP_BYTE;
        ++send_buffer_index;
        self->m_encode_finished = true;
    }

    return send_buffer_index;
}
