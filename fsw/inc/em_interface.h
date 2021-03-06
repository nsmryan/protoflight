/**
 * @file em_interface.h
 *
 * @author Noah Ryan
 *
 * @brief Event Message Interface
 *
 *
 * This file contains the interface of the Event Message Module.
 * This module provides a mechanism to send small messages indicating
 * events within the system.
 */
#ifndef __EM_INTERFACE_H__
#define __EM_INTERFACE_H__

#include "stdint.h"

#include "em_definitions.h"


/**
 * @brief EM_Initialize
 *
 * @author Noah Ryan
 *
 * This function initializes the Event Message module.
 */
FSW_RESULT_ENUM em_initialize(void);

/**
 * @brief EM_Event
 *
 * @author Noah Ryan
 *
 * This function sends an event message.
 */
void em_event(FSW_MODULEID_ENUM module_id,
              uint16_t event_id,
              uint32_t line_number,
              uint32_t param0,
              uint32_t param1,
              uint32_t param2,
              uint32_t param3,
              uint32_t param4);

/**
 * @brief em_get_status
 *
 * This function provides the Event module's status.
 * No status is returned- if a null pointer is provided, it
 * will do nothing.
 */
void em_get_status(EM_Status *status);

#endif // ndef __EM_INTERFACE_H__ */
