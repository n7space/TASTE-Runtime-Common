#ifndef TIMESERVICE_RTEMS6_H
#define TIMESERVICE_RTEMS6_H

#include "interfaces_info.h"

#define TICKS_PER_SECOND 1000000
#define SCALER_VALUE 5

static uint64_t get_time()
{
   uint64_t ticks = UINT64_MAX - ((uint64_t)(Timer_Apbctrl1_getCounterValue(&timer_2) + 1) << 32
         | Timer_Apbctrl1_getCounterValue(&timer_1));
   long double ticks_per_nanosecond = (long double)TICKS_PER_SECOND / NS_PER_SECOND;
   uint64_t nanoseconds = ticks / ticks_per_nanosecond;
   return nanoseconds;
}

static uint64_t swap_bytes(uint64_t x)
{
  uint64_t result;
  // Every byte needs to be shifted by a proper multiple of 8 bits
  result = ((x & 0x00000000000000FFULL) << 56) |
           ((x & 0x000000000000FF00ULL) << 40) |
           ((x & 0x0000000000FF0000ULL) << 24) |
           ((x & 0x00000000FF000000ULL) << 8) |
           ((x & 0x000000FF00000000ULL) >> 8) |
           ((x & 0x0000FF0000000000ULL) >> 24) |
           ((x & 0x00FF000000000000ULL) >> 40) |
           ((x & 0xFF00000000000000ULL) >> 56);
  return result;
}

static uint64_t adjust_endianess(uint64_t x)
{
    return swap_bytes(x);
}

static int check_clock_available()
{
    if (Timer_Apbctrl1_getCounterValue(&timer_1) && Timer_Apbctrl1_getCounterValue(&timer_2)) {
      return true;
    }
    return false;
}

#endif
