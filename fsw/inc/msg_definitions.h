/**
 * @file msg_definitions.h
 *
 * @author Noah Ryan
 *
 * @brief Message Module
 *
 * The message module contains defintiions for messages.
 */
#ifndef __MSG_DEFINITIONS_H__
#define __MSG_DEFINITIONS_H__

#include "stdint.h"


/**
 * This enum provides the result value of Message module functions.
 * It either indicates either a succesful result (MSG_RESULT_OKAY)
 * or an error code indicating the cause of an error.
 */
typedef enum
{
	MSG_RESULT_INVALID             = 0, /*<< Invalid result value */
	MSG_RESULT_OKAY                = 1, /*<< Success */
	MSG_RESULT_NULL_POINTER        = 2, /*<< NULL pointer provided as an argument */
	MSG_RESULT_INVALID_PACKET_ID   = 3, /*<< Invalid packet ID provided */
	MSG_RESULT_NUM_RESULTS
} MSG_RESULT_ENUM;

/**
 * The MSG_PACKETTYPE_ENUM indicates whether a message is a telemetry
 * message or a command message.
 */
typedef enum
{
  MSG_PACKETTYPE_INVALID   = 0, /*<< Invalid packet type */
  MSG_PACKETTYPE_COMMAND   = 1, /*<< Packet type command */
  MSG_PACKETTYPE_TELEMETRY = 2, /*<< Packet type telemetry */
  MSG_PACKETTYPE_NUM_TYPES      /*<< Number of packet types */
} MSG_PACKETTYPE_ENUM;

/**
 * This enum is the packet id of a message, indicating which message
 * structure the packet contains.
 */
typedef enum
{
    MSG_PACKETID_INVALID         = 0, /*<< Invalid packet ID */
    MSG_PACKETID_HEALTHANDSTATUS = 1, /*<< Health and Status packet ID */
    MSG_PACKETID_EVENT           = 2, /*<< Event packet ID */
    MSG_PACKETID_COMMAND         = 3, /*<< Command packet ID */
    MSG_PACKETID_NUM_PACKET_IDS,      /*<< Number of packet IDs */
} MSG_PACKETID_ENUM;

/**
 * This struct is the message definition for the messages
 * used in the flight software
 */
typedef struct
{
    uint8_t packet_type; /*<< Packet Type (see MSG_PACKETTYPE_ENUM) */
    uint8_t packet_id;   /*<< Packet Id (see MSG_PACKETID_ENUM) */
    uint16_t length;     /*<< Length of data after the message header */
} MSG_Header;

#endif // ndef __MSG_DEFINITIONS_H__ */
