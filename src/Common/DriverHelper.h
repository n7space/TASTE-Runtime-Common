#ifndef DRIVER_HELPER_H
#define DRIVER_HELPER_H

#include <system_spec.h>

void* get_device_configuration(enum SystemDevice device_id);
void* get_remote_device_configuration(enum SystemDevice device_id);

#endif
