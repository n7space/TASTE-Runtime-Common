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

#include "DriverHelper.h"

const void*
get_device_configuration(const enum SystemDevice device_id)
{
    if((int)device_id > SYSTEM_DEVICE_NUMBER) {
        return (void*)0;
    }

    return device_configurations[(int)device_id];
}

const void*
get_remote_device_configuration(const enum SystemDevice device_id)
{
    if((int)device_id > SYSTEM_DEVICE_NUMBER) {
        return (void*)0;
    }
    const enum SystemBus bus_id = device_to_bus_map[(int)device_id];
    for(int index = 0; index < SYSTEM_DEVICE_NUMBER; ++index) {
        if(index != (int)device_id && device_to_bus_map[index] == bus_id) {
            return device_configurations[index];
        }
    }

    return (void*)0;
}
