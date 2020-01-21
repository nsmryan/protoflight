/**
 * @file os_timer.c
 *
 * @author Noah Ryan
 *
 * This file contains the implementation for the timer abstraction 
 * used by the fsw.
 */
#include "unistd.h"

#include "string.h"

#include <signal.h>
#include <errno.h>
#include "sys/types.h"

#include "os_definitions.h"
#include "os_timer.h"


/**
 * The signal to allocate to the next timer created.
 * Note that this starts at -1 so that it can be initialized
 * when the first timer is created.
 */
static int gvOS_currentSignal = 0;


static OS_Timer *gvOS_timers[OS_MAX_TIMERS];


void os_timer_function(int signal)
{
  int timer_index = signal - SIGRTMIN;

  if ((timer_index > 0) && (timer_index < OS_MAX_TIMERS))
  {
    OS_Timer *timer = gvOS_timers[signal - SIGRTMIN];

    if (timer != NULL)
    {
      if (timer->callback != NULL)
      {
        timer->callback();
      }
    }
  }

  // There is nothing to do in the error case- we are in a
  // signal handler.
}

OS_RESULT_ENUM os_timer_create(OS_Timer *timer)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;
  int ret_code = 0;

  if (timer == NULL)
  {
    result = OS_RESULT_NULL_POINTER;
  }

  if (result == OS_RESULT_OKAY)
  {
    if (gvOS_currentSignal == SIGRTMAX)
    {
      result = OS_RESULT_MAX_TIMERS_REACHED;
    }
  }

  if (result == OS_RESULT_OKAY)
  {
    struct sigevent events;

    // the result of memset is not checked- it returns the destination
    // pointer, which cannot be NULL.
    memset((void*)&events, 0, sizeof(events));

    events.sigev_notify = SIGEV_SIGNAL;
    events.sigev_signo = gvOS_currentSignal + SIGRTMIN;

    ret_code = timer_create(CLOCK_REALTIME, &events, &timer->timer);
    if (ret_code == 0)
    {
      timer->signal = gvOS_currentSignal + SIGRTMIN;

      gvOS_timers[gvOS_currentSignal] = timer;
    }
    else
    {
      result = OS_RESULT_ERROR;
    }
  }

  if (result == OS_RESULT_OKAY)
  {
    gvOS_currentSignal++;
  }

  return result;
}

OS_RESULT_ENUM os_timer_start(OS_Timer *timer, OS_TIMER_FUNC *callback, OS_Timeout timeout)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;

  int ret_code = 0;

  // set up the signal that this timer triggers
  struct sigaction signal_action;


  if ((timer == NULL) || (callback == NULL))
  {
    result = OS_RESULT_NULL_POINTER;
  }

  if (result == OS_RESULT_OKAY)
  {
    // the result of memset is not checked, as it returns the destination pointer
    // which cannot be NULL.
    memset((void*)&signal_action, 0, sizeof(signal_action));

    ret_code = sigemptyset((sigset_t*)&signal_action);
    if (ret_code < 0)
    {
      result = OS_RESULT_ERROR;
    }
  }

  if (result == OS_RESULT_OKAY)
  {
    // set the callback for the signal.
    signal_action.sa_handler = os_timer_function;

    // SA_RESTART restarts system functions which are interrupted by a signal, instead
    // of returning an error.
    signal_action.sa_flags = SA_RESTART;

    // update sigaction with current callback
    ret_code = sigaction(timer->signal,
                         (const struct sigaction * restrict)&signal_action,
                         NULL);
    if (ret_code < 0)
    {
      result = OS_RESULT_ERROR;
    }
  }

  if (result == OS_RESULT_OKAY)
  {
    // start the timer, setting it off after a given timeout period
    struct timespec timeout_spec;

    int nanoseconds = timeout * OS_CONFIG_CLOCK_TICK_NANOSECONDS;
    timeout_spec.tv_sec = nanoseconds / OS_NANOSECONDS_PER_SECOND;
    timeout_spec.tv_nsec = nanoseconds % OS_NANOSECONDS_PER_SECOND;;

    ret_code =
      timer_settime(timer,
                    0,
                    (const struct itimerspec * restrict)&timeout_spec,
                    NULL);

    if (ret_code < 0)
    {
      result = OS_RESULT_ERROR;
    }
  }

  if (result == OS_RESULT_OKAY)
  {
    timer->callback = callback;
  }

  return result;
}

