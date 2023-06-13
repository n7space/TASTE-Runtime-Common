/**@file
 * This file is part of the TASTE Runtime Common.
 *
 * @copyright 2023 N7 Space Sp. z o.o.
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

#ifndef ROUTING_HELPER_HPP
#define ROUTING_HELPER_HPP

/**
 * @file RoutingHelper.h
 * @brief Helper functions for runtime.
 *
 * Functions to operate on system spec tables.
 */

#include <system_spec.h>

/** @brief Find unique destination interface for given bus.
 *
 * @param[bus] system bus
 * @return unique provided interface connected to given bus or INVALID_INTERFACE_ID.
 */
enum RemoteInterface find_unique_destination(const enum SystemBus bus);

#endif
