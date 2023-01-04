#include "timeservice-api.h"

#define BYTE_CAPACITY 256

CUCTimestamp nanoseconds_to_cuc_timestamp(uint64_t nanoseconds)
{
    CUCTimestamp timestamp;
    long double seconds = (long double)nanoseconds / NS_PER_SECOND;

    // CUC timestamp contains three bytes for fractions of a second
    uint64_t seconds_shifted = seconds * BYTE_CAPACITY * BYTE_CAPACITY * BYTE_CAPACITY; 
    seconds_shifted = adjust_endianess(seconds_shifted);
    unsigned char *time = (unsigned char *)&seconds_shifted;

    for (size_t i = 0; i < sizeof(timestamp.tField); i++)
    {
        timestamp.tField[i] = time[sizeof(timestamp.tField) - 1 - i];
    }
    return timestamp;
}

void TimeService_Startup(TimeService *const self)
{
    self->starting_time = get_time();
}

int TimeService_CfsTimestampCmp(TimeService *const self, const CfsTimestamp *timestamp1, const CfsTimestamp *timestamp2)
{
    // Unused in this implementation
    (void)self;

    if (timestamp1->seconds > timestamp2->seconds)
    {
        return 1;
    }
    else if (timestamp1->seconds < timestamp2->seconds)
    {
        return -1;
    }
    else if (timestamp1->seconds == timestamp2->seconds)
    {
        if (timestamp1->subseconds > timestamp2->subseconds)
        {
            return 1;
        }
        else if (timestamp1->subseconds < timestamp2->subseconds)
        {
            return -1;
        }
        else
        {
            return 0;
        }
    }
    return 0;
}


ClockStatusEnum TimeService_ClockStatus(TimeService *const self)
{
    // Unused in this implementation
    (void)self;

    ClockStatusEnum status = STOPPED;

    uint64_t start_time = get_time();
    delay(10000);
    uint64_t finish_time = get_time();

    if (finish_time != start_time)
    {
        status = STARTED;
    }
    return status;
}

void TimeService_CucTimestamp(TimeService *const self, uint64_t nanoseconds, CUCTimestamp *timestamp)
{
    // Unused in this implementation
    (void)self;

    *timestamp = nanoseconds_to_cuc_timestamp(nanoseconds);
}


int TimeService_CucTimestampCmp(TimeService *const self, const CUCTimestamp *timestamp1, const CUCTimestamp *timestamp2)
{
    // Unused in this implementation
    (void)self;

    return cmp_memory(timestamp1->tField, timestamp2->tField, sizeof(timestamp1->tField));
}


uint64_t TimeService_ObetTime(TimeService *const self)
{
    uint64_t measured_time = get_time();
    return measured_time - self->starting_time;
}
