/**
 * @file mb.c
 *
 * @brief Message Bus Module Implementation
 *
 * @author Noah Ryan
 *
 * This file contains the implementation of the Message Bus module functions.
 */
#include "stddef.h"
#include "stdlib.h"
#include "string.h" // for memset

#include "fsw_definitions.h"
#include "msg_definitions.h"
#include "em_interface.h"

#include "mb_definitions.h"
#include "mb_interface.h"


/**
 * The module state for the Message Bus module
 */
MB_State gvMB_state = {0};


FSW_RESULT_ENUM mb_initialize(void)
{
  FSW_RESULT_ENUM result = FSW_RESULT_OKAY;

  return result;
}

MB_RESULT_ENUM mb_send(MSG_Header *message, OS_Timeout timeout)
{
  MB_RESULT_ENUM result = MB_RESULT_OKAY;

  gvMB_state.status.messages_received++;

  if (message == NULL)
  {
    result = MB_RESULT_NULL_POINTER;
    gvMB_state.status.message_errors++;

    em_event(FSW_MODULEID_MB,
             FSW_EVENT_NULL_POINTER,
             __LINE__,
             0, 0, 0, 0, 0);

  }

  if (result == MB_RESULT_OKAY)
  {
    // the message size is the size of the data section (the length field)
    // plus the size of the header itself.
    uint32_t msg_size = message->length + sizeof(MSG_Header);

    MSG_PACKETTYPE_ENUM packet_type =
      (MSG_PACKETTYPE_ENUM)message->packet_type;

    for (uint16_t pipe_index = 0;
         pipe_index < gvMB_state.packets[packet_type].num_queues;
         pipe_index++)
    {
      uint32_t pipe_index = gvMB_state.packets[packet_type].queues[pipe_index];
      OS_RESULT_ENUM os_result =
        os_queue_send(&gvMB_state.pipes[pipe_index],
                      (uint8_t*)message,
                      msg_size,
                      timeout);

      if (os_result != OS_RESULT_OKAY)
      {
        em_event(FSW_MODULEID_MB,
                 MB_EVENT_MESSAGE_NOT_SENT,
                 __LINE__,
                 (uint32_t)packet_type,
                 (uint32_t)pipe_index,
                 0, 0, 0);

        gvMB_state.status.message_errors++;
      }
    }
  }

  return result;
}

MB_RESULT_ENUM mb_receive(MB_Pipe pipe_id,
                          MSG_Header *message,
                          uint32_t *msg_size,
                          OS_Timeout timeout)
{
  MB_RESULT_ENUM result = MB_RESULT_OKAY;

  if ((message == NULL) || (msg_size == NULL))
  {
    result = MB_RESULT_NULL_POINTER;
  }

  if (result == MB_RESULT_OKAY)
  {
    if (pipe_id >= gvMB_state.num_pipes)
    {
      result = MB_RESULT_INVALID_ARGUMENTS;
    }
  }

  if (result == MB_RESULT_OKAY)
  {
    OS_RESULT_ENUM os_result =
      os_queue_receive(&gvMB_state.pipes[pipe_id],
                       (uint8_t*)message,
                       msg_size,
                       timeout);

    if (os_result != OS_RESULT_OKAY)
    {
      result = MB_RESULT_PIPE_READ_ERROR;
    }
  }

  return result;
}

MB_RESULT_ENUM mb_create_pipe(MB_Pipe *pipe,
                              uint32_t msg_size_bytes,
                              uint32_t num_msgs)
{
    MB_RESULT_ENUM result = MB_RESULT_OKAY;

    if (pipe == NULL)
    {
      result = MB_RESULT_NULL_POINTER;
    }

    if (result == MB_RESULT_OKAY)
    {
      if (gvMB_state.num_pipes >= MB_MAX_NUM_PIPES)
      {
        result = MB_RESULT_MAX_PIPES_REACHED;
      }
    }

    if (result == MB_RESULT_OKAY)
    {
      uint32_t next_pipe = gvMB_state.num_pipes;

      OS_RESULT_ENUM os_result =
        os_queue_create(&gvMB_state.pipes[next_pipe],
                        msg_size_bytes,
                        num_msgs);

    }

    if (result == MB_RESULT_OKAY)
    {
      *pipe = gvMB_state.num_pipes;

      gvMB_state.num_pipes++;
    }

    return result;
}

MB_RESULT_ENUM mb_register_packet(MB_Pipe pipe, MSG_PACKETTYPE_ENUM packet_type)
{
  MB_RESULT_ENUM result = MB_RESULT_OKAY;

  uint32_t next_queue = 0;

  if (pipe > gvMB_state.num_pipes)
  {
    result = MB_RESULT_INVALID_PIPE;
  }

  if (result == MB_RESULT_OKAY)
  {
    next_queue = gvMB_state.packets[packet_type].num_queues;

    if (next_queue >= MB_MAX_PIPES_PER_PACKET)
    {
      result = MB_RESULT_MAX_PIPES_REACHED;
    }
  }

  if (result == MB_RESULT_OKAY)
  {
    gvMB_state.packets[packet_type].queues[next_queue] = pipe;

    gvMB_state.packets[packet_type].num_queues++;
  }

  return result;
}

void mb_get_status(MB_Status *status)
{
  if (status != NULL)
  {
    *status = gvMB_state.status;
  }
}

