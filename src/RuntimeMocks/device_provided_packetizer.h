#include <system_spec.h>
#include <Packetizer.h>

packetizer_init_function getDeviceProvidedPacketizerInitFunction(const enum SystemBus bus_id);

packetizer_packetize_function getDeviceProvidedPacketizerPacketizeFunction(const enum SystemBus bus_id);

packetizer_depacketize_function getDeviceProvidedPacketizerDepacketizeFunction(const enum SystemBus bus_id);
