/**
 * @file tm.c
 *
 * @author Noah Ryan
 *
 * This file contains the implementation of Task Manager module functions.
 */
#include "stddef.h"
#include "stdbool.h"
#include "string.h"
#include "stdio.h"

#include "os_task.h"
#include "os_sem.h"

#include "fsw_tasks.h"

#include "tm_definitions.h"
#include "tm_interface.h"


TM_State gvTM_state = {0};


/**
 * This function updates a task's structure for a schedule slot tick.
 * It returns a value indicating whether the task is ready to be
 * scheduled, if it missed a heartbeat, or if the scheduler should
 * wait before acting on it.
 *
 * @param[in,out] task - the task structure to update
 * 
 * @return A value of type TM_TASKSTATUS_ENUM indicating what to
 *         do with the task.
 */
TM_TASKSTATUS_ENUM tm_update_task(TM_Task *task);

/**
 * @brief tm_process_task
 *
 * This function performs the processing for a particular task for
 * a particular schedule slot, either scheduling it to run, running
 * a callback, and checking the OS status of all tasks
 *
 * @param[in] status - the task status to act on
 * @param[in] task_id - the id of the task to process
 */
void tm_process_task(TM_TASKSTATUS_ENUM status, TM_TaskId task_id);

/**
 * @brief tm_schedule_callback
 *
 * This function is the callback used to start the Task Manager
 * schedule. This is run in a OS watchdog timer.
 *
 * @param[in] argument - this is unused, but is required by the type of
 *            the timer callback.
 *
 * @return a return of true will cause the timer to restart, and a return
 *         of false will stop the timer.
 */
bool tm_schedule_callback(void *argument);


FSW_RESULT_ENUM tm_initialize(void)
{
    FSW_RESULT_ENUM fsw_result = FSW_RESULT_OKAY;
    OS_RESULT_ENUM os_result = OS_RESULT_OKAY;

    TM_RESULT_ENUM tm_result = TM_RESULT_OKAY;


    memset(&gvTM_state, 0, sizeof(gvTM_state));
    gvTM_state.continue_running = true;


    tm_result =
        tm_periodic_task(FSW_TASK_NAME_TM_SCHEDULER,
                         FSW_TASK_ID_TM_SCHEDULER,
                         tm_scheduler_task,
                         NULL,
                         TM_SCHEDULER_PERIOD,
                         TM_SCHEDULER_PERIOD,
                         FSW_DEFAULT_STACKS_SIZE,
                         TM_SCHEDULER_PRIORITY);
    if (tm_result != TM_RESULT_OKAY)
    {
        fsw_result = FSW_RESULT_TASK_REGISTRATION_ERROR;
    }

    if (fsw_result == FSW_RESULT_OKAY)
    {
        os_result = os_sem_create(&gvTM_state.schedule_semaphore);

        if (os_result != OS_RESULT_OKAY)
        {
            fsw_result = FSW_RESULT_OS_SEM_CREATE_ERROR;
        }
    }

    if (fsw_result == FSW_RESULT_OKAY)
    {
        OS_RESULT_ENUM os_result = os_timer_create(&gvTM_state.schedule_timer);
        if (os_result != OS_RESULT_OKAY)
        {
            fsw_result = FSW_RESULT_OS_TIMER_CREATE_ERROR;
        }
    }

    return fsw_result;
}

bool tm_schedule_callback(void *argument)
{
    (void)argument;

    os_sem_give(&gvTM_state.schedule_semaphore);

    return true;
}

TM_RESULT_ENUM tm_start()
{
    TM_RESULT_ENUM tm_result = TM_RESULT_OKAY;
    OS_RESULT_ENUM os_result = OS_RESULT_OKAY;

    for (int task_id = 0; task_id < TM_MAX_TASKS; task_id++)
    {
        if ((gvTM_state.tasks[task_id].type == TM_TASKTYPE_PERIODIC) ||
            (gvTM_state.tasks[task_id].type == TM_TASKTYPE_EVENT))
        {
            os_result = 
                os_task_spawn(&gvTM_state.tasks[task_id].os_task,
                              gvTM_state.tasks[task_id].function,
                              gvTM_state.tasks[task_id].argument,
                              gvTM_state.tasks[task_id].priority,
                              gvTM_state.tasks[task_id].stack_size);
            if (os_result != OS_RESULT_OKAY)
            {
                tm_result = TM_RESULT_TASK_SPAWN_ERROR;
            }
        }
    }

    os_result = os_timer_start(&gvTM_state.schedule_timer,
                               tm_schedule_callback,
                               0,
                               TM_SYSTEM_CLOCK_TICKS_PER_SLOT);
    if (os_result != OS_RESULT_OKAY)
    {
        tm_result = TM_RESULT_TIMER_ERROR;
    }

    return tm_result;
}

