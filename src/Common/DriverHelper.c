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
