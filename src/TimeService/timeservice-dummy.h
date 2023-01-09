#ifndef TIMESERVICE_DUMMY_H
#define TIMESERVICE_DUMMY_H

#include "timeservice-utils.h"

static uint64_t get_time()
{
    return 0;
}

static uint64_t adjust_endianess(uint64_t x)
{
    return x;
}

ClockStatusEnum check_clock_available()
{
    return false;
}

#endif
