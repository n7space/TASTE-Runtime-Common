#include "Broker.h"

#include <string.h>
#include <stdint.h>

static Packetizer packetizer_data = { 0 };
static uint8_t packetizer_buffer[BROKER_BUFFER_SIZE];

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
Broker_deliver_request(enum RemoteInterface interface, uint8_t* const data, const size_t length)
{
    Broker_acquire_lock();
    memcpy(packetizer_buffer + SPACE_PACKET_PRIMARY_HEADER_SIZE, data, length);

    size_t packet_size = Packetizer_packetize(&packetizer_data,
                                              Packetizer_PacketType_Telemetry,
                                              0,
                                              (uint16_t)interface,
                                              packetizer_buffer,
                                              SPACE_PACKET_PRIMARY_HEADER_SIZE,
                                              length);

    // sent to driver
    const enum SystemBus bus_id = port_to_bus_map[interface];
    void* driver_private_data = bus_to_driver_private_data[bus_id];
    driver_send_function send_function = bus_to_driver_send_function[bus_id];
    send_function(driver_private_data, packetizer_buffer, packet_size);

    Broker_release_lock();
}

// this shall be called by driver
void
Broker_receive_packet(uint8_t* const data, const size_t size)
{
    Broker_acquire_lock();
    // mutex
    uint16_t source;
    uint16_t destination;
    size_t data_offset;
    size_t data_size;
    int32_t error_code;
    const bool success = Packetizer_depacketize(&packetizer_data,
                                                Packetizer_PacketType_Telemetry,
                                                data,
                                                size,
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
    fn(data + data_offset, data_size);

    Broker_release_lock();
}
