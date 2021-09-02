#ifndef DRIVER_HELPER_H
#define DRIVER_HELPER_H

#include <system_spec.h>

const void* get_device_configuration(const enum SystemDevice device_id);
const void* get_remote_device_configuration(const enum SystemDevice device_id);

#endif
