/**
 * @file main.c
 *
 * @author Noah Ryan
 *
 * This file contains a main function for the flight software.
 * This simply calls the initialization function for each module
 * in the protoflight system.
 */
#include "stdint.h"
#include "stdbool.h"

#include "fsw_tasks.h"
#include "tm_interface.h"
#include "em_interface.h"
#include "tlm_interface.h"
#include "mb_interface.h"


int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

	FSW_RESULT_ENUM fsw_result = FSW_RESULT_OKAY;

	bool initialize_success = true;

	// flags indicating failed module initializations.
	// this assumes there are <= 64 modules.
	uint64_t module_flags = 0ULL;

	bool tasks_init_success = false;

    TM_RESULT_ENUM tmResult;
    tmResult = tm_monitor_task(FSW_TASK_NAME_MAIN, FSW_TASK_ID_MAIN);
	if (tmResult != TM_RESULT_OKAY)
	{
		initialize_success = false;
		module_flags |= (1ULL << FSW_MODULEID_INIT);
	}

	fsw_result = tm_initialize();
	if (fsw_result != FSW_RESULT_OKAY)
	{
		initialize_success = false;
		module_flags |= (1ULL << FSW_MODULEID_TM);
	}

	fsw_result = em_initialize();
	if (fsw_result != FSW_RESULT_OKAY)
	{
		initialize_success = false;
		module_flags |= (1ULL << FSW_MODULEID_EM);
	}

	fsw_result = tlm_initialize();
	if (fsw_result != FSW_RESULT_OKAY)
	{
		initialize_success = false;
		module_flags |= (1ULL << FSW_MODULEID_TLM);
	}

	fsw_result = mb_initialize();
	if (fsw_result != FSW_RESULT_OKAY)
	{
		initialize_success = false;
		module_flags |= (1ULL << FSW_MODULEID_MB);
	}

	TM_RESULT_ENUM tm_result = tm_start();
	if (tm_result == TM_RESULT_OKAY)
	{
		tasks_init_success = true;
	}

	if (!initialize_success)
	{
		em_event(FSW_MODULEID_INIT,
			     FSW_EVENT_INIT_ERROR,
                 __LINE__,
			     (module_flags & 0xFFFF00000) >> 32,
			     (module_flags & 0x00000FFFF),
			     tasks_init_success,
			     0, 0);
	}

    while (tm_running(FSW_TASK_ID_MAIN))
    {
        os_task_delay(10);
    }

	return 0;
}

