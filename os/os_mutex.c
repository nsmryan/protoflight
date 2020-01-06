/**
 * @file os_mutex.c
 *
 * @author Noah Ryan
 *
 * This file contains the implementations of mutex functions for the OS
 * abstraction used by the FSW.
 */
#include "stdbool.h"

#include "errno.h"
#include "pthreads.h"

#include "os_definitions.h"
#include "os_mutex.h"


OS_RESULT_ENUM os_mutex_create(OS_Mutex *mutex)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;
  int ret_code = 0;

  bool attribute_init = false;

  pthread_mutexattr_t attr;

  memset(&attr, 0, sizeof(pthread_mutexattr_t));

  if (mutex == NULL)
  {
    ret_code = -1;
    result = OS_RESULT_NULL_POINTER;
  }

  if (ret_code == 0)
  {
    ret_code = pthread_mutexattr_init(&attr);
    if (ret_code == 0)
    {
      attribute_init = true;

      ret_code = pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_INHERIT);
    }
  }

  if (ret_code == 0)
  {
    ret_code = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
  }

  if (ret_code == 0)
  {
    ret_code = pthread_mutex_init(mutex, &attr);
  }

  if (attribute_init)
  {
    ret_code = pthread_mutexattr_destroy(&attr);
  }

  if (ret_code != 0)
  {
    if (result == OS_RESULT_OKAY)
    {
      result = OS_RESULT_ERROR;
    }
  }

  return result;
}

OS_RESULT_ENUM os_mutex_give(OS_Mutex *mutex, FSW_Timeout timeout)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;

  struct timespec timeout_spec;

  if (mutex == NULL)
  {
    result = OS_RESULT_NULL_POINTER;
  }

  if (result == OS_RESULT_OKAY)
  {
    int nanoseconds = timeout * OS_CONFIG_CLOCK_TICK_NANOSECONDS;
    timeout_spec.tv_sec = nanoseconds % OS_NANOSECONDS_PER_SECOND;
    timeout_spec.tv_nsec = nanoseconds / OS_NANOSECONDS_PER_SECOND;;

    int ret_code = pthread_mutex_timedlock(mutex, &timeout_spec);
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
  }

  return result;
}

OS_RESULT_ENUM os_mutex_take(OS_Mutex *mutex, FSW_Timeout timeout)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;

  if (mutex == NULL)
  {
    result = OS_RESULT_NULL_POINTER;
  }

  if (result == OS_RESULT_OKAY)
  {
    int ret_code = pthread_mutex_unlock(mutex);
    if (ret_code != 0)
    {
      result = OS_RESULT_ERROR;
    }
  }

  return result;
}
