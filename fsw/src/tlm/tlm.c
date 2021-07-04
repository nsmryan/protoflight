/**
 * @file tlm.c
 *
 * @author Noah Ryan
 *
 * This file contains the implementation of the Telemetry module functions.
 */
#include "stddef.h"
#include "string.h"
#include "stdio.h"

#include "fsw_definitions.h"
#include "fsw_tasks.h"
#include "em_interface.h"
#include "tm_interface.h"
#include "msg_interface.h"
#include "mb_interface.h"
#include "tbl_interface.h"

#include "tlm_definitions.h"
#include "tlm_interface.h"


TLM_State gvTLM_state;


FSW_RESULT_ENUM tlm_initialize(void)
{
    FSW_RESULT_ENUM result = FSW_RESULT_OKAY;

    memset(&gvTLM_state, 0, sizeof(gvTLM_state));

    TM_RESULT_ENUM tm_result =
        tm_periodic_task(FSW_TASK_NAME_TLM,
                         FSW_TASK_ID_TLM,
                         tlm_telemetry_task,
                         FSW_TASK_NO_ARGUMENT,
                         FSW_TASK_RATE_1_HZ,
                         FSW_HEARBEAT_RATE_1_HZ,
                         FSW_DEFAULT_STACK_SIZE,
                         FSW_PRIORITY_TLM_TASK);

    if (tm_result != TM_RESULT_OKAY)
    {
        result = FSW_RESULT_TASK_REGISTRATION_ERROR;
    }

    return result;
}

void tlm_get_status(TLM_Status *status)
{
    if (status != NULL)
    {
        *status = gvTLM_state.status;
    }
}

void tlm_telemetry_task(void *argument)
{
    (void)argument;

    TLM_HealthAndStatusMessage telemetry = {0};

    while (tm_running(FSW_TASK_ID_TELEMETRY))
    {
        tlm_get_status(&telemetry.telemetry.tlm);
        mb_get_status(&telemetry.telemetry.mb);
        em_get_status(&telemetry.telemetry.em);
        tbl_get_status(&telemetry.telemetry.tbl);
        tm_get_status(&telemetry.telemetry.tm);

        // return value not checked because the message cannot be null.
        (void)msg_telemetry_message((MSG_Header*)&telemetry,
                                    MSG_PACKETTYPE_TELEMETRY,
                                    sizeof(TLM_HealthAndStatusMessage));

        MB_RESULT_ENUM mb_result = mb_send(&telemetry.header, OS_TIMEOUT_NO_WAIT);

        if (mb_result == MB_RESULT_OKAY)
        {
            gvTLM_state.status.telemetry_sent++;
        }
        else
        {
            gvTLM_state.status.telemetry_errors++;
            em_event(FSW_MODULEID_TLM,
                     TLM_EVENT_ID_TLM_ERROR,
                     __LINE__,
                     0, 0, 0, 0, 0);
        }

        // zero out the telemetry structure so it is ready for the next
        // set of telemetry definitions.
        // the return of memset is the source pointer, which cannot be NULL.
        (void)memset(&telemetry.telemetry, 0, sizeof(telemetry.telemetry));
    }
}

