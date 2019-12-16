/**
 * @file os_task.c
 *
 * @author Noah Ryan
 * 
 * This file contains the implementation for the OS task abstraction used
 * by the fsw.
 */
#include "os_definitions.h"
#include "os_task.h"


OS_RESULT_ENUM os_task_spawn(OS_Task *task,
		             OS_TASK_FUNC function,
		             int argument,
			     int priority,
			     int stack_size)
{
	// NOTE need implementation
}

OS_TASK_STATUS_ENUM os_task_status(OS_Task task)
{
	OS_TASK_STATUS_ENUM task_status = OS_TASK_STATUS_OKAY;

	return task_status;
}

