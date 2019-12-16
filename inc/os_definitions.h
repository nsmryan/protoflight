/**
 * @file os_definitions.h
 *
 * @author Noah Ryan
 *
 * This file contains definitions that are common to OS abstractions
 * used by the fsw.
 */


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
typedef OS_Timeout int;

/**
 * This definition is the result type for OS abstraction types.
 * These can either indicate succses OS_RESULT_OKAY, or provide
 * an error code indicating the cause of the error.
 */
typedef enum
{
	OS_RESULT_INVALID = 0, /*<< Invalid result */
	OS_RESULT_OKAY    = 1, /*<< Success */
	OS_RESULT_NUM_RESULTS /*<< Number of result codes */
} OS_RESULT_ENUM;