bool tm_running(TM_TaskId task_id)
{
    if (gvTM_state.tasks[task_id].type == TM_TASKTYPE_PERIODIC)
    {
        os_sem_take(&gvTM_state.tasks[task_id].semaphore, OS_TIMEOUT_WAIT_FOREVER);
    }

    return gvTM_state.continue_running;
}

void tm_stop(void)
{
    gvTM_state.continue_running = false;
}

void tm_scheduler_task(void *argument)
{
    (void)argument;

    while (gvTM_state.continue_running)
    {
        OS_RESULT_ENUM os_result =
            os_sem_take(&gvTM_state.schedule_semaphore, OS_TIMEOUT_WAIT_FOREVER);

        if (os_result == OS_RESULT_OKAY)
        {
            for (int task_id = 0; task_id < TM_MAX_TASKS; task_id++)
            {
                // NOTE that the os specific task checking is missing
                // from this function
                OS_TASK_STATUS_ENUM task_status = 
                    os_task_status(&gvTM_state.tasks[task_id].os_task);
                if (task_status == OS_TASK_STATUS_OKAY)
                {
                    TM_TASKSTATUS_ENUM status =
                        tm_update_task(&gvTM_state.tasks[task_id]);

                    tm_process_task(status, task_id);
                }
                else
                {
                    // NOTE treat this like a missed heartbeat-
                    // task is in a bad state
                }
            }
        }
        else
        {
            // If the schedule cannot be run, shut down the system.
            gvTM_state.continue_running = false;
        }
    }

    // When shutting down, unblock all tasks to allow them to close cleanly
    for (int task_id = 0; task_id < TM_MAX_TASKS; task_id++)
    {
        if (gvTM_state.tasks[task_id].type == TM_TASKTYPE_PERIODIC)
        {
            // The return value is not checked here- the system is shutting
            // down so there is nothing to do to handle the error.
            os_sem_give(&gvTM_state.tasks[task_id].semaphore);
        }
    }
}

void tm_process_task(TM_TASKSTATUS_ENUM status, TM_TaskId task_id)
{
    switch (status)
    {
        case TM_TASKSTATUS_INVALID:
            // NOTE invalid- treat like missed heartbeat
            break;

        case TM_TASKSTATUS_WAIT:
            // do nothing- task does not need any action
            break;

        case TM_TASKSTATUS_SCHEDULE:
            if (gvTM_state.tasks[task_id].type == TM_TASKTYPE_PERIODIC)
            {
                OS_RESULT_ENUM os_status =
                    os_sem_give(&gvTM_state.tasks[task_id].semaphore);
                if (os_status == OS_RESULT_OKAY)
                {
                    gvTM_state.status.tasks_scheduled |=
                        (1ULL << task_id);
                }
                else
                {
                    // NOTE treat like missed heartbeat
                }
            }
            else if (gvTM_state.tasks[task_id].type == TM_TASKTYPE_CALLBACK)
            {
                if (gvTM_state.tasks[task_id].function == NULL)
                {
                    // NOTE unexpected situation, perhaps treat
                    // as missed heartbeat
                }
                else
                {
                    (*gvTM_state.tasks[task_id].function)(gvTM_state.tasks[task_id].argument);
                }
            }
            else
            {
                // NOTE indicate error
            }
            break;

        case TM_TASKSTATUS_MISSED_HEARTBEAT:
            // NOTE - missed heartbeat
            gvTM_state.status.tasks_missed_heartbeat |=
                (1ULL << task_id);
            break;

        case TM_TASKSTATUS_ERROR:
            // NOTE treat like a missed heartbeat
            break;
    }
}

