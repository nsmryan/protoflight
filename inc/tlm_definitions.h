/**
 * @file tlm_definitions.h
 *
 * @author Noah Ryan
 *
 * This file contains the definition for the Telemetry module.
 */
#include "stdint.h"


/**
 * This definition is for the telemetry error message from the
 * Telemetry module. This indicates that a telemetry packet could
 * not be sent on the Message Bus.
 */
#define TLM_EVENT_ID_TLM_ERROR 1


/**
 * This enum is the result enum for the Telemetry module.
 * It is used to indicate a successful result (TLM_RESULT_OKAY)
 * or provides and error code indicating the cause of an error.
 */
typedef enum
{
	TLM_RESULT_INVALID = 0, /*<< Invalid result */
	TLM_RESULT_OKAY    = 1, /*<< Success */
	TLM_RESULT_NUM_RESULTS /**< The number of Telemetry module result types */
} TLM_RESULT_ENUM;

/**
 * This struct is the status structure for the Telemetry module.
 */
typedef struct
{
	uint32_t telemetry_sent;
	uint32_t telemetry_errors;
} TLM_Status;

/**
 * This struct is the state struct of the Telemetry module.
 */
typedef struct
{
	TLM_Status status;
} TLM_State;

/**
 * This struct contains the status structure of all modules in the flight
 * software.
 */
typedef struct
{
	TLM_Status tlm;
	MB_Status  mb;
	EM_Status  em;
	TBL_Status tbl;
	TM_Status  tm;
} TLM_Telemetry;

/**
 * This struct is the telemetry packet containing status of all
 * modules in the flight software.
 */
typedef struct
{
	MSG_Header header;
	TLM_Telemetry telemetry;
} TLM_TelemetryMessage;

