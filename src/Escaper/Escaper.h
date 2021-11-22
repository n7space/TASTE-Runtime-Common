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

#include <Broker.h>
#include "EscaperInternal.h"

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
    uint8_t m_recv_packet_buffer[BROKER_BUFFER_SIZE];
    size_t m_recv_packet_buffer_index;
    uint8_t m_send_packet_buffer[PACKET_MAX_SIZE];
} Escaper;

typedef void (*Receive_packet)(uint8_t* const, const size_t);

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
void Escaper_decode_packet(Escaper* const self, uint8_t* buffer, const size_t length, Receive_packet receivePacket);

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
 * @param[in]   self    		Pointer to a structure representing Escaper
 * @param[in]	data			Pointer to array of data to be sent
 * @param[in]	length			Length of data to be sent
 * @param[in]	index			Pointer to variable holding index of next byte
 *								to parse
 * @param[out]	packetLength    Length of packet to be sent
 */
size_t Escaper_encode_packet(Escaper* const self, const uint8_t* const data, const size_t length, size_t* const index);

#endif
