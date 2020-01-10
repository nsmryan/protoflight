/**
 * @file em.c
 *
 * @author Noah Ryan
 *
 * This file contains the implementation for the Event Message module.
 */
#include "stddef.h"
#include "stdint.h"
#include "string.h"

#include "fsw_definitions.h"

#include "msg_interface.h"
#include "mb_interface.h"

#include "em_definitions.h"
#include "em_interface.h"


EM_State gvEM_state = {0};


FSW_RESULT_ENUM em_initialize(void)
{
  FSW_RESULT_ENUM result = FSW_RESULT_OKAY;

  memset(&gvEM_state, 0, sizeof(gvEM_state));

  return result;
}

void em_event(FSW_MODULEID_ENUM module_id,
              uint16_t event_id,
              uint32_t line_number,
              uint32_t param0,
              uint32_t param1,
              uint32_t param2,
              uint32_t param3,
              uint32_t param4)
{
  MB_RESULT_ENUM mb_result = MB_RESULT_OKAY;

  gvEM_state.status.messages_received++;

  if (module_id < FSW_MODULEID_NUM_IDS)
  {
     EM_Event event;

    msg_telemetry_message(&event.header, MSG_PACKETID_EVENT, sizeof(EM_Event));

    event.module = module_id;
    event.line_number = line_number;
    event.params[0] = param0;
    event.params[1] = param1;
    event.params[2] = param2;
    event.params[3] = param3;
    event.params[4] = param4;

    mb_result = mb_send((MSG_Header*)&event, OS_TIMEOUT_NO_WAIT);

    if (mb_result == MB_RESULT_OKAY)
    {
       gvEM_state.status.messages_received++;
    }
    else
    {
       gvEM_state.status.message_errors++;
    }
    }
    else
    {
       gvEM_state.status.message_errors++;
    }
}

void em_get_status(EM_Status *status)
{
  if (status != NULL)
  {
    *status = gvEM_status;
  }
}
