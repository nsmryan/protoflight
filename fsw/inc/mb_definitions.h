/**
 * @file mb_definitions.h
 *
 * @author Noah Ryan
 *
 * This file contains definitions for the Message Bus module
 */
#ifndef __MB_DEFINITIONS_H__
#define __MB_DEFINITIONS_H__

#include "stdint.h"

#include "os_queue.h"

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


typedef uint32_t MB_Pipe;

/**
 * The MB_RESULT_ENUM is used as the return value from MB module functions.
 * It either indicates a successful return from a function, or it indicates the
 * reason for an error.
 */
typedef enum MB_RESULT_ENUM
{
    MB_RESULT_INVALID            = 0,  /*<< Invalid result */
    MB_RESULT_OKAY               = 1,  /*<< Successful */
    MB_RESULT_NULL_POINTER       = 2,  /*<< A null pointer was detected */
    MB_RESULT_MAX_PIPES_REACHED  = 3,  /*<< Attempted to register a new pipe, but the maximum number of pipes was reached */
    MB_RESULT_PIPE_CREATE_FAILED = 4,  /*<< Pipe creation failed */
    MB_RESULT_INVALID_ARGUMENTS  = 5,  /*<< Invalid arguments provided to function */
    MB_RESULT_PIPE_READ_ERROR    = 6,  /*<< Error reading from a pipe */
    MB_RESULT_INVALID_PIPE       = 7,  /*<< Pipe was an invalid value */
    MB_RESULT_TIMEOUT            = 8,  /*<< Time on read or write */
    MB_RESULT_SEND_ERROR         = 9,  /*<< Message send error */
    MB_RESULT_INVALID_PACKET_ID  = 10, /*<< Invalid packet id given */
    MB_RESULT_NUM_RESULTS             /*<< Number of result values for MB */
} MB_RESULT_ENUM;

/**
 * This structure contains the data related to pipes. This is used
 * when registering packet types with a pipe, and when sending
 * a message on a pipe.
 */
typedef struct MB_PacketData
{
  uint32_t num_queues;                       /*<< The number of queues used in the 'queues' array */
  uint32_t queues[MB_MAX_PIPES_PER_PACKET];  /*<< The array of queues associated with the packet that MB_PacketData tracks */
} MB_PacketData;

/**
 * This structure is the status of the Message Bus module.
 */
typedef struct MB_Status
{
	uint32_t messages_sent;            /*<< A count of the number of messages sent over MB */
	uint32_t message_sent_errors;      /*<< A count of errors when sending messages */
	uint32_t message_receive_errors;   /*<< A count of errors when receiving  messages */
    uint32_t send_error_packet_id;     /*<< The packet ID that caused the last send error */
    uint32_t send_error_pipe_index;    /*<<  The pipe index that caused the last send error */
    int32_t  send_error_code;          /*<< The error code returned from the os_queue on the last send error */
    uint32_t receive_error_pipe_id;    /*<< The pipe ID that caused the last receive error */
    int32_t  receive_error_code;       /*<< The error code returned from the os_queue on the last receive error */
} MB_Status;

/**
 * This struct is the state of the Message Bus module.
 */
typedef struct MB_State
{
  uint32_t num_pipes;                                 /*<< The number of allocated pipes in the 'pipes' array */
  OS_Queue pipes[MB_MAX_NUM_PIPES];                   /*<< The queues allocated to receive packets */
  MB_PacketData packets[MSG_PACKETID_NUM_PACKET_IDS]; /*<< The packet structures tracking which queues are used to receive which packets */
  MB_Status status;                                   /*<< The MB module status structure reported in health and status */
} MB_State;

#endif // ndef __MB_DEFINITIONS_H__ */
