/**
 * @file tlm.c
 *
 * @author Noah Ryan
 *
 * This file contains the implementation of the Telemetry module functions.
 */
#include "fsw_interface.h"
#include "tm_interface.h"

#include "tlm_definitions.h"
#include "tlm_interface.h"


TLM_State gvTLM_state;


FSW_RESULT_ENUM tlm_initialize(void)
{
	FSW_RESULT_ENUM result = FSW_RESULT_OKAY;

	memset(&gvTLM_state, 0, sizeof(gvTLM_state));

	TM_RESULT_OKAY tm_result =
		tm_rate_task(TM_TASK_NAME_TLM,
		             tlm_telemetrytask,
		             TM_NO_ARGUMENT,
			     TM_TASK_RATE_TLM,
		             TM_PRIORITY_TLM_TASK);

	if (tm_result != TM_RESULT_OKAY)
	{
		result = FSW_RESULT_TASK_REGISTRATION_ERROR;
	}

	return result;
}

void tlm_telemetry_task(void)
{
	TLM_TelemetryMessage telemetry = {0};

	while (tm_running())
	{
		tlm_get_status(&telemetry.tlm);
		mb_get_status(&telemetry.mb);
		em_get_status(&telemetry.em);
		tbl_get_status(&telemetry.tbl);
		tm_get_status(&telemetry.tm);

		// return value not checked because the message cannot be null.
		msg_telemetry_message(&telemetry,
                                      MSG_PACKETTYPE_TELEMETRY,
                                      sizeof(TLM_TelemetryMessage));

		MB_RESULT_ENUM mb_result = mb_send(&telemetry.header, FSW_NO_WAIT);

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
		memset(telemetry.telemetry, 0, sizeof(telemetry.telemetry));
	}
}

