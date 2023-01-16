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

#ifndef BROKER_H
#define BROKER_H

/**
 * @file    Broker.h
 * @brief   Broker - message passing between nodes.
 *
 * The Broker is used by TASTE runtime to transport messages between nodes.
 *
 * The Runtime shall provide following functions:
 * - void Broker_acquire_lock() - which locks the mutex designated to Broker
 * - void Broker_release_lock() - which unlocks the mutex designated to Broker
 *
 * The Runtime shall provide following global variables:
 * - driver_send_function bus_to_driver_send_function[SYSTEM_BUSES_NUMBER];
 * - void* bus_to_driver_private_data[SYSTEM_BUSES_NUMBER];
 * - deliver_function interface_to_deliver_function[INTERFACE_MAX_ID];
 *
 */

#include <stddef.h>
#include <stdint.h>

#include <SpacePacket.h>

#include <system_spec.h>
#include <request_size.h>
#include <dataview-uniq.h>

/// @brief  Buffer size to store space packet data.
#ifdef STANDARD_SPACE_PACKET
#define BROKER_BUFFER_SIZE                                                                                             \
    (SPACE_PACKET_PRIMARY_HEADER_SIZE + GENERIC_PARTITION_BUFFER_SIZE + SPACE_PACKET_ERROR_CONTROL_SIZE)
#else
#define BROKER_BUFFER_SIZE                                                                                             \
    (SPACE_PACKET_PRIMARY_HEADER_SIZE + GENERIC_PARTITION_BUFFER_SIZE + SPACE_PACKET_SENDER_PID_SIZE                   \
     + SPACE_PACKET_ERROR_CONTROL_SIZE)
#endif

typedef enum
{
    Broker_ErrorType_UnknownError,
    Broker_ErrorType_IncorrectCrc16,
    Broker_ErrorType_IncorrectPacketType,
    Broker_ErrorType_IncorrectPacketSize,
} Broker_ErrorType;

/** @brief Initialize Broker
 *
 * Broker shall be initialized before starting system's threads.
 *
 * @param[in]    valid_buses   An array of busses used in the broker instance
 */
void Broker_initialize(enum SystemBus valid_buses[SYSTEM_BUSES_NUMBER]);

/** @brief Deliver message to remote interface
 *
 * This function shall be used to deliver a message (sporadic interface call)
 * to remote partition. This function is used in code generated using kazoo.
 *
 * @param[in]    interface      Target remote interface
 * @param[in]    senderPid      Source function PID
 * @param[in]    data           Pointer to an array with the data
 * @param[in]    length         Size of the data
 */
#if defined GENERIC_LINUX_TARGET || defined RTEMS6_TARGET
void Broker_deliver_request(const enum RemoteInterface interface,
                            const asn1SccPID senderPid,
                            const uint8_t* const data,
                            const size_t length);
#else
void Broker_deliver_request(const enum RemoteInterface interface, const uint8_t* const data, const size_t length);
#endif

/** @brief Deliver message from remote interface
 *
 * This function shall be used to deliver a message (sporadic interface call)
 * from remote partition. This function is used by device drivers.
 *
 * @param[in]    bus_id         Identifier of the bus used in communication
 * @param[in]    data           Pointer to an array with the data
 * @param[in]    length         Size of the data
 */
void Broker_receive_packet(enum SystemBus bus_id, uint8_t* const data, const size_t length);

typedef void (*broker_error_detected)(const Broker_ErrorType);

void Broker_reigster_error_callback(broker_error_detected callback);

/** @brief The signature of driver deliver function
 *
 * The device drivers shall provide a function with this signature.
 */
typedef void (*driver_send_function)(void* driver_private_data, const uint8_t* const data, const size_t length);

/** @brief The signature of interface deliver function
 *
 * The code generated by kazoo shall provide a functions with this signature
 * for all sporadic interfaces connected with remote partitions.
 */
#if defined GENERIC_LINUX_TARGET || defined RTEMS6_TARGET
typedef void (*deliver_function)(const asn1SccPID sender_pid, const uint8_t* const data, const size_t length);
#else
typedef void (*deliver_function)(const uint8_t* const data, const size_t length);
#endif

#endif
