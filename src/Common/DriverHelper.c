#include "DriverHelper.h"

void*
get_device_configuration(enum SystemDevice device_id)
{
    if(device_id > SYSTEM_DEVICE_NUMBER) {
        return (void*)0;
    }

    return device_configurations[device_id];
}

void*
get_remote_device_configuration(enum SystemDevice device_id)
{
    if(device_id > SYSTEM_DEVICE_NUMBER) {
        return 0;
    }
    enum SystemBus bus_id = device_to_bus_map[device_id];
    for(int index = 0; index < SYSTEM_DEVICE_NUMBER; ++index) {
        if(index != device_id) {
            if(device_to_bus_map[index] == bus_id) {
                return device_configurations[index];
            }
        }
    }

    return 0;
}
