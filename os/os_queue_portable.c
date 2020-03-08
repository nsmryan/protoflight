/**
 * @file os_queue_pthread.c
 *
 * @author Noah Ryan
 *
 * This file contains the implementation of the OS abstraction for message
 * queues.
 * This implementation uses only pthread to implement a queue. It is not
 * intended for high performance, but rather as a simple alternative queue
 * for systems that do not implement librt.
 */
#if defined(OS_QUEUE_PTHREAD)

#include "stdint.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdlib.h"
#include "string.h"

#include "fcntl.h"
#include "errno.h"

#include "sys/stat.h"

#include "pthread.h"

#include "os_definitions.h"
#include "os_mutex.h"
#include "os_queue.h"


OS_RESULT_ENUM os_queue_create(OS_Queue *queue,
                               uint32_t num_msgs,
                               uint32_t msg_size_bytes)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;

  int ret_code = 0;

  OS_Mutex mutex;

  OS_Sem read_sem;
  OS_Sem write_sem;

  uint8_t *buffer = NULL;
  uint32_t *msg_sizes = NULL;


  if (queue == NULL)
  {
    result = OS_RESULT_NULL_POINTER;
  }

  if (result == OS_RESULT_OKAY)
  {
    if ((num_msgs == 0) || (msg_size_bytes == 0))
    {
      result = OS_RESULT_INVALID_ARGUMENTS;
    }
  }

  if (result == OS_RESULT_OKAY)
  {
    result = os_mutex_create(&mutex);
  }

  if (result == OS_RESULT_OKAY)
  {
    // The return of memset is not checked as it is a pointer to
    // mutex_attr which cannot be NULL.
    memset(&cond_attr, 0, sizeof(cond_attr));

    result = os_sem_create(&read_sem);
  }

  if (result == OS_RESULT_OKAY)
  {
    result = os_sem_create(&write_sem);
  }

  // allocate space for messages
  if (result == OS_RESULT_OKAY)
  {
    buffer = (uint8_t*)malloc(num_msgs * msg_size_bytes);
    if (buffer == NULL)
    {
      result = OS_RESULT_ERROR;
    }
  }

  // allocate space for message sizes
  if (result == OS_RESULT_OKAY)
  {
    msg_sizes = (uint32_t*)malloc(num_msgs * sizeof(uint32_t));
    if (msg_sizes == NULL)
    {
      result = OS_RESULT_ERROR;
    }
  }

  if (result == OS_RESULT_OKAY)
  {
    memset(queue, 0, sizeof(OS_Queue));

    queue->mutex = mutex;
    queue->write_sem = write_sem;
    queue->read_sem = read_sem;
    queue->num_msgs = num_msgs;
    queue->msg_size_bytes = msg_size_bytes;
    queue->buffer = buffer;
    queue->msg_sizes = msg_sizes;
    queue->read_offset = 0;
    queue->write_offset = 0;
    queue->num_queued = 0;
  }

  return result;
}

