#include "system_spec.h"

enum SystemBus port_to_bus_map[] = {
    BUS_INVALID_ID,
};

enum RemoteInterface bus_to_port_map[] = {
    INTERFACE_INVALID_ID,
};

enum SystemBus device_to_bus_map[SYSTEM_DEVICE_NUMBER] = { 0 };

void* device_configurations[SYSTEM_DEVICE_NUMBER] = { 0 };
