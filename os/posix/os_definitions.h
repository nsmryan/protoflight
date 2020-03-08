/**
 * @file os_definitions.h
 *
 * @author Noah Ryan
 *
 * This file contains definitions that are common to OS abstractions
 * used by the fsw.
 */
#ifndef __OS_DEFINITIONS_H__
#define __OS_DEFINITIONS_H__

#include "time.h" // included because of CLOCKS_PER_SEC

#include "semaphore.h"
#include "pthread.h"

#include "os_types.h"


/**
 * This definition is for the internal representation of a mutex
 * within the OS abstraction.
 */
typedef pthread_mutex_t OS_Mutex;

/**
 * This definition is the underlying data definition for queues.
 */
#if defined(OS_HAS_NO_QUEUES)
typedef struct OS_Queue
{
  OS_Mutex mutex;
  OS_Sem write_sem;
  OS_Sem read_sem;

  uint8_t *buffer;
  uint32_t *msg_sizes;

  uint32_t num_msgs;
  uint32_t msg_size_bytes;
  uint32_t read_offset;
  uint32_t write_offset;

  uint32_t num_queued;
} OS_Queue;
#else
#include "mqueue.h"

typedef mqd_t OS_Queue;
#endif

/**
 * This definition is for the internal representation of a semaphore
 * within the OS abstraction.
 */
typedef sem_t OS_Sem;

/**
 * The OS_Task type is the implementation dependant type for
 * tasks. This type is used as a pointer, allowing it to either
 * point to task data, or to a handle passed to OS functions.
 */
typedef pthread_t OS_Task;

/**
 * Ths OS_Timer type is the implementation dependant type for
 * timers. It is used to initialize, start, and stop a timer.
 */
typedef struct OS_Timer
{
  timer_t timer;
  int signal;
  OS_TIMER_FUNC callback;
  void *argument;
  OS_Timeout timeout;
} OS_Timer;

#endif // ndef __OS_DEFINITIONS_H__ */
