/**
 * @file em_definitions.h
 *
 * @author Noah Ryan
 *
 * Event Message definitions
 */
#ifndef __EM_DEFINITIONS_H__
#define __EM_DEFINITIONS_H__

#include "stdint.h"

#include "fsw_definitions.h"
#include "msg_definitions.h"


/**
 * This definition is the number of parameters (param*) for the
 * em_event function.
 * This should match the number of parameters for the em_event.
 */
#define EM_NUM_PARAMS 5

/**
 * The EM_RESULT_ENUM gives the result type for Event Module functions.
 * It either indicates a success (EM_RESULT_OKAY) or provides an
 * error code indicating the cause of the problem.
 */
typedef enum
{
  EM_RESULT_INVALID           = 0,
  EM_RESULT_OKAY              = 1,
  EM_RESULT_COULD_NOT_SEND    = 2,
  EM_RESULT_INVALID_ARGUMENTS = 3,
  EM_RESULT_NUM_EVENTS
} EM_RESULT_ENUM;

/**
 * The event struct is the packet definition of for event messages.
 */
typedef struct
{
  MSG_Header header;
  FSW_MODULEID_ENUM module;
  uint16_t event_id;
  uint16_t line_number;
  uint32_t params[EM_NUM_PARAMS];
} EM_Event;

/**
 * This struct is the Event Message module's status structure.
 */
typedef struct EM_Status
{
  uint32_t messages_received;
  uint32_t messages_sent;
  uint32_t message_errors;
} EM_Status;

/**
 * This struct provides the Event Message module's state
 */
typedef struct EM_State
{
  EM_Status status;
} EM_State;

#endif // ndef __EM_DEFINITIONS_H__ */
