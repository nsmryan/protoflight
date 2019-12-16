/**
 * @file os_mutex.h
 *
 * @author Noah Ryan
 *
 * This file contains definitions for the OS mutex abstraction
 * used by the fsw. This is similar to a semaphore, but can be
 * taken and given recursively and is intended for mutually
 * exclusive access to a resource.
 */
#include "os_definitions.h"


/**
 * This definition is for the internal representation of a mutex
 * within the OS abstraction.
 */
typedef OS_Mutex int;

/**
 * @brief os_mutex_create
 *
 * This function creates a mutex.
 *
 * @return A OS result type either indicating success (OS_RESULT_OKAY)
 * or an error code indicating the cause of the error.
 */
OS_RESULT_ENUM os_mutex_create(OS_Mutex *mutex);

/**
 * @brief os_mutex_give
 *
 * This function gives a mutex.
 *
 * @return An OS result indicating either success (OS_RESULT_OKAY)
 * or indicating the cause of the error.
 */
OS_RESULT_ENUM os_mutex_give(OS_Mutex mutex);

/**
 * @brief os_mutex_take
 *
 * This function attempts to take a mutex, blocking if it is not
 * currently avaiable. The given timeout indicates how long to block,
 * or if timeout == OS_NO_WAIT, then to return an error without blocking.
 *
 * @param[in] mutex - the mutex to take.
 * @param[in] timeout - the amount of time (in system clock ticks) to block
 *            while waiting for the mutex. If this value is FSW_NO_WAIT
 *            then this function will not block.
 *
 * @return An OS result type either indicating success (OS_RESULT_OKAY)
 * or an error code indicating the cause of the error.
 */
OS_RESULT_ENUM os_mutex_take(OS_Mutex mutex, FSW_Timeout timeout);

