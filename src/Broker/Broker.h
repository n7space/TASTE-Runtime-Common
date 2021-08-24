#ifndef BROKER_H
#define BROKER_H

#include <system_spec.h>

#include <stddef.h>
#include <stdint.h>

void Broker_initialize(void);
void Broker_deliver_request(enum RemoteInterface interface, uint8_t* data, size_t size);
void Broker_receive_packet(uint8_t* data, size_t size);

typedef void (*driver_send_function)(void* driver_private_data, uint8_t* data, size_t length);
typedef void (*deliver_function)(uint8_t* data, size_t length);

#endif
