/**
 * @file tm_definitions.h
 *
 * @author Noah Ryan
 *
 * This file contains definitions for the Task Manager module.
 */
#ifndef __TM_DEFINITIONS_H__
#define __TM_DEFINITIONS_H__

#include "stdint.h" 
#include "stdbool.h" 

#include "os_sem.h"
#include "os_timer.h"


/**
 * This definition provides the number of system clock ticks per second.
 * This can be used when calculating task periods.
 */
#define TM_SYSTEM_CLOCK_TICKS_PER_SECOND 100

/**
 * This definition determines the number of system clock ticks between
 * Task Manager schedule slots. This can be either 1 (one slot per tick)
 * or greater to run the scheduler slower than the system clock.
 */
#define TM_SYSTEM_CLOCK_TICKS_PER_SLOT 1

/**
 * This definition is the maximum number of tasks that can be registered
 * with the Task Manager module.
 */
#define TM_MAX_TASKS 100

/**
 * The name of the Task Manager's schedule task
 */
#define TM_TASK_NAME_TM_SCHEDULER "tm_scheduler"


/**
 * This type is for task ids, which uniquely identify a particular task.
 */
typedef int TM_TaskId;

/**
 * This type is for bitfields where each bit cooresponds to a task, given
 * by its task id.
 * Not that the size of this bitfield must be greater than or equal to
 * TM_MAX_TASKS.
 */
typedef int TM_TaskBitField;

/**
 * This enum provides the results for Task Manager module functions.
 */
typedef enum TM_RESULT_ENUM
{
	TM_RESULT_INVALID           = 0, /**< Invalid result */
	TM_RESULT_OKAY              = 1, /**< Successful result */
	TM_RESULT_NULL_POINTER      = 2, /**< Null pointer detected */
	TM_RESULT_INVALID_ARGUMENT  = 3, /**< Invalid argument provided to a TM function */
	TM_RESULT_TIMER_ERROR       = 4, /**< Timer error */
	TM_RESULT_TASK_SPAWN_ERROR  = 5, /**< Task spawn returned an error */
	TM_RESULT_SEM_CREATE_ERROR  = 6, /**< Semaphore create returned an error */
	TM_RESULT_NUM_RESULTS
} TM_RESULT_ENUM;

/**
 * This enum provides the possible task types supported by the Task Manager
 * module.
 */
typedef enum TM_TASKTYPE_ENUM
{
	TM_TASKTYPE_INVALID  = 0, /*<< Invalid task type */
	TM_TASKTYPE_PERIODIC = 1, /*<< Periodic task, scheduled at a fixed rate */
	TM_TASKTYPE_EVENT    = 2, /*<< Aperiodic task, or run on external events */
	TM_TASKTYPE_CALLBACK = 3, /*<< Periodic task, called as a callback in the scheulder task */
	TM_TASKTYPE_MONITOR  = 4, /*<< External task, does not participate in heartbeat */
} TM_TASKTYPE_ENUM;

/**
 * This enum provides task status for a task in a particular schedule slot.
 */
typedef enum TM_TASKSTATUS_ENUM
{
	TM_TASKSTATUS_INVALID          = 0,
	TM_TASKSTATUS_WAIT             = 1,
	TM_TASKSTATUS_SCHEDULE         = 2,
	TM_TASKSTATUS_MISSED_HEARTBEAT = 3,
	TM_TASKSTATUS_ERROR            = 4,
} TM_TASKSTATUS_ENUM;

/**
 * This struct contains the information that Task Manager keeps on each
 * task, including its type and status.
 */
typedef struct TM_Task
{
	TM_TASKTYPE_ENUM type;
	uint32_t schedule_period;
	uint32_t heartbeat_period;
	uint32_t ticks;
	OS_TASK_FUNC *function;
	void *argument;
	int stack_size;
	int priority;
	OS_Sem semaphore;
  OS_Task os_task;
} TM_Task;

/**
 * This struct provides the status of the Task Manager module.
 */
typedef struct TM_Status
{
	uint32_t cycle;
	TM_TaskBitField tasks_scheduled;
	TM_TaskBitField tasks_missed_heartbeat;
} TM_Status;

/**
 * This struct is the state of the Task Manager module.
 */
typedef struct TM_State
{
	TM_Status status;

	bool continue_running;

	OS_Sem schedule_semaphore;
	OS_Timer schedule_timer;

	uint16_t num_tasks;
	TM_Task tasks[TM_MAX_TASKS];
} TM_State;

#endif // ndef __TM_DEFINITIONS_H__ */
