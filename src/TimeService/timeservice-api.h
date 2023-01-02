#ifndef TIMESERVICE_API_H
#define TIMESERVICE_API_H

#include "timeservice-utils.h"

#if defined(RTEMS6_TARGET)
#include "timeservice-rtems.h"
#elif defined(GENERIC_LINUX_TARGET)
#include "timeservice-linux.h"
#else
#include "timeservice-dummy.h"
#endif

/// @brief Structure representing TimeService
typedef struct
{
    uint64_t starting_time;
} TimeService;

void TimeService_Startup(TimeService *const self);


int TimeService_CfsTimestampCmp(TimeService *const self, const CfsTimestamp *timestamp1, const CfsTimestamp *timestamp2);


ClockStatusEnum TimeService_ClockStatus(TimeService *const self);


void TimeService_CucTimestamp(TimeService *const self, const uint64_t nanoseconds, CUCTimestamp *timestamp);


int TimeService_CucTimestampCmp(TimeService *const self, const CUCTimestamp *timestamp1, const CUCTimestamp *timestamp2);


uint64_t TimeService_ObetTime(TimeService *const self);

#endif
