/**
 * @file os_sem.c
 *
 * @author Noah Ryan
 *
 * This file contains the implementations of semaphore functions for the OS
 * abstraction used by the FSW.
 */
#include "string.h"

#include "pthread.h"

#include "os_definitions.h"
#include "os_sem.h"


OS_RESULT_ENUM os_sem_create(OS_Sem *sem)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;
  int ret_code = 0;

  // a pthreads semaphore consists of a mutex and a condition
  // variable. this function sets up both and stores them in
  // the given sem.

  pthread_mutexattr_t mutex_attr;

  if (sem == NULL)
  {
    result = OS_RESULT_NULL_POINTER;
  }

  if (result == OS_RESULT_OKAY)
  {
    // The return of memset is not checked as it is a pointer to
    // mutex_attr which cannot be NULL.
    memset(&mutex_attr, 0, sizeof(mutex_attr));

    ret_code = pthread_mutexattr_init(&mutex_attr);
    if (ret_code < 0)
    {
      result = OS_RESULT_ERROR;
    }
  }

  // set the mutex to use priority inheritance
  if (result == OS_RESULT_OKAY)
  {
    ret_code = pthread_mutexattr_setprotocol(&mutex_attr, PTHREAD_PRIO_INHERIT);
    if (ret_code < 0)
    {
      result = OS_RESULT_ERROR;
    }
  }

  // initialze the mutex
  if (result == OS_RESULT_OKAY)
  {
    ret_code = pthread_mutex_init(&sem->mutex, &mutex_attr);
    if (ret_code < 0)
    {
      result = OS_RESULT_ERROR;
    }
  }

  // initialize condition variable with default settings
  if (result == OS_RESULT_OKAY)
  {
    ret_code = pthread_cond_init(&sem->cond, NULL);
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

  if (sem == NULL)
  {
    result = OS_RESULT_NULL_POINTER;
  }

  if (result == OS_RESULT_OKAY)
  {
    pthread_cond_signal(&sem->cond);
  }

  return result;
}

OS_RESULT_ENUM os_sem_take(OS_Sem *sem, OS_Timeout timeout)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;

  if (sem == NULL)
  {
    result = OS_RESULT_NULL_POINTER;
  }

  if (result == OS_RESULT_OKAY)
  {
    struct timespec timeout_spec;

    int nanoseconds = timeout * OS_CONFIG_CLOCK_TICK_NANOSECONDS;
    timeout_spec.tv_sec = nanoseconds / OS_NANOSECONDS_PER_SECOND;
    timeout_spec.tv_nsec = nanoseconds % OS_NANOSECONDS_PER_SECOND;;

    int ret_code =
      pthread_cond_timedwait(&sem->cond, &sem->mutex, &timeout_spec);

    if (ret_code < 0)
    {
      result = OS_RESULT_ERROR;
    }
  }

  return result;
}

