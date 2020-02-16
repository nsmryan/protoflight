/**
 * @file os_timer.h
 *
 * @author Noah Ryan
 *
 * This file contains definitions for the timer OS abstraction
 * used by the fsw.
 */
#ifndef __OS_TIMER_H__
#define __OS_TIMER_H__

#include "stdbool.h"

#include <signal.h>

#include "os_definitions.h"


#define OS_MAX_TIMERS 32


typedef bool (OS_TIMER_FUNC)(void *argument);

typedef struct OS_Timer
{
  timer_t timer;
  int signal;
  OS_TIMER_FUNC *callback;
  void *argument;
  OS_Timeout timeout;
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
OS_RESULT_ENUM os_timer_create(OS_Timer *timer);

/**
 * This function starts a timer, given a pointer to a timer
 * created by os_timer_create, a callback, and a timeout.
 *
 * Note that 
 *
 * @param[in] timer - a pointer to a timer.
 * @param[in] function - the callback function to run when the timer executes.
 * @param[in] argument - the argument that will be passed to the 'callback' function
 *                       when the timer triggers.
 * @param[in] timeout - the time that will elapsed before the timer executes.
 *
 * @return A value of type OS_TASK_STATUS_ENUM, indicating whether the timer
 * was started successfully.
 */
OS_RESULT_ENUM os_timer_start(OS_Timer *timer,
                              OS_TIMER_FUNC *callback,
                              void *argument,
                              OS_Timeout timeout);

/**
 * This function starts a timer, given a pointer to a timer
 * created by os_timer_create, a callback, and a timeout.
 *
 * Note that 
 *
 * @param[in] timer - a pointer to a timer.
 *
 * @return A value of type OS_TASK_STATUS_ENUM, indicating whether the
 * timer was stopped succesfully.
 */
OS_RESULT_ENUM os_timer_stop(OS_Timer *timer);

#endif // ndef __OS_TIMER_H__ */
