/**
 * @file os_queue_udp.c
 *
 * @author Noah Ryan
 *
 * This file contains the implementation of the OS abstraction for message
 * queues. This implementation uses UDP sockets for systems that do not have
 * librt implemented.
 */
#if defined(OS_QUEUE_UDP)
#include "stdint.h"
#include "stdio.h"

#include "fcntl.h"
#include "errno.h"

#include "sys/stat.h"

#include "mqueue.h"

#include "os_definitions.h"
#include "os_queue.h"


/**
 * This definition is the base port used when allocating UDP ports for queues.
 * This is offset by the value of gvOS_queue_num_queues when the queue is created.
 */
#define OS_QUEUE_UDP_BASE_PORT 45000


/**
 * This definition fills in the OS_Queue type in os_queue.h to the implementation
 * specific queue type.
 */
typedef struct OS_Queue
{
  int socket;
  int id;
} OS_Queue;


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

  int sock = 0;

  struct sockaddr_t addr;


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
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
      result = OS_RESULT_ERROR;
    }
  }

  if (result == OS_RESULT_OKAY)
  {
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(OS_QUEUE_UDP_BASE_PORT + gvOS_queue_num_queues);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    ret_code = bind(sock, (struct sockaddr)&addr, sizeof(addr));
    if (ret_code == 0)
    {
      queue->socket = sock;
      queue->id = gvOS_queue_num_queues;
      gvOS_queue_num_queues++;
    }
    else
    {
      result = OS_RESULT_ERROR;
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

  struct sockaddr_in addr;

  struct timespec timeout_spec;

  int ret_code = 0;


  if ((queue == NULL) || (buffer == NULL))
  {
    result = OS_RESULT_NULL_POINTER;
  }

  if (result == OS_RESULT_OKAY)
  {
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(OS_QUEUE_UDP_BASE_PORT + queue->id);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    int nanoseconds = timeout * OS_CONFIG_CLOCK_TICK_NANOSECONDS;
    timeout_spec.tv_sec = nanoseconds % OS_NANOSECONDS_PER_SECOND;
    timeout_spec.tv_nsec = nanoseconds / OS_NANOSECONDS_PER_SECOND;;

    int sock = socket(AD_AFINET, SOCK_DGRAM, 0);
    if (sock > 0)
    {
      ret_code =
        sendto(sock, (char*)buffer, buffer_size_bytes, 0, (struct sockaddr*)&addr, sizeof(addr));
      if (ret_code < 0)
      {
        result = OS_RESULT_ERROR;
      }

      close(socket);
    }
    else
    {
      result = OS_RESULT_ERROR;
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

  struct timespec timeout_spec;

  int ret_code = 0;


  if ((queue == NULL) || (buffer == NULL))
  {
    result = OS_RESULT_NULL_POINTER;
  }

  if (result == OS_RESULT_OKAY)
  {
    int nanoseconds = timeout * OS_CONFIG_CLOCK_TICK_NANOSECONDS;
    timeout_spec.tv_sec = nanoseconds % OS_NANOSECONDS_PER_SECOND;
    timeout_spec.tv_nsec = nanoseconds / OS_NANOSECONDS_PER_SECOND;;

    fd_set fdset;
    FD_ZERO(&fdset);
    FD_SET(*queue, &fdset);

    bool finished = false;
    while (!finished)
    {
      ret_code = select(1, &fdset, NULL, NULL, &timeout_spec);

      // select returns the number of descriptors that are ready, so 1 indicates that
      // the socket has a message.
      if (ret_code == 1)
      {
        finished = true;
      }
      else if ((ret_code == -1) && (errno != EINTR))
      {
        // if the call had an error that was not just an interruption from a signal,
        // then finish with an error.
        finished = true;
        result = OS_RESULT_ERROR;
      }
    }
  }

  if (result == OS_RESULT_OKAY)
  {
    ret_code = recvfrom(queue->socket, buffer, *buffer_size_bytes, 0, NULL, NULL);
    if (ret_code > 0)
    {
      *buffer_size_bytes = ret_code;
    }
    else
    {
      result = OS_RESULT_ERROR;
    }
  }

  return result;
}

#endif /* !defined(OS_QUEUE_UDP) */
