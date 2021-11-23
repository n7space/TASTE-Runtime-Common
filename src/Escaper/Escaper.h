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

/**
 * @file    Escaper.h
 * @brief   Main header file for escaping mode
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/// @brief Enumeration listing possible parser states
typedef enum
{
    Escaper_State_Wait,
    Escaper_State_Data_Byte,
    Escaper_State_Escape_Byte,
} Escaper_PacketParseState;

/// @brief Structure representing escaper
typedef struct
{
    Escaper_PacketParseState m_parse_state;
    bool m_encode_started;
    bool m_escape;
    bool m_encode_finished;
    uint8_t* m_encoded_packet_buffer;
    size_t m_encoded_packet_max_size;
    uint8_t* m_decoded_packet_buffer;
    size_t m_decoded_packet_max_size;
    size_t m_decoded_packet_buffer_index;
} Escaper;

/** @brief Function pointer to function responsible for packet reception
 *
 * @param[in]   Pointer to received packet data buffer
 * @param[in]   Size of the received packet data buffer
 */
typedef void (*Receive_packet_fn)(uint8_t* const, const size_t);

/** @brief Initialize structure representing escaper
 *
 * @param[in]   self                            Pointer to a structure representing Escaper
 * @param[in]   m_encoded_packet_buffer         Pointer to a buffer that holds encoded packets
 * @param[in]   m_encoded_packet_buffer_size    Length of encoded packet buffer
 * @param[in]   m_decoded_packet_buffer         Pointer to a buffer that holds decoded packets
 * @param[in]   m_decoded_packet_buffer_size    Length of decoded packet buffer
 */
void Escaper_init(Escaper* const self,
                  uint8_t* m_encoded_packet_buffer,
                  size_t m_encoded_packet_buffer_size,
                  uint8_t* m_decoded_packet_buffer,
                  size_t m_decoded_packet_buffer_size);

/** @brief Initialize decoder
 *
 * This function shall be used in the polling method
 * before polling loop.
 *
 * @param[in]   self    Pointer to a structure representing Escaper
 */
void Escaper_start_decoder(Escaper* const self);

/** @brief Decode received message
 *
 * This function shall be used in polling loop in order to
 * decode received data.
 *
 * @param[in]   self            Pointer to a structure representing Escaper
 * @param[in]   buffer          Pointer to a buffer that holds received data
 * @param[in]   length          Length of received data buffer
 * @param[in]   receivePacket   Pointer to a function that handles packet reception
 */
void Escaper_decode_packet(Escaper* const self,
                           uint8_t* buffer,
                           const size_t length,
                           Receive_packet_fn receivePacketFn);

/** @brief Initialize packer encoder
 *
 * This function shall be used in the sender method
 * before calling Escaper_encode_packet
 *
 * @param[in]   self    Pointer to a structure representing Escaper
 */
void Escaper_start_encoder(Escaper* const self);

/** @brief Encode packet
 *
 * This function shall be used in the send method inside the sending loop.
 * This method puts encoded data into self->m_send_buffer array. This
 * array together with the returned packetLength parameter can be passed to device
 * specific sending method.
 *
 * @param[in]       self            Pointer to a structure representing Escaper
 * @param[in]       data            Pointer to array of data to be sent
 * @param[in]       length          Length of data to be sent
 * @param[in,out]   index           Pointer to variable holding index of next byte to encode.
 *                                  If index == length, then the whole data bufer was encoded.
 *
 * @returns	Length of packet to be sent.
 */
size_t Escaper_encode_packet(Escaper* const self, const uint8_t* const data, const size_t length, size_t* const index);

#endif
