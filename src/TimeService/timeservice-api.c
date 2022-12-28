#include "timeservice-api.h"

static uint64_t starting_time;

void nanoseconds_to_cuc_timestamp(uint64_t nanoseconds, CUCTimestamp *timestamp)
{
   long double seconds = (long double)nanoseconds / NS_PER_SECOND;
   uint64_t seconds_shifted = seconds * 256 * 256 * 256;

   ADJUST_ENDIANESS(seconds_shifted);
   unsigned char *time = (unsigned char *)&seconds_shifted;

   CUCTimestamp *timestamp_struct = (CUCTimestamp *)timestamp;

   for (int i = 0; i < sizeof(timestamp_struct->tField); i++)
   {
      timestamp_struct->tField[i] = time[sizeof(timestamp_struct->tField) - 1 - i];
   }
}

void TimeService_Startup(void)
{
    INIT_TIMERS();
    starting_time = get_clock();
}

void TimeService_CfsTimestampCmp(const CfsTimestamp *t1, const CfsTimestamp *t2, int *result)
{
    if (t1->seconds > t2->seconds)
    {
        *result = 1;
    }
    else if (t1->seconds < t2->seconds)
    {
        *result = -1;
    }
    else if (t1->seconds == t2->seconds)
    {
        if (t1->subseconds > t2->subseconds)
        {
            *result = 1;
        }
        else if (t1->subseconds < t2->subseconds)
        {
            *result = -1;
        }
        else
        {
            *result = 0;
        }
    }
}


void TimeService_ClockStatus(ClockStatusEnum *status)
{
   *status = STOPPED;

   uint64_t starting_time = get_clock();
   delay(10000);
   uint64_t finish_time = get_clock();

   if (finish_time != starting_time)
   {
      *status = STARTED;
   }
}

void TimeService_CucTimestamp(const uint64_t *nanoseconds, uint64_t *timestamp)
{
    nanoseconds_to_cuc_timestamp(*nanoseconds, timestamp);
}


void TimeService_CucTimestampCmp(const CUCTimestamp *t1, const CUCTimestamp *t2, int *result)
{
    *result = cmp_memory(t1->tField, t2->tField, sizeof(t1->tField));
}


void TimeService_ObetTime(uint64_t *nanoseconds)
{
    uint64_t measured_time = get_clock();
    *nanoseconds = measured_time - starting_time;
}
