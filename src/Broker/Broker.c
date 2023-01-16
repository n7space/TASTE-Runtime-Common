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
#include <ThinPacketizer.h>
#include <CCSDSPacketizer.h>
#include <DriverHelper.h>

static Packetizer packetizers_data[SYSTEM_BUSES_NUMBER] = { 0 };
static PacketizerFunctions packetizers_functions[PACKETIZER_MAX_ID];

__attribute__((section(".sdramMemorySection"))) static uint8_t packetizer_buffer[BROKER_BUFFER_SIZE];
extern driver_send_function bus_to_driver_send_function[SYSTEM_BUSES_NUMBER];
extern void* bus_to_driver_private_data[SYSTEM_BUSES_NUMBER];
extern enum PacketizerCfg bus_to_packetizer_cfg[SYSTEM_BUSES_NUMBER];
extern deliver_function interface_to_deliver_function[INTERFACE_MAX_ID];

extern void Broker_acquire_lock();
extern void Broker_release_lock();

void
Broker_initialize_packetizers_functions()
{
    packetizers_functions[PACKETIZER_DEFAULT].headerSize = SPACE_PACKET_PRIMARY_HEADER_SIZE;
    packetizers_functions[PACKETIZER_DEFAULT].init = &Packetizer_init;
    packetizers_functions[PACKETIZER_DEFAULT].packetize = &Packetizer_packetize;
    packetizers_functions[PACKETIZER_DEFAULT].depacketize = &Packetizer_depacketize;
    packetizers_functions[PACKETIZER_CCSDS].headerSize = SPACE_PACKET_PRIMARY_HEADER_SIZE;
    packetizers_functions[PACKETIZER_CCSDS].init = &Packetizer_init;
    packetizers_functions[PACKETIZER_CCSDS].packetize = &Packetizer_packetize;
    packetizers_functions[PACKETIZER_CCSDS].depacketize = &Packetizer_depacketize;
    packetizers_functions[PACKETIZER_STRICT_CCSDS].headerSize = SPACE_PACKET_PRIMARY_HEADER_SIZE;
    packetizers_functions[PACKETIZER_STRICT_CCSDS].init = &CCSDS_Packetizer_init;
    packetizers_functions[PACKETIZER_STRICT_CCSDS].packetize = &CCSDS_Packetizer_packetize;
    packetizers_functions[PACKETIZER_STRICT_CCSDS].depacketize = &CCSDS_Packetizer_depacketize;
    packetizers_functions[PACKETIZER_THIN].headerSize = THIN_PACKET_PRIMARY_HEADER_SIZE;
    packetizers_functions[PACKETIZER_THIN].init = &ThinPacketizer_init;
    packetizers_functions[PACKETIZER_THIN].packetize = &ThinPacketizer_packetize;
    packetizers_functions[PACKETIZER_THIN].depacketize = &ThinPacketizer_depacketize;
}

void
Broker_initialize(enum SystemBus valid_buses[SYSTEM_BUSES_NUMBER])
{
    Broker_initialize_packetizers_functions();

    for(int i = 0; i < SYSTEM_BUSES_NUMBER; ++i) {
        enum SystemBus bus_id = valid_buses[i];
        if(bus_id == BUS_INVALID_ID) {
            // Input array (valid_buses) is initialized with BUS_INVALID_ID. When we put actual values into the array,
            // they are different than BUS_INVALID_ID. Therefore, when we meet first BUS_INVALID_ID, it means that
            // there are no more actual values in the array.
            break;
        }

        if(bus_id < SYSTEM_BUSES_NUMBER) {
            // It is a fix for gcc's false positive warning. The index 'bus_id' is retrieved from another array and
            // gcc is not sure if the value is outside array 'bus_to_packetizer_cfg' bounds.
            // https://gcc.gnu.org/bugzilla/show_bug.cgi?id=85398
            enum PacketizerCfg packetizer_type = bus_to_packetizer_cfg[bus_id];
            packetizer_init_function packetizer_init = packetizers_functions[packetizer_type].init;
            packetizer_init(&packetizers_data[bus_id]);
        }
    }
}

#if defined GENERIC_LINUX_TARGET || defined RTEMS6_TARGET
void
Broker_deliver_request(const enum RemoteInterface interface,
                       const asn1SccPID senderPid,
                       const uint8_t* const data,
                       const size_t length)
#else
void
Broker_deliver_request(const enum RemoteInterface interface, const uint8_t* const data, const size_t length)
#endif
{
    Broker_acquire_lock();

#if defined BROKER_SEND_TELECOMMAND
    const Packetizer_PacketType packet_type = Packetizer_PacketType_Telecommand;
#elif defined BROKER_SEND_TELEMETRY
    const Packetizer_PacketType packet_type = Packetizer_PacketType_Telemetry;
#else
    const Packetizer_PacketType packet_type = Packetizer_PacketType_Telemetry;
#endif

    const enum SystemBus bus_id = port_to_bus_map[interface];
    enum PacketizerCfg packetizer_type = bus_to_packetizer_cfg[bus_id];
    unsigned header_size = packetizers_functions[packetizer_type].headerSize;
    memcpy(packetizer_buffer + header_size, data, length);

    packetizer_packetize_function packetizer_packetize = packetizers_functions[packetizer_type].packetize;

#if defined GENERIC_LINUX_TARGET || defined RTEMS6_TARGET
    const size_t packet_size = packetizer_packetize(&packetizers_data[bus_id],
                                                    packet_type,
                                                    (uint16_t)senderPid,
                                                    (uint16_t)interface,
                                                    packetizer_buffer,
                                                    header_size,
                                                    length);
#else
    const size_t packet_size = packetizer_packetize(
            &packetizers_data[bus_id], packet_type, 0, (uint16_t)interface, packetizer_buffer, header_size, length);
#endif

    void* driver_private_data = bus_to_driver_private_data[bus_id];
    driver_send_function send_function = bus_to_driver_send_function[bus_id];
    send_function(driver_private_data, packetizer_buffer, packet_size);

    Broker_release_lock();
}

void
Broker_receive_packet(enum SystemBus bus_id, uint8_t* const data, const size_t length)
{
    Broker_acquire_lock();
    uint16_t source;
    uint16_t destination;
    size_t data_offset;
    size_t data_size;
    int32_t error_code;

#if defined BROKER_EXPECT_TELECOMMAND
    const Packetizer_PacketType packet_type = Packetizer_PacketType_Telecommand;
#elif defined BROKER_EXPECT_TELEMETRY
    const Packetizer_PacketType packet_type = Packetizer_PacketType_Telemetry;
#else
    const Packetizer_PacketType packet_type = Packetizer_PacketType_Telemetry;
#endif

    enum PacketizerCfg packetizer_type = bus_to_packetizer_cfg[bus_id];
    packetizer_depacketize_function packetizer_depacketize = packetizers_functions[packetizer_type].depacketize;
    const bool success = packetizer_depacketize(&packetizers_data[bus_id],
                                                packet_type,
                                                data,
                                                length,
                                                &source,
                                                &destination,
                                                &data_offset,
                                                &data_size,
                                                &error_code);
    if(!success) {
        Broker_release_lock();
        return;
    }

    deliver_function fn = interface_to_deliver_function[destination];

#if defined GENERIC_LINUX_TARGET || defined RTEMS6_TARGET
    fn((asn1SccPID)source, data + data_offset, data_size);
#else
    fn(data + data_offset, data_size);
#endif

    Broker_release_lock();
}
