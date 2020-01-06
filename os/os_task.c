/**
 * @file os_task.c
 *
 * @author Noah Ryan
 * 
 * This file contains the implementation for the OS task abstraction used
 * by the fsw.
 */
#include "os_definitions.h"
#include "os_task.h"


OS_RESULT_ENUM os_task_spawn(OS_Task *task,
                             OS_TASK_FUNC function,
                             int argument,
                             int priority,
                             int stack_size)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;


  // check input parameters for NULL pointers
  if ((task == NULL) || (function == NULL))
  {
    result = OS_RESULT_NULL_POINTER;
  }

  // initialize the pthread attributes
  if (result == OS_RESULT_OKAY)
  {
    int ret_code = 0;

    pthread_attr_t attr;

    // the return value of memset is not checked as it returns a pointer to
    // the set memory area, which was checked for NULL above.
    memset(&attr, 0, sizeof(pthread_attr_t));
    ret_code = pthread_attr_init(&attr);

    if (ret_code < 0)
    {
      result = OS_RESULT_ERROR;
    }
  }

  // Check if the user is root. This is required to set thread attributes.
  // This strategy comes from NASA OSAL.
  if (geteuid() == 0)
  {
    if (result == OS_RESULT_OKAY)
    {
      ret_code = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
      if (ret_code < 0)
      {
        result = OS_RESULT_ERROR;
      }
    }

    if (result == OS_RESULT_OKAY)
    {
      ret_code = pthread_attr_setstacksize(&attr, (size_t)stack_size);
      if (ret_code < 0)
      {
        result = OS_RESULT_ERROR;
      }
    }

    if (result == OS_RESULT_OKAY)
    {
      ret_code = pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
      if (ret_code < 0)
      {
        result = OS_RESULT_ERROR;
      }
    }

    if (result == OS_RESULT_OKAY)
    {
      struct sched_param priority_struct;

      memset(&priority_struct, 0, sizeof(priority));
      priority_struct.sched_priority = priority;

      ret_code = pthread_attr_setschedparam(&attr, &priority_struct);
      if (ret_code < 0)
      {
        result = OS_RESULT_ERROR;
      }
    }
  }

  // create the thread itself
  if (result == OS_RESULT_OKAY)
  {
    ret_code = pthread_create(task, &attr, function, argument);

    if (ret_code < 0)
    {
      result = OS_RESULT_ERROR;
    }
  }

  // detach the thread, cleaning up resources (taken from NASA OSAL)
  if (result == OS_RESULT_OKAY)
  {
    ret_code = pthread_detach(task);

    if (ret_code < 0)
    {
      result = OS_RESULT_ERROR;
    }
  }

  // clean up attributes
  if (result == OS_RESULT_OKAY)
  {
    ret_code = pthread_attr_destroy(&attr);

    if (ret_code < 0)
    {
      result = OS_RESULT_ERROR;
    }
  }

  return result;
}

OS_TASK_STATUS_ENUM os_task_status(OS_Task task)
{
  OS_TASK_STATUS_ENUM task_status = OS_TASK_STATUS_OKAY;

  // NOTE no implementation appears possible for POSIX?

  return task_status;
}

