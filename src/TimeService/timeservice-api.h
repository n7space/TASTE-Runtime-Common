#ifndef TIMESERVICE_API_H
#define TIMESERVICE_API_H

#include "timeservice-utils.h"

#define ADJUST_ENDIANESS(x)

static uint64_t get_time();

#ifdef RTEMS6_TARGET
#include "timeservice-rtems.h"
#endif

#ifdef GENERIC_LINUX_TARGET
#include "timeservice-linux.h"
#endif

/// @brief Structure representing TimeService
typedef struct
{
    uint64_t starting_time;
} TimeService;

void TimeService_Startup(TimeService *const self);


void TimeService_CfsTimestampCmp(TimeService *const self, const CfsTimestamp *t1, const CfsTimestamp *t2, int *result);


void TimeService_ClockStatus(TimeService *const self, ClockStatusEnum *status);


void TimeService_CucTimestamp(TimeService *const self, const uint64_t *nanoseconds, CUCTimestamp *timestamp);


void TimeService_CucTimestampCmp(TimeService *const self, const CUCTimestamp *t1, const CUCTimestamp *t2, int *result);


void TimeService_ObetTime(TimeService *const self, uint64_t *nanoseconds);

#endif
