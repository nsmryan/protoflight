/**
 * @file fsw_tasks.h
 *
 * @author Noah Ryan
 *
 * This file contains definitions for all tasks within the fsw.
 */
#ifndef __FSW_TASKS_H__
#define __FSW_TASKS_H__

/* General Task Definitions */
/**
 * This definition is the default stack size for FSW tasks.
 * This is used for all core modules tasks.
 */
#define FSW_DEFAULT_STACK_SIZE (1024 * 20)

/**
 * This definition is used when providing the task argument for a task
 * that does not use its argument.
 */
#define FSW_TASK_NO_ARGUMENT 0


/* Task Names */
#define FSW_TASK_NAME_MAIN "Main"
#define FSW_TASK_NAME_TLM "Telemetry"
#define FSW_TASK_NAME_TM_SCHEDULER "TmScheduler"

/* Task Rates */
/**
 * This definition is the period of a 1 Hz task.
 */
#define FSW_TASK_RATE_1_HZ TM_SYSTEM_CLOCK_TICKS_PER_SECOND 


/**
 * This definition is the heartbeat rate of a task that checks in
 * at 1 Hz.
 */
#define FSW_HEARBEAT_RATE_1_HZ TM_SYSTEM_CLOCK_TICKS_PER_SECOND 


/* Task Priorities */
/**
 * This definition is the task priority of the Telemetry task.
 */
#define FSW_PRIORITY_TLM_TASK 25

/**
 * This definition is the task priority of the Task Scheduler task.
 */
#define TM_SCHEDULER_PRIORITY 1

/* Task Ids */
/**
 * This definition is the task id for the main task that starts
 * the FSW process.
 */
#define FSW_TASK_ID_MAIN 1

/**
 * This definition is the task id for the TM Scheduler task.
 */
#define FSW_TASK_ID_TM_SCHEDULER 2

/**
 * This definition is the task id for the Telemetry task.
 */
#define FSW_TASK_ID_TLM 3


#endif /* ndef __FSW_TASKS_H__ */
