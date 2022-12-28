#include "timeservice-utils.h"

#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define INIT_TIMERS()
#define ADJUST_ENDIANESS(x)

uint64_t get_clock()
{
   struct timespec time_spec;
   clock_gettime(CLOCK_REALTIME, &time_spec);
   uint64_t nanoseconds = timespec_to_nanoseconds(&time_spec);
   return nanoseconds;
}
