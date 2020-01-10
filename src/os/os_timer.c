/**
 * @file os_timer.c
 *
 * @author Noah Ryan
 *
 * This file contains the implementation for the timer abstraction 
 * used by the fsw.
 */
#include "unistd.h"

#include "sys/types.h"
#include "sys/signal.h"
#include "sys/errno.h"

#include "os_definitions.h"
#include "os_timer.h"


int gvOS_currentSignal = SIGRTMIN;


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

    memset(&events, 0, sizeof(events));
    events.sigev_notify = SIGEV_SIGNAL;
    events.sigev_signo = gvOS_currentSignal;

    ret_code = timer_create(CLOCK_REALTIME, &events, &timer->timer);
    if (ret_code == 0)
    {
      timer->signal = gvOS_currentSignal;
    }
    else
    {
      result = OS_RESULT_OKAY;
    }
  }

  if (result == OS_RESULT_OKAY)
  {
    struct sigaction signal_action;

    memset(&signal_action, 0, sizeof(signal_action));

    sigemptyset(&signal_action);
    signal_action.sa_handler = callback;
    signal_action.sa_flags = SA_RESTART;

    ret_code = sigaction(gvOS_currentSignal, &signal_action, NULL);

    if (ret_code < 0)
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

OS_RESULT_ENUM os_timer_start(OS_Timer *timer, OS_Timer function, OS_Timeout timeout)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;

  int ret_code = 0;

  if ((timer == NULL) || (function == NULL))
  {
    result = OS_RESULT_NULL_POINTER;
  }

  if (result == OS_RESULT_OKAY)
  {
    struct sigevent signal_action;
    ret_code = sigaction(timer->signal, NULL, &signal_action);

    if (ret_code == 0)
    {
      signal_action.sa_handlder = callback;

      ret_code = sigaction(gvOS_currentSignal, &signal_action, NULL);
      if (ret_code < 0)
      {
        result = OS_RESULT_OKAY;
      }
    }
    else
    {
      result = OS_RESULT_OKAY;
    }
  }

  if (result == OS_RESULT_OKAY)
  {
    struct timespec timeout_spec;

    int nanoseconds = timeout * OS_CONFIG_CLOCK_TICK_NANOSECONDS;
    timeout_spec.tv_sec = nanoseconds % OS_NANOSECONDS_PER_SECOND;
    timeout_spec.tv_nsec = nanoseconds / OS_NANOSECONDS_PER_SECOND;;

    timer_settime(timer, 0, &timeout_spec, NULL);
  }

  return result;
}

