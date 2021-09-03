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

#ifndef DRIVER_HELPER_H
#define DRIVER_HELPER_H

/**
 * @file DriverHelper.h
 * @brief Helper functions for runtime.
 *
 * Functions to operate on drivers configurations.
 */

#include <system_spec.h>

/** @brief Get device configuration.
 *
 * Returns device configuration for given device_id.
 *
 * @param[in]    device_id     Identifier of device.
 */
const void* get_device_configuration(const enum SystemDevice device_id);

/** @brief Get remote device configuration.
 *
 * Returns remote device configuration for given device_id.
 * This function returns the configuration of the remote device connected
 * with local device with identifier device_id. The local and remote devices
 * are connected with bus.
 *
 * @param[in]    device_id     Identifier of device.
 */
const void* get_remote_device_configuration(const enum SystemDevice device_id);

#endif
