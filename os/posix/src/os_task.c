/**
 * @file os_task.c
 *
 * @author Noah Ryan
 * 
 * This file contains the implementation for the OS task abstraction used
 * by the fsw.
 */
#include "stdlib.h"
#include "stdint.h"
#include "string.h"

#include "signal.h"
#include "errno.h"
#include "unistd.h"
#include "sys/types.h"

#include "pthread.h"

#include "os_definitions.h"
#include "os_task.h"


typedef struct OS_Task_Arg
{
    void *argument;
    OS_TASK_FUNC *function;
} OS_Task_Arg;


void *os_task_pthread_function(void *argument)
{
    OS_Task_Arg *task_arg = (OS_Task_Arg*)argument;

    if ((task_arg != NULL) && (task_arg->function != NULL))
    {
        // the task argument is not checked for NULL- a task that does not use
        // the argument may have NULL passed in.
        task_arg->function(task_arg->argument);
    }

    return NULL;
}

OS_RESULT_ENUM os_task_spawn(OS_Task *task,
        OS_TASK_FUNC function,
        void *argument,
        int priority,
        int stack_size)
{
    OS_RESULT_ENUM result = OS_RESULT_OKAY;

    // return code from pthread functions
    int ret_code = 0;

    pthread_attr_t attr;

    // check input parameters for NULL pointers
    if ((task == NULL) || (function == NULL))
    {
        result = OS_RESULT_NULL_POINTER;
    }

    if (result == OS_RESULT_OKAY)
    {
        if ((stack_size == 0) || (priority < 0))
        {
            result = OS_RESULT_INVALID_ARGUMENTS;
        }
    }

    if (result == OS_RESULT_OKAY)
    {
        if (priority > (SIGRTMAX - SIGRTMIN))
        {
            result = OS_RESULT_INVALID_ARGUMENTS;
        }
    }

    // initialize the pthread attributes
    if (result == OS_RESULT_OKAY)
    {
        ret_code = 0;

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

            // the priority is swapped from low numbers as high priority to high numbers as
            // high priority to comply with POSIX RT priorities.
            // The range of priority was checked above, so this subtraction cannot be < 0.
            priority_struct.sched_priority = SIGRTMAX - priority;

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
        OS_Task_Arg *pthread_argument =
            (OS_Task_Arg*)calloc(1, sizeof(OS_Task_Arg));

        pthread_argument->argument = argument;
        pthread_argument->function = function;

        ret_code = pthread_create(task, &attr, os_task_pthread_function, pthread_argument);

        if (ret_code < 0)
        {
            result = OS_RESULT_ERROR;
        }
    }

    // detach the thread, cleaning up resources (taken from NASA OSAL)
    if (result == OS_RESULT_OKAY)
    {
        ret_code = pthread_detach(*task);

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

OS_TASK_STATUS_ENUM os_task_status(OS_Task *task)
{
    (void)task;
    OS_TASK_STATUS_ENUM task_status = OS_TASK_STATUS_OKAY;

    // NOTE no implementation appears possible for POSIX?
    // may need to track tasks, such as setitng a flag on exit

    return task_status;
}

OS_TASK_STATUS_ENUM os_task_delay(OS_Timeout timeout)
{
    OS_RESULT_ENUM result = OS_RESULT_OKAY;

    struct timespec timespec_timeout;

    int ret_code = clock_gettime(CLOCK_MONOTONIC, &timespec_timeout);

    uint64_t timeout_ns = timeout * OS_CONFIG_CLOCK_TICK_NANOSECONDS;

    timespec_timeout.tv_sec  += timeout_ns / 1000000000;
    timespec_timeout.tv_nsec += timeout_ns % 1000000000;

    // drain down the timeout, even if the sleep is interrupted by signal
    ret_code =
        clock_nanosleep(CLOCK_MONOTONIC,
                TIMER_ABSTIME,
                &timespec_timeout,
                &timespec_timeout);
    while (ret_code == EINTR)
    {
        ret_code =
            clock_nanosleep(CLOCK_MONOTONIC,
                    TIMER_ABSTIME,
                    &timespec_timeout,
                    &timespec_timeout);
    }

    if (ret_code != 0)
    {
        result = OS_RESULT_ERROR;
    }

    return result;
}