TM_TASKSTATUS_ENUM tm_update_task(TM_Task *task)
{
    TM_TASKSTATUS_ENUM tm_status = TM_TASKSTATUS_INVALID;

    if (task == NULL)
    {
        tm_status = TM_RESULT_NULL_POINTER;
    }

    if (tm_status == TM_TASKSTATUS_INVALID)
    {
        task->ticks++;

        switch (task->type)
        {
            case TM_TASKTYPE_INVALID:
                tm_status = TM_TASKSTATUS_ERROR;
                break;

            case TM_TASKTYPE_PERIODIC:
                tm_status = TM_TASKSTATUS_WAIT;

                // periodic tasks are scheduled with the given period
                if (task->ticks == task->schedule_period)
                {
                    tm_status = TM_TASKSTATUS_SCHEDULE;
                    task->ticks = 0;
                }

                // if a heartbeat is missed, report instead of
                // scheduling
                if (task->ticks >= task->heartbeat_period)
                {
                    tm_status = TM_TASKSTATUS_MISSED_HEARTBEAT;
                }
                break;

            case TM_TASKTYPE_EVENT:
                tm_status = TM_TASKSTATUS_WAIT;

                // event tasks are only monitored for missed heartbeats
                if (task->ticks >= task->heartbeat_period)
                {
                    tm_status = TM_TASKSTATUS_MISSED_HEARTBEAT;
                }
                break;

            case TM_TASKTYPE_CALLBACK:
                tm_status = TM_TASKSTATUS_WAIT;

                // callback tasks are scheduled with the given
                // period.
                if (task->ticks == task->schedule_period)
                {
                    tm_status = TM_TASKSTATUS_SCHEDULE;
                    task->ticks = 0;
                }
                break;

            case TM_TASKTYPE_MONITOR:
                // monitor tasks are never scheduled, and never miss
                // heartbeats
                tm_status = TM_TASKSTATUS_WAIT;
                break;
        }
    }

    return tm_status;
}

TM_RESULT_ENUM tm_periodic_task(char *task_name,
                                TM_TaskId task_id,
                                OS_TASK_FUNC *task_function,
                                void *task_argument,
                                int period,
                                int heartbeat_period,
                                int stack_size,
                                int priority)
{
    TM_RESULT_ENUM tm_result = TM_RESULT_OKAY;

    if ((task_name == NULL) || (task_function == NULL))
    {
        tm_result = TM_RESULT_NULL_POINTER;
    }

    if (tm_result == TM_RESULT_OKAY)
    {
        if (period == 0)
        {
            tm_result = TM_RESULT_INVALID_ARGUMENT;
        }
    }

    if (tm_result == TM_RESULT_OKAY)
    {
        gvTM_state.tasks[task_id].type = TM_TASKTYPE_PERIODIC;
        gvTM_state.tasks[task_id].ticks = 0;
        gvTM_state.tasks[task_id].function = task_function;
        gvTM_state.tasks[task_id].argument = task_argument;
        gvTM_state.tasks[task_id].schedule_period = period;
        gvTM_state.tasks[task_id].heartbeat_period = heartbeat_period;
        gvTM_state.tasks[task_id].stack_size = stack_size;
        gvTM_state.tasks[task_id].priority = priority;

        // copy the task name, leaving space for a NULL terminator
        strncpy(gvTM_state.tasks[task_id].name, task_name, TM_MAX_TASK_NAME_LENGTH - 1);
        // NULL terminate the task name if it is the maximum length
        gvTM_state.tasks[task_id].name[TM_MAX_TASK_NAME_LENGTH - 1] = '\0';

        OS_RESULT_ENUM os_result =
            os_sem_create(&gvTM_state.tasks[task_id].semaphore);
        if (os_result == OS_RESULT_OKAY)
        {
            gvTM_state.num_tasks++;
        }
        else
        {
            tm_result = TM_RESULT_SEM_CREATE_ERROR;
        }
    }

    return tm_result;
}

