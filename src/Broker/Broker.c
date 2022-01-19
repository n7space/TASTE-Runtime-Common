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

#include "Broker.h"

#include <string.h>
#include <stdint.h>

#include <Packetizer.h>

static Packetizer packetizer_data = { 0 };

__attribute__((section(".sdramMemorySection"))) static uint8_t packetizer_buffer[BROKER_BUFFER_SIZE];
extern driver_send_function bus_to_driver_send_function[SYSTEM_BUSES_NUMBER];
extern void* bus_to_driver_private_data[SYSTEM_BUSES_NUMBER];
extern deliver_function interface_to_deliver_function[INTERFACE_MAX_ID];

extern void Broker_acquire_lock();
extern void Broker_release_lock();

void
Broker_initialize()
{
    Packetizer_init(&packetizer_data);
}

void
Broker_deliver_request(const enum RemoteInterface interface, const uint8_t* const data, const size_t length)
{
    Broker_acquire_lock();
    memcpy(packetizer_buffer + SPACE_PACKET_PRIMARY_HEADER_SIZE, data, length);

#if defined BROKER_SEND_TELECOMMAND
    const Packetizer_PacketType packet_type = Packetizer_PacketType_Telecommand;
#elif defined BROKER_SEND_TELEMETRY
    const Packetizer_PacketType packet_type = Packetizer_PacketType_Telemetry;
#else
    const Packetizer_PacketType packet_type = Packetizer_PacketType_Telemetry;
#endif

    const size_t packet_size = Packetizer_packetize(&packetizer_data,
                                                    packet_type,
                                                    0,
                                                    (uint16_t)interface,
                                                    packetizer_buffer,
                                                    SPACE_PACKET_PRIMARY_HEADER_SIZE,
                                                    length);

    const enum SystemBus bus_id = port_to_bus_map[interface];
    void* driver_private_data = bus_to_driver_private_data[bus_id];
    driver_send_function send_function = bus_to_driver_send_function[bus_id];
    send_function(driver_private_data, packetizer_buffer, packet_size);

    Broker_release_lock();
}

void
Broker_receive_packet(uint8_t* const data, const size_t length)
{
    Broker_acquire_lock();
    uint16_t source;
    uint16_t destination;
    size_t data_offset;
    size_t data_size;
    int32_t error_code;

#if defined BROKER_EXPECT_TELECOMMAND
    const Packetizer_PacketType packet_type = Packetizer_PacketType_Telecommand;
#elif defined BROKER_EXPECT_TELEMETRT
    const Packetizer_PacketType packet_type = Packetizer_PacketType_Telemetry;
#else
    const Packetizer_PacketType packet_type = Packetizer_PacketType_Telemetry;
#endif

    const bool success = Packetizer_depacketize(
            &packetizer_data, packet_type, data, length, &source, &destination, &data_offset, &data_size, &error_code);
    if(!success) {
        Broker_release_lock();
        return;
    }

    deliver_function fn = interface_to_deliver_function[destination];
    fn(data + data_offset, data_size);

    Broker_release_lock();
}
