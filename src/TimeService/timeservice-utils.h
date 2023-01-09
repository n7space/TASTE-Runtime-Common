#ifndef TIMESERVICE_UTILS
#define TIMESERVICE_UTILS

#include <time.h>
#include <stdint.h>

#define true 1
#define false 0

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
    AVAILABLE = 1
} ClockStatusEnum;

static int cmp_memory(const unsigned char *string_1, const unsigned char *string_2, int length)
{
    int charCompareStatus = 0;
    while (length > 0)
    {
        if (*string_1 != *string_2)
        {
            charCompareStatus = (*string_1 > *string_2) ? 1 : -1;
            break;
        }
        length--;
        string_1++;
        string_2++;
    }
    return charCompareStatus;
}

#endif
