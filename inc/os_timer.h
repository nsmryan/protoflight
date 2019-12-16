/**
 * @file os_timer.h
 *
 * @author Noah Ryan
 *
 * This file contains definitions for the watchdog OS abstraction
 * used by the fsw.
 * The watchdog is simply a timer that does not reset automatically.
 * The terminology 'watchdog' instead of 'timer' comes from VxWorks APIs.
 */
#include "os_definitions.h"


typedef OS_Timer int;

typedef void (OS_TIMER_FUNC)(void);


OS_RESULT_ENUM os_timer_create(OS_Timer *watchdog);

OS_RESULT_ENUM os_timer_start(OS_timer watchdog, OS_TIMER_FUNC function, OS_Timeout timeout);

