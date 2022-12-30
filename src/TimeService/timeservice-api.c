#include "timeservice-api.h"

#define BYTE_CAPACITY 256

void nanoseconds_to_cuc_timestamp(uint64_t nanoseconds, CUCTimestamp *timestamp)
{
   long double seconds = (long double)nanoseconds / NS_PER_SECOND;

   // CUC timestamp contains three bytes for fractions of a second
   uint64_t seconds_shifted = seconds * BYTE_CAPACITY * BYTE_CAPACITY * BYTE_CAPACITY; 

   ADJUST_ENDIANESS(seconds_shifted);
   unsigned char *time = (unsigned char *)&seconds_shifted;

   for (size_t i = 0; i < sizeof(timestamp->tField); i++)
   {
      timestamp->tField[i] = time[sizeof(timestamp->tField) - 1 - i];
   }
}

void TimeService_Startup(TimeService *const self)
{
    self->starting_time = get_time();
}

void TimeService_CfsTimestampCmp(TimeService *const self, const CfsTimestamp *t1, const CfsTimestamp *t2, int *result)
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


void TimeService_ClockStatus(TimeService *const self, ClockStatusEnum *status)
{
   *status = STOPPED;

   uint64_t start_time = get_time();
   delay(10000);
   uint64_t finish_time = get_time();

   if (finish_time != start_time)
   {
      *status = STARTED;
   }
}

void TimeService_CucTimestamp(TimeService *const self, const uint64_t *nanoseconds, CUCTimestamp *timestamp)
{
    nanoseconds_to_cuc_timestamp(*nanoseconds, timestamp);
}


void TimeService_CucTimestampCmp(TimeService *const self, const CUCTimestamp *t1, const CUCTimestamp *t2, int *result)
{
    *result = cmp_memory(t1->tField, t2->tField, sizeof(t1->tField));
}


void TimeService_ObetTime(TimeService *const self, uint64_t *nanoseconds)
{
    uint64_t measured_time = get_time();
    *nanoseconds = measured_time - self->starting_time;
}
