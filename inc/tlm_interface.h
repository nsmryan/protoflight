/**
 * @file tlm_interface.h
 *
 * @author Noah Ryan
 *
 * This file contains the interface for the Telemetry module.
 */
#ifndef __TLM_INTERFACE_H__
#define __TLM_INTERFACE_H__

#include "tlm_definitions.h"


/**
 * This is the initialization function for the Telemetry module.
 */
FSW_RESULT_ENUM tlm_initialize(void);

/**
 * This is the main task of the Telemetry module.
 * It is responsible for sending the health and status packet
 * to the Message Bus.
 *
 * @param[in] unused - this argument is not used, but is provided
 * because all tasks registered with Task Manager take 1 argument.
 */
void tlm_telemetry_task(int unused);

/**
 * @brief tlm_get_status
 *
 * This function provides the status of the Telemetry module.
 * It does not provide a return- if a null pointer is provided, then
 * it will do nothing.
 */
void tlm_get_status(TLM_Status *status);

#endif // ndef __TLM_INTERFACE_H__ */
