/**
 * @file tm_interface.h
 *
 * @author Noah Ryan
 *
 * This file contains the interface to the Task Manager module.
 */
#ifndef __TM_INTERFACE_H__
#define __TM_INTERFACE_H__

#include "os_task.h"

#include "fsw_definitions.h"

#include "tm_definitions.h"


/**
 * @brief tm_initialize
 *
 * This function initializes the Task Manager module.
 *
 * @return Either FSW_RESULT_OKAY, or an error code indicating
 * the cause of the initialization error.
 */
FSW_RESULT_ENUM tm_initialize(void);

/**
 * @brief tm_periodic_task
 *
 * This task registers a task to run at a particular period. This period
 * is given in schedule slot ticks. If TM_SYSTEM_CLOCK_TICKS_PER_SLOT is 1,
 * then this is in system clock ticks.
 *
 * Tasks are scheduled by releasing a semaphore. The task can block on this
 * semaphore by calling the tm_running function.
 *
 * @param[in] task_name - a string to use as a name for the task.
 * @param[in] task_id - a unique integer identifying the task.
 * @param[in] task_function - a function pointer to use when spawning the task.
 * @param[in] task_argument - a single argument to provide to the start function.
 * @param[in] period - the periodic at which to run the task. This period is in system
 *                     clock ticks. Use TM_SYSTEM_CLOCK_TICKS_PER_SECOND and
 *                     TM_SYSTEM_CLOCK_TICKS_PER_SLOT to get the clock ticks when
 *                     calculating appropriate periods.
 * @param[in] heartbeat_period - the number of schedule slots from the scheduling of
 *                               this task until it must provide a heartbeat (this can
 *                               be the same as the period).
 * @param[in] priority - the task priority.
 */
TM_RESULT_ENUM tm_periodic_task(char *task_name,
                                TM_TaskId task_id,
                                OS_TASK_FUNC *task_function,
                                void *task_argument,
                                int period,
                                int heartbeat_period,
                                int stack_size,
                                int priority);

/**
 * @brief tm_event_task
 *
 * This task registers a task which schedules itself.
 *
 * @param[in] task_name - a string to use as a name for the task.
 * @param[in] task_id - a unique integer identifying the task.
 * @param[in] task_function - a function pointer to use when spawning the task.
 * @param[in] task_argument - a single argument to provide to the start function.
 * @param[in] stack_size - the stack size to give to the task when it is spawned
 * @param[in] heartbeat_period - the maximum number of ticks between heartbeats
 *                               for this task.
 * @param[in] priority - the task priority.
 */
TM_RESULT_ENUM tm_event_task(char *task_name,
                             TM_TaskId task_id,
                             OS_TASK_FUNC *task_function,
                             void *task_argument,
                             int heartbeat_period,
                             int stack_size,
                             int priority);

/**
 * @brief tm_callback_task
 *
 * This task registers a task which is run as a callback in the Task Manager
 * module's Scheduler task. These tasks are scheduled based on a provided period,
 * in schedule slot increments. If the TM_SYSTEM_CLOCK_TICKS_PER_SLOT is 1,
 * then this is the number of system clock ticks between when this callback is run.
 *
 * @param[in] task_name - a string to use as a name for the task.
 * @param[in] task_id - a unique integer identifying the task.
 * @param[in] task_function - a function pointer to use when spawning the task.
 * @param[in] task_argument - a single argument to provide to the start function.
 * @param[in] period - the period at which to run the registered callback.
 */
TM_RESULT_ENUM tm_callback_task(char *task_name,
                                TM_TaskId task_id,
                                OS_TASK_FUNC *task_function,
                                void *task_argument,
                                int period);

/**
 * @brief tm_monitor_task
 *
 * This task registers a task which will not be scheduled by the Task Manager,
 * and will not provide a heartbeat, but will still has its status checked.
 * This status will be the operating system status, indicating whether the
 * task has exited or caused a fault/exception.
 *
 * @param[in] task_name - a string to use as a name for the task.
 * @param[in] task_id - a unique integer identifying the task.
 */
TM_RESULT_ENUM tm_monitor_task(char *task_name, TM_TaskId task_id);

/**
 * @brief tm_scheduler_task
 *
 * This function is the Task Manager module's scheduler task.
 * This task monitors all registered tasks, and will call task
 * callbacks at their registered rate, and unblock scheduled
 * tasks at their registered rate.
 *
 * @param[in] argument - this is an unused argument that is required
 *                       by the type of a scheduled task.
 */
void tm_scheduler_task(void *argument);

/**
 * @brief tm_running
 *
 * This function checks whether a task should continue to run, or
 * should exit cleanly.
 * This function will block rate tasks until they are ready to be
 * run. For other tasks, this function will return without blocking.
 *
 * @param[in] task_id - the task id of the current task. This is used to
 *            determine the task type.
 *
 * @return either true, indicating that the task should continue to run,
 * or false indicating that it should shut down cleanly.
 */
bool tm_running(TM_TaskId task_id);

/**
 * @brief tm_stop
 *
 * This function stops the task scheduler. This will cause the task
 * scheduler to attempt a clean shutdown of all tasks the next time it
 * runs.
 */
void tm_stop(void);

/**
 * @brief tm_start
 *
 * This function starts the Task Manager schedule. It spawns all tasks,
 * and starts the timer which driver the schedule.
 *
 * @return a TM_RESULT_ENUM that indicating either a successful start,
 * or indicates an error.
 * TM_RESULT_TIMER_ERROR indicates that the schedule timer could not be started.
 * TM_RESULT_TASK_SPAWN_ERROR indicates that one or more tasks failed.
 *
 * Note that if the timer fails, then this will be returned even if one or more
 * task fails to spawn. This is in order to attempt to start the system even if
 * there are some failures.
 */
TM_RESULT_ENUM tm_start(void);

/**
 * @brief tm_get_status
 *
 * This function provides the status of the Task Manager module.
 * It does not provide a return- if a null pointer is provided, then
 * it will do nothing.
 */
void tm_get_status(TM_Status *status);

#endif // ndef __TM_INTERFACE_H__ */
