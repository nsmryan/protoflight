/**
 * @file tlm_interface.h
 *
 * @author Noah Ryan
 *
 * This file contains the interface for the Telemetry module.
 */
#include "tlm_definitions.h"


/**
 * This is the initialization function for the Telemetry module.
 */
FSW_RESULT_ENUM tlm_initialize(void);

/**
 * This is the main task of the Telemetry module.
 * It is responsible for sending the health and status packet
 * to the Message Bus.
 */
void tlm_telemetry_task(void);

