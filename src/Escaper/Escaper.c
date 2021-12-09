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
#include "EscaperInternal.h"

#include <assert.h>

void
Escaper_init(Escaper* const self,
             uint8_t* m_encoded_packet_buffer,
             size_t m_encoded_packet_buffer_size,
             uint8_t* m_decoded_packet_buffer,
             size_t m_decoded_packet_buffer_size)
{
    self->m_parse_state = Escaper_State_Wait;
    self->m_encode_started = false;
    self->m_escape = false;
    self->m_encode_finished = false;
    self->m_encoded_packet_buffer = m_encoded_packet_buffer;
    self->m_encoded_packet_max_size = m_encoded_packet_buffer_size;
    self->m_decoded_packet_buffer = m_decoded_packet_buffer;
    self->m_decoded_packet_max_size = m_decoded_packet_buffer_size;
    self->m_decoded_packet_buffer_index = 0;
}

void
Escaper_start_decoder(Escaper* const self)
{
    self->m_parse_state = Escaper_State_Wait;
}

void
Escaper_decode_packet(Escaper* const self, uint8_t* buffer, const size_t length, Receive_packet_fn receivePacketFn)
{
    for(size_t i = 0; i < length; ++i) {
        switch(self->m_parse_state) {
            case Escaper_State_Wait:
                if(buffer[i] == START_BYTE) {
                    self->m_parse_state = Escaper_State_Data_Byte;
                }
                break;
            case Escaper_State_Data_Byte:
                if(buffer[i] == STOP_BYTE) {
                    receivePacketFn(self->m_decoded_packet_buffer, self->m_decoded_packet_buffer_index);
                    self->m_decoded_packet_buffer_index = 0;
                    self->m_parse_state = Escaper_State_Wait;
                } else if(buffer[i] == ESCAPE_BYTE) {
                    self->m_parse_state = Escaper_State_Escape_Byte;
                } else if(buffer[i] == START_BYTE) {
                    self->m_decoded_packet_buffer_index = 0;
                    self->m_parse_state = Escaper_State_Data_Byte;
                } else {
                    if(self->m_decoded_packet_buffer_index >= self->m_decoded_packet_max_size) {
                        // buffer overflow prevented
                        // the current packet cannot be delivered
                        // reset escaper to search for new START_BYTE
                        self->m_decoded_packet_buffer_index = 0;
                        self->m_parse_state = Escaper_State_Wait;
                    } else {
                        self->m_decoded_packet_buffer[self->m_decoded_packet_buffer_index] = buffer[i];
                        ++self->m_decoded_packet_buffer_index;
                    }
                }
                break;
            case Escaper_State_Escape_Byte:
                if(self->m_decoded_packet_buffer_index >= self->m_decoded_packet_max_size) {
                    // buffer overflow prevented
                    // the current packet cannot be delivered
                    // reset escaper to search for new START_BYTE
                    self->m_decoded_packet_buffer_index = 0;
                    self->m_parse_state = Escaper_State_Wait;
                } else {
                    self->m_decoded_packet_buffer[self->m_decoded_packet_buffer_index] = buffer[i];
                    ++self->m_decoded_packet_buffer_index;
                    self->m_parse_state = Escaper_State_Data_Byte;
                }
                break;
            default:
                assert(false && "Unknown parser state");
                break;
        }
    }
}

void
Escaper_start_encoder(Escaper* const self)
{
    self->m_encode_finished = false;
    self->m_escape = false;
    self->m_encode_started = false;
}

size_t
Escaper_encode_packet(Escaper* const self, const uint8_t* const data, const size_t length, size_t* const index)
{
    size_t encoded_packet_buffer_index = 0;

    if(!self->m_encode_started) {
        self->m_encoded_packet_buffer[0] = START_BYTE;
        ++encoded_packet_buffer_index;
        self->m_encode_started = true;
    }

    while(*index < length) {
        if(self->m_escape) {
            self->m_encoded_packet_buffer[encoded_packet_buffer_index] = data[*index];
            ++encoded_packet_buffer_index;
            ++*index;
            self->m_escape = false;
        } else if(data[*index] == START_BYTE) {
            self->m_encoded_packet_buffer[encoded_packet_buffer_index] = ESCAPE_BYTE;
            ++encoded_packet_buffer_index;
            self->m_escape = true;
        } else if(data[*index] == ESCAPE_BYTE) {
            self->m_encoded_packet_buffer[encoded_packet_buffer_index] = ESCAPE_BYTE;
            ++encoded_packet_buffer_index;
            self->m_escape = true;
        } else if(data[*index] == STOP_BYTE) {
            self->m_encoded_packet_buffer[encoded_packet_buffer_index] = ESCAPE_BYTE;
            ++encoded_packet_buffer_index;
            self->m_escape = true;
        } else {
            self->m_encoded_packet_buffer[encoded_packet_buffer_index] = data[*index];
            ++encoded_packet_buffer_index;
            ++*index;
        }

        if(encoded_packet_buffer_index == self->m_encoded_packet_max_size) {
            return encoded_packet_buffer_index;
        }
    }

    if(*index == length) {
        self->m_encoded_packet_buffer[encoded_packet_buffer_index] = STOP_BYTE;
        ++encoded_packet_buffer_index;
        self->m_encode_finished = true;
    }

    return encoded_packet_buffer_index;
}