OS_RESULT_ENUM os_queue_send(OS_Queue *queue,
                             uint8_t *buffer,
                             uint32_t buffer_size_bytes,
                             OS_Timeout timeout)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;

  int ret_code = 0;

  struct timespec timeout_spec;

  // this flag is required to ensure that the queue mutex is always
  // released, even in error conditions.
  bool mutex_taken = false;

  // check input pointers
  if ((queue == NULL) || (buffer == NULL))
  {
    result = OS_RESULT_NULL_POINTER;
  }

  // check that the queue can hold the buffer
  if (result == OS_RESULT_OKAY)
  {
    if (buffer_size_bytes > queue->msg_size_bytes)
    {
      result = OS_RESULT_MSG_SIZE_ERROR;
    }
  }

  // get the current time, using the clock configured for this mutex
  if (result == OS_RESULT_OKAY)
  {
    ret_code = clock_gettime(CLOCK_MONOTONIC, &timeout_spec);
    if (ret_code < 0)
    {
      result = OS_RESULT_ERROR;
    }
  }

  // wait on the queue mutex
  if (result == OS_RESULT_OKAY)
  {
    // NOTE does rollover need to be checked here?
    uint64_t nanoseconds = timeout * OS_CONFIG_CLOCK_TICK_NANOSECONDS;
    timeout_spec.tv_sec += nanoseconds / OS_NANOSECONDS_PER_SECOND;
    timeout_spec.tv_nsec += nanoseconds % OS_NANOSECONDS_PER_SECOND;

    result = os_mutex_take(&queue->mutex, &timeout_spec);

    if (result == OS_RESULT_OKAY)
    {
      mutex_taken = true;
    }
  }

  // check if there is data in the queue
  if (result == OS_RESULT_OKAY)
  {
    // if write pointer is at read pointer, the queue is full
    if (queue->num_queued == queue->num_msgs)
    {
      // wait for the queue to empty by at least one
      result = os_sem_take(&queue->write_sem, &timeout_spec);
  }

  if (result == OS_RESULT_OKAY)
  {
    // store the message size
    queue->msg_sizes[queue->write_offset] = buffer_size_bytes;

    // either we took the mutex and there was space, or we were signaled by the write_condition
    // condition variable and took the mutex that way. Either way, there is space in the queue.
    int buffer_offset = queue->msg_size_bytes * queue->write_offset;
    memcpy(&queue->buffer[buffer_offset], buffer, buffer_size_bytes);

    // update the write pointer
    queue->write_offset = (queue->write_offset + 1) % queue->num_msgs;

    queue->num_queued++;

    // if any threads where blocking waiting for a message to arrive, signal them.
    // This also has the effect of releasing the queue mutex.
    result = os_sem_give(&queue->read_sem);
  }

  if (mutex_taken)
  {
    result = os_mutex_give(&queue->mutex);
  }

  return result;
}

OS_RESULT_ENUM os_queue_receive(OS_Queue *queue,
                                uint8_t *buffer,
                                uint32_t *buffer_size_bytes,
                                OS_Timeout timeout)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;

  int ret_code = 0;

  // this flag is necessary to ensure that we finish this function with the
  // queue mutex released.
  bool mutex_taken = false;

  struct timespec timeout_spec;

  if ((queue == NULL) || (buffer == NULL) || (buffer_size_bytes == NULL))
  {
    result = OS_RESULT_NULL_POINTER;
  }

  if (result == OS_RESULT_OKAY)
  {
    if (*buffer_size_bytes < queue->msg_size_bytes)
    {
      result = OS_RESULT_MSG_SIZE_ERROR;
    }
  }

  if (result == OS_RESULT_OKAY)
  {
    int nanoseconds = timeout * OS_CONFIG_CLOCK_TICK_NANOSECONDS;
    timeout_spec.tv_sec = nanoseconds / OS_NANOSECONDS_PER_SECOND;
    timeout_spec.tv_nsec = nanoseconds % OS_NANOSECONDS_PER_SECOND;;

    result = os_mutex_take(&queue->mutex, &timeout_spec);

    if (result == OS_RESULT_OKAY)
    {
      mutex_taken = true;
    }
  }

  if (result == OS_RESULT_OKAY)
  {
    // check if the queue is empty
    if (queue->num_queued == 0)
    {
      result = os_sem_take(&queue->read_sem, &timeout_spec);
    }
  }

  if (result == OS_RESULT_OKAY)
  {
    // update buffer size with the size stored for this message
    *buffer_size_bytes = queue->msg_sizes[queue->read_offset];

    // at this point, we either took the mutex and found a message, or we where signaled
    // indicating that a new message is available.
    int buffer_offset = queue->msg_size_bytes * queue->read_offset;

    // copy the message into the given buffer
    memcpy(buffer, &queue->buffer[buffer_offset], *buffer_size_bytes);

    // update the read pointer
    queue->read_offset = (queue->read_offset + 1) % queue->num_msgs;

    queue->num_queued--;

    // signal to any threads waiting for space to open up that a message was read.
    // This has the effect of releasing the queue mutex as well.
    result = os_sem_give(&queue->write_condition);
  }

  if (mutex_taken)
  {
    result = os_mutex_give(&queue->mutex);
  }

  return result;
}

#endif /* defined(OS_QUEUE_PTHREAD) */
