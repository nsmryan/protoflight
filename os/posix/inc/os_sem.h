/**
 * @file os_sem.h
 *
 * @author Noah Ryan
 *
 * This file contains definitions for the OS semaphore abstraction
 * used by the fsw.
 */
#ifndef __OS_SEM_H__
#define __OS_SEM_H__

#include "pthread.h"

#include "os_definitions.h"


/**
 * This definition is for the internal representation of a semaphore
 * within the OS abstraction.
 */
typedef struct OS_Sem
{
  pthread_cond_t cond;
  pthread_mutex_t mutex;
} OS_Sem;


/**
 * @brief os_sem_create
 *
 * This function creates a semaphore.
 *
 * @return A OS result type either indicating success (OS_RESULT_OKAY)
 * or an error code indicating the cause of the error.
 */
OS_RESULT_ENUM os_sem_create(OS_Sem *sem);

/**
 * @brief os_sem_give
 *
 * This function gives a semaphore.
 *
 * @return An OS result indicating either success (OS_RESULT_OKAY)
 * or indicating the cause of the error.
 */
OS_RESULT_ENUM os_sem_give(OS_Sem *sem);

/**
 * @brief os_sem_take
 *
 * This function attempts to take a semaphore, blocking if it is not
 * currently avaiable. The given timeout indicates how long to block,
 * or if timeout == OS_NO_WAIT, then to return an error without blocking.
 *
 * @param[in] sem - the semaphore to take.
 * @param[in] timeout - the amount of time (in system clock ticks) to block
 *            while waiting for the semaphore. If this value is FSW_NO_WAIT
 *            then this function will not block.
 *
 * @return An OS result type either indicating success (OS_RESULT_OKAY)
 * or an error code indicating the cause of the error.
 */
OS_RESULT_ENUM os_sem_take(OS_Sem *sem, OS_Timeout timeout);

#endif // ndef __OS_SEM_H__ */