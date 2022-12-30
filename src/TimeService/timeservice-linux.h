#ifndef TIMESERVICE_LINUX_H
#define TIMESERVICE_LINUX_H

#include "timeservice-utils.h"

#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define ADJUST_ENDIANESS(x)

static uint64_t get_time()
{
   struct timespec time_spec;
   clock_gettime(CLOCK_REALTIME, &time_spec);
   uint64_t nanoseconds = time_spec.tv_sec * NS_PER_SECOND + time_spec.tv_nsec;
   return nanoseconds;
}

#endif
