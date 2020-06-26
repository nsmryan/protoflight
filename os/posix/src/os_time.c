
/**
 * @file os_time.c
 *
 * @author Noah Ryan
 *
 * This file contains definitions for the time OS abstraction
 * used by the fsw.
 */
#include "time.h"

#include "os_definitions.h"
#include "os_time.h"


OS_TimeStamp os_timestamp(void)
{
    OS_TimeStamp timestamp = {0, 0};

    struct timespec time;

    int ret_code = clock_gettime(CLOCK_MONOTONIC, &time);

    if (ret_code == 0)
    {
        timestamp.seconds = time.tv_sec;
        timestamp.nanoseconds = time.tv_nsec;
    }

    return timestamp;
}

double os_timestamp_double(void)
{
    OS_TimeStamp timestamp = os_timestamp();

    double time = ((double)timestamp.seconds);

    time += ((double)timestamp.nanoseconds) / ((double)OS_NANOSECONDS_PER_SECOND);

    return time;
}
