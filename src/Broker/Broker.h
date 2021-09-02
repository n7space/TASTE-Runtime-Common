#ifndef BROKER_H
#define BROKER_H

#include <system_spec.h>

#include <stddef.h>
#include <stdint.h>

#include <Packetizer.h>
#include <request_size.h>

#define BROKER_BUFFER_SIZE                                                                                             \
    (SPACE_PACKET_PRIMARY_HEADER_SIZE + GENERIC_LINUX_BUFFER_SIZE + SPACE_PACKET_ERROR_CONTROL_SIZE)

void Broker_initialize(void);
void Broker_deliver_request(const enum RemoteInterface interface, uint8_t* const data, const size_t size);
void Broker_receive_packet(uint8_t* const data, const size_t size);

typedef void (*driver_send_function)(void* driver_private_data, uint8_t* const data, const size_t length);
typedef void (*deliver_function)(uint8_t* const data, const size_t length);

#endif
