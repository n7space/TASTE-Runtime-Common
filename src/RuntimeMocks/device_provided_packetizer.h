#include <system_spec.h>
#include <Packetizer.h>

size_t getDeviceProvidedPacketizerHeaderSize(const enum SystemBus bus_id);

packetizer_init_function getDeviceProvidedPacketizerInitFunction(const enum SystemBus bus_id);

packetizer_packetize_function getDeviceProvidedPacketizerPacketizeFunction(const enum SystemBus bus_id);

packetizer_depacketize_function getDeviceProvidedPacketizerDepacketizeFunction(const enum SystemBus bus_id);

void setDeviceProvidedPacketizerHeaderSize(const enum SystemBus bus_id, const size_t header_size);
