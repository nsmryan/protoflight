/**
 * @file os_definitions.h
 *
 * @author Noah Ryan
 *
 * This file contains definitions that are common to OS abstractions
 * used by the fsw.
 */
#ifndef __OS_DEFINITIONS_H__
#define __OS_DEFINITIONS_H__

#include "time.h" // included because of CLOCKS_PER_SEC

/**
 * This definition is the number of nanoseconds per system
 * clock tick
 */
#define OS_CONFIG_CLOCK_TICK_NANOSECONDS (OS_NANOSECONDS_PER_SECOND / CLOCKS_PER_SEC)

/**
 * This definition is the maximum length of an OS resource name, such as
 * a queue or thread.
 */
#define OS_CONFIG_MAX_NAME_LENGTH (256)

/**
 * This definition is the number of nanoseconds per second.
 */
#define OS_NANOSECONDS_PER_SECOND 1000000000

/**
 * This definition is used for timeout parameters (OS_Timeout) to indicate
 * that an operation may block indefinitely.
 */
#define OS_TIMEOUT_WAIT_FOREVER (-1)

/**
 * This definition is used for timeout parameters (OS_Timeout) to indicate
 * that an operation will not block.
 */
#define OS_TIMEOUT_NO_WAIT (0)


/**
 * A OS timeout is used for indicating whether to block
 * on a semaphore/queue/etc, and if so for how long
 */
typedef int OS_Timeout;

/**
 * This definition is the result type for OS abstraction types.
 * These can either indicate succses OS_RESULT_OKAY, or provide
 * an error code indicating the cause of the error.
 */
typedef enum
{
	OS_RESULT_INVALID            = 0, /*<< Invalid result */
	OS_RESULT_OKAY               = 1, /*<< Success */
	OS_RESULT_NULL_POINTER       = 2, /*<< Null pointer detected */
	OS_RESULT_TIMEOUT            = 3, /*<< Timeout */
	OS_RESULT_MSG_SIZE_ERROR     = 4, /*<< Message provided was too large */
	OS_RESULT_MAX_TIMERS_REACHED = 5, /*<< Maximum number of timers reached */
	OS_RESULT_NUM_RESULTS /*<< Number of result codes */
} OS_RESULT_ENUM;

#endif // ndef __OS_DEFINITIONS_H__ */
