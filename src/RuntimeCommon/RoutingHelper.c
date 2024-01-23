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

#include "RoutingHelper.h"

#include <stddef.h>

#include "routing.h"

enum RemoteInterface
find_unique_destination(const enum SystemBus bus)
{
    size_t index = 0;
    size_t found_interface = INTERFACE_INVALID_ID;
    for(index = 0; index < INTERFACE_MAX_ID; ++index) {
        if(port_to_partition_bus_map[index].partition != PARTITION_NAME
           && port_to_partition_bus_map[index].bus == bus) {
            if(found_interface != INTERFACE_INVALID_ID) {
                return INTERFACE_INVALID_ID;
            } else {
                found_interface = index;
            }
        }
    }

    return (enum RemoteInterface)found_interface;
}

int
check_bus_has_any_destination_port(const enum SystemBus bus)
{
    size_t index;
    int result = 0;
    for(index = 0; index < INTERFACE_MAX_ID; ++index) {
        if(port_to_partition_bus_map[index].partition != PARTITION_NAME
           && port_to_partition_bus_map[index].bus == bus) {
            result = 1;
        }
    }

    return result;
}
