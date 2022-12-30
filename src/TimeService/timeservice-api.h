#ifndef TIMESERVICE_API_H
#define TIMESERVICE_API_H

#include "timeservice-utils.h"

#define INIT_TIMERS()
#define ADJUST_ENDIANESS(x)

uint64_t get_clock();

#ifdef RTEMS6_TARGET
#include "timeservice-rtems.h"
#endif

#ifdef GENERIC_LINUX_TARGET
#include "timeservice-linux.h"
#endif

void TimeService_Startup(void);


void TimeService_CfsTimestampCmp(const CfsTimestamp *t1, const CfsTimestamp *t2, int *result);


void TimeService_ClockStatus(ClockStatusEnum *status);


void TimeService_CucTimestamp(const uint64_t *nanoseconds, CUCTimestamp *timestamp);


void TimeService_CucTimestampCmp(const CUCTimestamp *t1, const CUCTimestamp *t2, int *result);


void TimeService_ObetTime(uint64_t *nanoseconds);

#endif
