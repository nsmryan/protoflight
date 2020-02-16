/**
 * @file os_time.h
 *
 * @author Noah Ryan
 *
 * This file contains definitions for the time OS abstraction
 * used by the fsw.
 */
#ifndef __OS_TIME_H__
#define __OS_TIME_H__

#include "stdbool.h"
#include "stdint.h"

#include "os_definitions.h"


/**
 * This definition is the timestamp structure used in the OS
 * abstraction layer.
 *
 * It provides a consistent timestamp format with high resolution,
 * even if the underlying clock has a resolution.
 *
 * This is not necessarily intended as the FSW time, which may be
 * managed separately. It is simply an abstraction of a free running
 * clock used to timestamp events.
 */
typedef struct OS_TimeStamp
{
    uint32_t seconds;
    uint32_t nanoseconds;
} OS_TimeStamp;


/**
 * This function samples the current time.
 *
 * @return the current time in a timestamp structure.
 * If an error occurs in sampling the time then the 'timestamp' parameter
 * will be set to 0 seconds and 0 nanoseconds.
 */
OS_TimeStamp os_timestamp(void);

/**
 * This function provides a timestamp as a double precision floating
 * point number. This is the same timestamp provided by os_timestamp.
 *
 * @return the current time as a double.
 * If an error occurs in sampling the time then the return value
 * will be 0.0.
 */
double os_timestamp_double(void);

#endif // ndef __OS_TIME_H__ */
