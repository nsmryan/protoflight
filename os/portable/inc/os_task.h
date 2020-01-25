/**
 * @file os_task.h
 *
 * @author Noah Ryan
 * 
 * This file contains the definitions for the OS task abstraction used
 * by the fsw.
 *
 * This implementation uses TinyCThreads for portability.
 */
#ifndef __OS_TASK_H__
#define __OS_TASK_H__

#include "pthread.h"

#include "os_definitions.h"


/**
 * The OS_Task type is the implementation dependant type for
 * tasks. This type is used as a pointer, allowing it to either
 * point to task data, or to a handle passed to OS functions.
 */
typedef pthread_t OS_Task;

/**
 * The OS_TASK_FUNC type is for task function pointers registered
 * with the Task Manager module.
 */
typedef void (OS_TASK_FUNC)(void*);

/**
 * This enum provides possible task status' returned by
 * os_task_status
 */
typedef enum
{
  OS_TASK_STATUS_INVALID      = 0,
  OS_TASK_STATUS_OKAY         = 1,
  OS_TASK_STATUS_TASK_UNKNOWN = 2,
  OS_TASK_STATUS_TASK_CRASHED = 3,
  OS_TASK_STATUS_NUM_STATUS
} OS_TASK_STATUS_ENUM;


/**
 * @brief os_task_spawn
 *
 * This function spawns a new task.
 *
 * @return A OS result type either indicating success (OS_RESULT_OKAY)
 * or an error code indicating the cause of the error.
 */
OS_RESULT_ENUM os_task_spawn(OS_Task *task,
                             OS_TASK_FUNC *function,
                             void *argument,
                             int priority,
                             int stack_size);

/**
 * @brief os_task_status
 *
 * This function returns a OS_TASK_STATUS_ENUM value indicating the
 * current status of the task.
 *
 * @return A value of type OS_TASK_STATUS_ENUM, indicating the
 * current status of the task.
 */
OS_TASK_STATUS_ENUM os_task_status(OS_Task *task);

#endif // ndef __OS_TASK_H__ */