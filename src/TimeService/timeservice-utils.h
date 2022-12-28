#ifndef TIMESERVICE_UTILS
#define TIMESERVICE_UTILS

#include <time.h>
#include <stdint.h>

#define NS_PER_SECOND (1000000000)

#define CUC_TIME_ARRAY_SIZE 7

typedef struct CUCTimestamp
{
   unsigned char pField;
   unsigned char tField[CUC_TIME_ARRAY_SIZE];
} CUCTimestamp;

typedef struct CfsTimestamp
{
   uint64_t seconds;
   uint64_t subseconds;
} CfsTimestamp;

typedef enum {
    UNAVAILABLE = 0,
    STOPPED = 1,
    STARTED = 2
} ClockStatusEnum;

static uint64_t timespec_to_nanoseconds(struct timespec *time)
{
   return time->tv_sec * NS_PER_SECOND + time->tv_nsec;
}

static int cmp_memory(const unsigned char *str1, const unsigned char *str2, int len)
{
    int charCompareStatus = 0;
    while (len > 0)
    {
        if (*str1 != *str2)
        {
            charCompareStatus = (*str1 > *str2) ? 1 : -1;
            break;
        }
        len--;
        str1++;
        str2++;
    }
    return charCompareStatus;
}

static void delay(uint64_t loop_cycles)
{
    for (volatile uint64_t i = 0; i < loop_cycles; i++);
}

#endif
