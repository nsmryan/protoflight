/**
 * @file mb_definitions.h
 *
 * @author Noah Ryan
 *
 * This file contains definitions for the Message Bus module
 */
#include "stdint.h"

#include "fsw_definitions.h"
#include "msg_definitions.h"


/**
 * This definition controls the maximim number of
 * pipes that can exist within the system.
 */
#define MB_MAX_NUM_PIPES 100

/**
 * This definition controls the maximum number of
 * pipes that can be registered for a particular packet.
 */
#define MB_MAX_PIPES_PER_PACKET 10

/**
 * This event message indicates that a message could not be sent.
 * Its parameters indicate which message could not be sent, and
 * addition information to help determine the intended receiver.
 */
#define MB_EVENT_MESSAGE_NOT_SENT 1


/**
 * The MB_RESULT_ENUM is used as the return value from MB module functions.
 * It either indicates a successful return from a function, or it indicates the
 * reason for an error.
 */
typedef enum
{
    MB_RESULT_INVALID            = 0, /*<< Invalid result */
    MB_RESULT_OKAY               = 1, /*<< Successful */
    MB_RESULT_NULL_POINTER       = 2, /*<< A null pointer was detected */
    MB_RESULT_MAX_PIPES_REACHED  = 3, /*<< Attempted to register a new pipe, but the maximum number of pipes was reached */
    MB_RESULT_PIPE_CREATE_FAILED = 4, /*<< Pipe creation failed */
    MB_RESULT_INVALID_ARGUMENTS  = 5, /*<< Invalid arguments provided to function */
    MB_RESULT_NUM_RESULTS             /*<< Number of result values for MB */
} MB_RESULT_ENUM;

/**
 * This structure contains the data related to pipes. This is used
 * when registering packet types with a pipe, and when sending
 * a message on a pipe.
 */
typedef struct
{
  uint16_t num_pipes;
  OS_Queue pipes[MB_MAX_PIPES_PER_PACKET];
} MB_PipeData;

/**
 * This structure is the status of the Message Bus module.
 */
typedef struct
{
	uint32_t messages_received;
	uint32_t message_errors;
} MB_Status;

/**
 * This struct is the state of the Message Bus module.
 */
typedef struct
{
    FSW_Queue pipes[MB_MAX_NUM_PIPES];
    MB_PipeData packets[MSG_PACKETTYPE_NUM_TYPES];
    uint16_t num_pipes;
    MB_Status status;
} MB_State;

