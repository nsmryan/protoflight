/**
 * @file os_task.h
 *
 * @author Noah Ryan
 * 
 * This file contains the definitions for the OS task abstraction used
 * by the fsw.
 */
#include "os_definitions.h"



typedef enum
{
	OS_TASK_STATUS_INVALID      = 0,
	OS_TASK_STATUS_OKAY         = 1,
	OS_TASK_STATUS_TASK_UNKNOWN = 2,
	OS_TASK_STATUS_TASK_CRASHED = 3,
	OS_TASK_STATUS_NUM_STATUS
} OS_TASK_STATUS_ENUM;

OS_RESULT_ENUM os_task_spawn(OS_Task *task,
		             OS_TASK_FUNC function,
		             int argument,
			     int priority,
			     int stack_size);

OS_TASK_STATUS_ENUM os_task_status(OS_Task task);