TM_RESULT_ENUM tm_event_task(char *task_name,
                             TM_TaskId task_id,
                             OS_TASK_FUNC *task_function,
                             void *task_argument,
                             int heartbeat_period,
                             int stack_size,
                             int priority)
{
    TM_RESULT_ENUM tm_result = TM_RESULT_OKAY;

    if ((task_name == NULL) || (task_function == NULL))
    {
        tm_result = TM_RESULT_NULL_POINTER;
    }

    if (tm_result == TM_RESULT_OKAY)
    {
        gvTM_state.tasks[task_id].type = TM_TASKTYPE_EVENT;
        gvTM_state.tasks[task_id].ticks = 0;
        gvTM_state.tasks[task_id].function = task_function;
        gvTM_state.tasks[task_id].argument = task_argument;
        gvTM_state.tasks[task_id].schedule_period = 0;
        gvTM_state.tasks[task_id].heartbeat_period = heartbeat_period;
        gvTM_state.tasks[task_id].stack_size = stack_size;
        gvTM_state.tasks[task_id].priority = priority;

        // copy the task name, leaving space for a NULL terminator
        strncpy(gvTM_state.tasks[task_id].name, task_name, TM_MAX_TASK_NAME_LENGTH - 1);
        // NULL terminate the task name if it is the maximum length
        gvTM_state.tasks[task_id].name[TM_MAX_TASK_NAME_LENGTH - 1] = '\0';

        gvTM_state.num_tasks++;
    }

    return tm_result;
}

TM_RESULT_ENUM tm_callback_task(char *task_name,
                                TM_TaskId task_id,
                                OS_TASK_FUNC *task_function,
                                void *task_argument,
                                int period)
{
    TM_RESULT_ENUM tm_result = TM_RESULT_OKAY;

    if ((task_name == NULL) || (task_function == NULL))
    {
        tm_result = TM_RESULT_NULL_POINTER;
    }

    if (tm_result == TM_RESULT_OKAY)
    {
        gvTM_state.tasks[task_id].type = TM_TASKTYPE_CALLBACK;
        gvTM_state.tasks[task_id].ticks = 0;
        gvTM_state.tasks[task_id].function = task_function;
        gvTM_state.tasks[task_id].argument = task_argument;
        gvTM_state.tasks[task_id].schedule_period = period;
        gvTM_state.tasks[task_id].heartbeat_period = 0;
        gvTM_state.tasks[task_id].stack_size = 0;
        gvTM_state.tasks[task_id].priority = 0;

        // copy the task name, leaving space for a NULL terminator
        strncpy(gvTM_state.tasks[task_id].name, task_name, TM_MAX_TASK_NAME_LENGTH - 1);
        // NULL terminate the task name if it is the maximum length
        gvTM_state.tasks[task_id].name[TM_MAX_TASK_NAME_LENGTH - 1] = '\0';

        gvTM_state.num_tasks++;
    }

    return tm_result;
}

TM_RESULT_ENUM tm_monitor_task(char *task_name, int task_id)
{
    TM_RESULT_ENUM tm_result = TM_RESULT_OKAY;

    if (task_name == NULL)
    {
        tm_result = TM_RESULT_NULL_POINTER;
    }

    if (tm_result == TM_RESULT_OKAY)
    {
        gvTM_state.tasks[task_id].type = TM_TASKTYPE_MONITOR;
        gvTM_state.tasks[task_id].ticks = 0;
        gvTM_state.tasks[task_id].function = NULL;
        gvTM_state.tasks[task_id].argument = 0;
        gvTM_state.tasks[task_id].schedule_period = 0;
        gvTM_state.tasks[task_id].heartbeat_period = 0;
        gvTM_state.tasks[task_id].stack_size = 0;
        gvTM_state.tasks[task_id].priority = 0;

        // copy the task name, leaving space for a NULL terminator
        strncpy(gvTM_state.tasks[task_id].name, task_name, TM_MAX_TASK_NAME_LENGTH - 1);
        // NULL terminate the task name if it is the maximum length
        gvTM_state.tasks[task_id].name[TM_MAX_TASK_NAME_LENGTH - 1] = '\0';

        gvTM_state.num_tasks++;
    }

    return tm_result;
}

void tm_get_status(TM_Status *status)
{
    if (status != NULL)
    {
        *status = gvTM_state.status;
    }
}
