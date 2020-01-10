/**
 * @file fsw_definitions.h
 *
 * @author Noah Ryan
 *
 * @brief General Flight Software definitions
 *
 *
 * This file contains general definitions used throughout
 * the flight software, but do not below to a particular module.
 */
#ifndef __FSW_DEFINITIONS_H__
#define __FSW_DEFINITIONS_H__

/* Generic Event Ids */
/**
 * This definition is the offset for event ids, which distinguishes generic
 * ids from module-specific ids.
 */
#define FSW_EVENT_BASE_ID 1000

/**
 * This event is for any function that detects a null pointer.
 */
#define FSW_EVENT_NULL_POINTER (FSW_EVENT_BASE_ID + 1)

/**
 * This event indicates a failure of the fsw to initialize
 * successfully.
 * The first two arguments are a bitflag indicating which
 * modules reported a failure in initialization, and
 * the third argument is a flag indicating whether the
 * Task Manager module was able to initialize all tasks.
 */
#define FSW_EVENT_INIT_ERROR (FSW_EVENT_BASE_ID + 2)

/**
 * Task ID for the telemetry module task
 */
#define FSW_TASK_ID_TELEMETRY 10

/**
 * The module id enum enumerates all modules within the FSW system.
 * When adding a new module, it should be given a module id in this enum.
 */
typedef enum
{
	FSW_MODULEID_INVALID = 0, /*<< Invalid module id */
	FSW_MODULEID_INIT    = 1, /*<< Initialization */
	FSW_MODULEID_EM      = 2, /*<< Event Message */
	FSW_MODULEID_MB      = 3, /*<< Message Bus */
	FSW_MODULEID_TLM     = 4, /*<< Telemetry */
	FSW_MODULEID_TM      = 5, /*<< Task Manager */
	FSW_MODULEID_TBL     = 6, /*<< Table */
	FSW_MODULEID_NUM_IDS      /*<< Number of modules */
} FSW_MODULEID_ENUM;

/**
 * The FSW_RESULT_ENUM is used to indicate successful function execution,
 * or provides an error indicator otherwise.
 */
typedef enum
{
	FSW_RESULT_INVALID                 = 0, /*<< Invalid result value */
	FSW_RESULT_OKAY                    = 1, /*<< Success */
	FSW_RESULT_TASK_REGISTRATION_ERROR = 2, /*<< Error when registering a task */
	FSW_RESULT_OS_TIMER_CREATE_ERROR   = 3, /*<< Error when creating a timer */
	FSW_RESULT_OS_SEM_CREATE_ERROR     = 4, /*<< Error when creating a semaphore */
	FSW_RESULT_NUM_RESULTS  /*<< Number of FSW result values */
} FSW_RESULT_ENUM;

#endif // ndef __FSW_DEFINITIONS_H__ */
