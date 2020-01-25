/**
 * @file os_timer.h
 *
 * @author Noah Ryan
 *
 * This file contains definitions for the watchdog OS abstraction
 * used by the fsw.
 * The watchdog is simply a timer that does not reset automatically.
 * The terminology 'watchdog' instead of 'timer' comes from VxWorks APIs.
 *
 * This implementation uses TinyCThreads for portability.
 */
#ifndef __OS_TIMER_H__
#define __OS_TIMER_H__

#include <signal.h>

#include "os_definitions.h"


#define OS_MAX_TIMERS 32


typedef void (OS_TIMER_FUNC)(void);

typedef struct OS_Timer
{
  timer_t timer;
  int signal;
  OS_TIMER_FUNC *callback;
} OS_Timer;


/**
 * This function creates (but does not start) a timer.
 * The timer is not given a timeout or handler function until
 * it is started, so this function simply configures the timer.
 *
 * NOTE This function is not re-entrant- it makes used of a global
 * variable to track the RT signal to use.
 *
 * @return A value of type OS_TASK_STATUS_ENUM, indicating the
 * current status of the task.
 */
OS_RESULT_ENUM os_timer_create(OS_Timer *watchdog);

/**
 * This function starts a timer, given a pointer to a timer
 * created by os_timer_create, a callback, and a timeout.
 *
 * @param[in] timer - a pointer to a timer.
 * @param[in] function - the callback function to run when the timer executes.
 * @param[in] timeout - the time that will elapsed before the timer executes.
 *
 * @return A value of type OS_TASK_STATUS_ENUM, indicating the
 * current status of the task.
 */
OS_RESULT_ENUM os_timer_start(OS_Timer *timer, OS_TIMER_FUNC function, OS_Timeout timeout);

#endif // ndef __OS_TIMER_H__ */