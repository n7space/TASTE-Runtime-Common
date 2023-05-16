#include <device_provided_packetizer.h>

packetizer_init_function bus_to_device_provided_packetizer_init_function[SYSTEM_BUSES_NUMBER];
packetizer_packetize_function bus_to_device_provided_packetizer_packetize_function[SYSTEM_BUSES_NUMBER];
packetizer_depacketize_function bus_to_device_provided_packetizer_depacketize_function[SYSTEM_BUSES_NUMBER];

void
resetDeviceProvidedPacketizerFunctions()
{
    bus_to_device_provided_packetizer_init_function[BUS_1] = NULL;
    bus_to_device_provided_packetizer_packetize_function[BUS_1] = NULL;
    bus_to_device_provided_packetizer_depacketize_function[BUS_1] = NULL;
}

packetizer_init_function
getDeviceProvidedPacketizerInitFunction(const enum SystemBus bus_id)
{
    if(bus_id > SYSTEM_BUSES_NUMBER) {
        return NULL;
    }

    return bus_to_device_provided_packetizer_init_function[bus_id];
}

packetizer_packetize_function
getDeviceProvidedPacketizerPacketizeFunction(const enum SystemBus bus_id)
{
    if(bus_id > SYSTEM_BUSES_NUMBER) {
        return NULL;
    }

    return bus_to_device_provided_packetizer_packetize_function[bus_id];
}

packetizer_depacketize_function
getDeviceProvidedPacketizerDepacketizeFunction(const enum SystemBus bus_id)
{
    if(bus_id > SYSTEM_BUSES_NUMBER) {
        return NULL;
    }

    return bus_to_device_provided_packetizer_depacketize_function[bus_id];
}

void
setDeviceProvidedPacketizerFunctions(const enum SystemBus bus_id,
                                     packetizer_init_function init_function,
                                     packetizer_packetize_function packetize_function,
                                     packetizer_depacketize_function depacketize_function)
{
    if(bus_id > SYSTEM_BUSES_NUMBER) {
        return;
    }

    bus_to_device_provided_packetizer_init_function[bus_id] = init_function;
    bus_to_device_provided_packetizer_packetize_function[bus_id] = packetize_function;
    bus_to_device_provided_packetizer_depacketize_function[bus_id] = depacketize_function;
}
