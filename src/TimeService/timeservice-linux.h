#ifndef TIMESERVICE_LINUX_H
#define TIMESERVICE_LINUX_H

#include "timeservice-utils.h"

#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

static uint64_t get_time()
{
   struct timespec time_spec;
   clock_gettime(CLOCK_REALTIME, &time_spec);
   uint64_t nanoseconds = time_spec.tv_sec * NS_PER_SECOND + time_spec.tv_nsec;
   return nanoseconds;
}

static uint64_t adjust_endianess(uint64_t x)
{
    return x;
}

static int check_clock_available()
{
    return true;
}

#endif
