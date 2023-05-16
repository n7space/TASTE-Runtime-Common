#include <system_spec.h>
#include <Packetizer.h>

extern packetizer_init_function bus_to_device_provided_packetizer_init_function[SYSTEM_BUSES_NUMBER];
extern packetizer_packetize_function bus_to_device_provided_packetizer_packetize_function[SYSTEM_BUSES_NUMBER];
extern packetizer_depacketize_function bus_to_device_provided_packetizer_depacketize_function[SYSTEM_BUSES_NUMBER];

void resetDeviceProvidedPacketizerFunctions();

packetizer_init_function getDeviceProvidedPacketizerInitFunction(const enum SystemBus bus_id);

packetizer_packetize_function getDeviceProvidedPacketizerPacketizeFunction(const enum SystemBus bus_id);

packetizer_depacketize_function getDeviceProvidedPacketizerDepacketizeFunction(const enum SystemBus bus_id);

void setDeviceProvidedPacketizerFunctions(const enum SystemBus bus_id,
                                          packetizer_init_function init_function,
                                          packetizer_packetize_function packetize_function,
                                          packetizer_depacketize_function depacketize_function);
