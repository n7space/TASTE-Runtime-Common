#include "interfaces_info.h"

#define TICKS_PER_SECOND 1000000
#define SCALER_VALUE 5

void init_timer(Timer_Apbctrl1 *const timer, bool isChained, Timer_Id timerId)
{
   Timer_Apbctrl1_stop(timer);
   Timer_Config config;
   config.isEnabled = true;
   config.isAutoReloaded = true;
   config.isInterruptEnabled = false;
   config.isChained = isChained;
   config.reloadValue = UINT32_MAX;
   Timer_Apbctrl1_init(timerId, timer, defaultInterruptHandler);
   Timer_Apbctrl1_setBaseScalerReloadValue(timer, SCALER_VALUE);
   Timer_Apbctrl1_setConfigRegisters(timer, &config);
   Timer_Apbctrl1_start(timer);
}

static uint64_t swap_bytes(uint64_t x)
{
  uint64_t result;
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

uint64_t get_clock()
{
   uint64_t ticks = UINT64_MAX - ((uint64_t)(Timer_Apbctrl1_getCounterValue(&timer_2) + 1) << 32
         | Timer_Apbctrl1_getCounterValue(&timer_1));
   long double ticks_per_nanosecond = (long double)TICKS_PER_SECOND / NS_PER_SECOND;
   uint64_t nanoseconds = ticks / ticks_per_nanosecond;
   return nanoseconds;
}

#define INIT_TIMERS() \
    init_timer(&timer_2, true, Timer_Id_3); \
    init_timer(&timer_1, false, Timer_Id_2)

#define ADJUST_ENDIANESS(x) \
   x = swap_bytes(x)
