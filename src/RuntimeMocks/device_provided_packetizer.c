#include <device_provided_packetizer.h>

size_t
getDeviceProvidedPacketizerHeaderSize(const enum SystemBus bus_id)
{
    (void)bus_id;

    return 0;
}

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

void
setDeviceProvidedPacketizerHeaderSize(const enum SystemBus bus_id, const size_t header_size)
{
    (void)bus_id;
    (void)header_size;
}
