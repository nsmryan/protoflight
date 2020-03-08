/**
 * @file os_sem.c
 *
 * @author Noah Ryan
 *
 * This file contains the implementations of semaphore functions for the OS
 * abstraction used by the FSW.
 */
#include "string.h"

#include "errno.h"

#include "pthread.h"

#include "os_definitions.h"
#include "os_sem.h"


#define OS_SEM_SHARED 0


OS_RESULT_ENUM os_sem_create(OS_Sem *sem)
{
    OS_RESULT_ENUM result = OS_RESULT_OKAY;
    int ret_code = 0;

    if (sem == NULL)
    {
        result = OS_RESULT_NULL_POINTER;
    }

    if (result == OS_RESULT_OKAY)
    {
        ret_code = sem_init(sem, OS_SEM_SHARED, 0);
        if (ret_code < 0)
        {
            result = OS_RESULT_ERROR;
        }
    }

    return result;
}

OS_RESULT_ENUM os_sem_give(OS_Sem *sem)
{
    OS_RESULT_ENUM result = OS_RESULT_OKAY;
    int ret_code = 0;
  
    if (sem == NULL)
    {
        result = OS_RESULT_NULL_POINTER;
    }
    else
    {
        ret_code = sem_post(sem);
        if (ret_code < 0)
        {
            result = OS_RESULT_ERROR;
        }
    }
  
    return result;
}

OS_RESULT_ENUM os_sem_take(OS_Sem *sem, OS_Timeout timeout)
{
    OS_RESULT_ENUM result = OS_RESULT_OKAY;
    int ret_code = 0;
  
    if (sem == NULL)
    {
        result = OS_RESULT_NULL_POINTER;
    }
  
    if (result == OS_RESULT_OKAY)
    {
        struct timespec timeout_spec;
  
        clock_gettime(CLOCK_MONOTONIC, &timeout_spec);
  
        int nanoseconds = timeout * OS_CONFIG_CLOCK_TICK_NANOSECONDS;
        timeout_spec.tv_sec += nanoseconds / OS_NANOSECONDS_PER_SECOND;
        timeout_spec.tv_nsec += nanoseconds % OS_NANOSECONDS_PER_SECOND;;
  
        ret_code = sem_timedwait(sem, &timeout_spec);
        while ((ret_code == -1) && (errno == EINTR))
        {
            ret_code = sem_timedwait(sem, &timeout_spec);
        }
    }

    if (ret_code != 0)
    {
        if (errno == ETIMEDOUT)
        {
            result = OS_RESULT_TIMEOUT;
        }
        else
        {
            result = OS_RESULT_ERROR;
        }
    }
  
    return result;
}

