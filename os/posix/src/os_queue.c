/**
 * @file os_queue.c
 *
 * @author Noah Ryan
 *
 * This file contains the implementation of the OS abstraction for message
 * queues.
 */
#if !defined(OS_WSL)

#include "stdint.h"
#include "stdio.h"

#include "fcntl.h"
#include "errno.h"

#include "sys/stat.h"

#include "mqueue.h"

#include "os_definitions.h"
#include "os_queue.h"


/**
 * This definition is the mode argument used when creating queues.
 */
#define OS_QUEUE_MODE (0733)

/**
 * This definition is for the priority used when sending messages on
 * a message queue. All messages used the same priority.
 */
#define OS_QUEUE_PRIORITY 1


/**
 * This global variable is the number of queues allocated.
 * It is used to provide a queue name for each created queue.
 */
static int gvOS_queue_num_queues = 0;


OS_RESULT_ENUM os_queue_create(OS_Queue *queue,
                               uint32_t num_msgs,
                               uint32_t msg_size_bytes)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;

  int ret_code = 0;

  char queue_name[OS_CONFIG_MAX_NAME_LENGTH];

  struct mq_attr attr;


  attr.mq_maxmsg = num_msgs;
  attr.mq_msgsize = msg_size_bytes;


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
    ret_code = sprintf(queue_name, "/Fsw_Queue_%d", gvOS_queue_num_queues);

    if (ret_code < 0)
    {
      result = OS_RESULT_ERROR;
    }
  }

  if (result == OS_RESULT_OKAY)
  {
    gvOS_queue_num_queues++;

    mqd_t temp_queue =
      mq_open(queue_name, O_RDWR | O_CREAT, OS_QUEUE_MODE, &attr);

    if (temp_queue != ((mqd_t) -1))
    {
      *queue = temp_queue;
    }
    else
    {
      result = OS_RESULT_QUEUE_CREATE_ERROR;
    }
  }

  return result;
}

OS_RESULT_ENUM os_queue_send(OS_Queue *queue,
                             uint8_t *buffer,
                             uint32_t buffer_size_bytes,
                             OS_Timeout timeout)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;

  ssize_t msg_size = 0;

  struct timespec timeout_spec;

  if ((queue == NULL) || (buffer == NULL))
  {
    result = OS_RESULT_NULL_POINTER;
  }

  if (result == OS_RESULT_OKAY)
  {
    int nanoseconds = timeout * OS_CONFIG_CLOCK_TICK_NANOSECONDS;
    timeout_spec.tv_sec = nanoseconds / OS_NANOSECONDS_PER_SECOND;
    timeout_spec.tv_nsec = nanoseconds % OS_NANOSECONDS_PER_SECOND;;

    msg_size =
      mq_timedsend(*queue, (const char*)buffer, buffer_size_bytes, OS_QUEUE_PRIORITY, &timeout_spec);

    if (msg_size < 0)
    {
      if (errno == ETIMEDOUT)
      {
        result = OS_RESULT_TIMEOUT;
      }
      else if (errno == EMSGSIZE)
      {
        result = OS_RESULT_MSG_SIZE_ERROR;
      }
      else
      {
        result = OS_RESULT_ERROR;
      }
    }
  }

  return result;
}

OS_RESULT_ENUM os_queue_receive(OS_Queue *queue,
                                uint8_t *buffer,
                                uint32_t *buffer_size_bytes,
                                OS_Timeout timeout)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;

  ssize_t msg_size = 0;

  struct timespec timeout_spec;

  if ((queue == NULL) || (buffer_size_bytes == NULL))
  {
    result = OS_RESULT_NULL_POINTER;
  }

  if (result == OS_RESULT_OKAY)
  {
    int nanoseconds = timeout * OS_CONFIG_CLOCK_TICK_NANOSECONDS;
    timeout_spec.tv_sec = nanoseconds / OS_NANOSECONDS_PER_SECOND;
    timeout_spec.tv_nsec = nanoseconds % OS_NANOSECONDS_PER_SECOND;;

    int priority = OS_QUEUE_PRIORITY;
    msg_size =
      mq_timedreceive(*queue, (char*)buffer, *buffer_size_bytes, (unsigned*)&priority, &timeout_spec);

    if (msg_size > 0)
    {
      *buffer_size_bytes = msg_size;
    }
    else
    {
      if (msg_size == ETIMEDOUT)
      {
        result = OS_RESULT_TIMEOUT;
      }
      else if (msg_size == EMSGSIZE)
      {
        result = OS_RESULT_MSG_SIZE_ERROR;
      }
      else
      {
        result = OS_RESULT_ERROR;
      }
    }
  }

  return result;
}

#endif /* defined OS_WSL */
