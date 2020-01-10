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

  if (message != NULL)
  {
    // the message size is the size of the data section (the length field)
    // plus the size of the header itself.
    uint32_t msg_size = message->length + sizeof(MSG_Header);

    MSG_PACKETTYPE_ENUM packet_type =
      (MSG_PACKETTYPE_ENUM)message->packet_type;

    for (uint16_t pipe_index = 0;
         pipe_index < gvMB_state.packets[packet_type].num_pipes;
         pipe_index++)
    {
      MB_Pipe pipe = gvMB_state.packets[packet_type].pipes[pipe_index];
      OS_RESULT_ENUM os_result =
        os_queue_send(pipe,
                      message,
                      msg_size,
                      timeout);

      if (os_result == OS_RESULT_OKAY)
      {
        em_event(FSW_MODULEID_MB,
           MB_EVENT_MESSAGE_NOT_SENT,
           __LINE__,
           (uint32_t)packet_type,
           (uint32_t)pipe_index,
           (uint32_t)pipe,
           0, 0);
      }
      else
      {
        gvMB_state.status.message_errors++;
      }
    }
  }
  else
  {
    result = MB_RESULT_NULL_POINTER;
    gvMB_state.status.message_errors++;

    em_event(FSW_MODULEID_MB,
             FSW_EVENT_NULL_POINTER,
             __LINE__,
             0, 0, 0, 0, 0);

  }

  return result;
}

MB_RESULT_ENUM mb_receive(MB_Pipe pipe_id,
                          MSG_Header *message,
                          uint16_t msg_size,
                          OS_Timeout timeout)
{
  MB_RESULT_ENUM result = MB_RESULT_OKAY;

  if (message == NULL)
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
    os_queue_receive(pipe_id, message, msg_size, timeout);
  }

  return result;
}

MB_RESULT_ENUM mb_create_pipe(MB_Pipe *pipe, uint16_t msg_size_bytes, uint16_t num_msgs)
{
    MB_RESULT_ENUM result = MB_RESULT_OKAY;

    if (pipe != NULL)
    {
         if (gvMB_state.next_pipe < MB_MAX_NUM_PIPES)
   {
       OS_Queue queue_id;

             OS_RESULT_ENUM os_result =
         os_queue_create(&queue_id, msg_size_bytes, num_msgs);
       if (os_result == OS_RESULT_OKAY)
       {
         gvMB_state.pipes[gvMB_state.next_pipe] = queue_id;
       }
       else
       {
         result = MB_RESULT_PIPE_CREATE_FAILED;
       }
   }
   else
   {
     result = MB_RESULT_MAX_PIPES_REACHED;
   }
    }
    else
    {
        result = MB_RESULT_NULL_POINTER;
    }

    return result;
}

MB_RESULT_ENUM mb_register_packet(MB_Pipe pipe, MSG_PACKETTYPE_ENUM packet_type)
{
  MB_RESULT_ENUM result = MB_RESULT_OKAY;

  if ((pipe < gvMB_state.num_pipes) && (packet_type < MSG_PACKETTYPE_NUM_TYPES))
  {
    uint16_t pipe_index = gvMB_state.pipe_data[packet_type].num_pipes;

    gvMB_state.packets[packet_type].pipes[pipe_index] = pipe;

    gvMB_state.packets[packet_type].num_pipes++;
  }
  else
  {
    result = MB_RESULT_INVALID_ARGUMENTS;
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

