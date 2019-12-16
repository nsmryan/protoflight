/**
 * @file msg_definitions.h
 *
 * @author Noah Ryan
 *
 * @brief Message Module
 *
 * The message module contains defintiions for messages.
 */
#include "stdint.h"


/**
 * This enum provides the result value of Message module functions.
 * It either indicates either a succesful result (MSG_RESULT_OKAY)
 * or an error code indicating the cause of an error.
 */
typedef enum
{
	MSG_RESULT_INVALID      = 0,
	MSG_RESULT_OKAY         = 1,
	MSG_RESULT_NULL_POINTER = 2,
	MSG_RESULT_NUM_RESULTS
} MSG_RESULT_ENUM;

/**
 * The MSG_PACKETTYPE_ENUM indicates whether a message is a telemetry
 * message or a command message.
 */
typedef enum
{
  MSG_PACKETTYPE_INVALID   = 0,
  MSG_PACKETTYPE_COMMAND   = 1,
  MSG_PACKETTYPE_TELEMETRY = 2,
  MSG_PACKETTYPE_NUM_TYPES
} MSG_PACKETTYPE_ENUM

/**
 * This enum is the packet type of a message.
 */
typedef enum
{
    MSG_PACKETID_HEALTHANDSTATUS = 1,
    MSG_PACKETID_EVENT           = 2,
    MSG_PACKETID_NUM_PACKET_IDS,
} MSG_PACKETID_ENUM;

/**
 * This struct is the message definition for the messages
 * used in the flight software
 */
typedef struct
{
    uint8_t packet_type;
    uint8_t packet_id;
    uint16_t length;
} MSG_Header;

