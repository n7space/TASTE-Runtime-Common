#include <device_provided_packetizer.h>

packetizer_init_function
getDeviceProvidedPacketizerInitFunction(const enum SystemBus bus_id)
{
    (void)bus_id;

    return NULL;
}

packetizer_packetize_function
getDeviceProvidedPacketizerPacketizeFunction(const enum SystemBus bus_id)
{
    (void)bus_id;

    return NULL;
}

packetizer_depacketize_function
getDeviceProvidedPacketizerDepacketizeFunction(const enum SystemBus bus_id)
{
    (void)bus_id;

    return NULL;
}
