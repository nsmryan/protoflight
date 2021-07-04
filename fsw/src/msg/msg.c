/**
 * @file msg.c
 *
 * @author Noah Ryan
 *
 * This file contains the implementation of Message module functions.
 */
#include "stddef.h"
#include "stdint.h"

#include "msg_definitions.h"
#include "msg_interface.h"


MSG_RESULT_ENUM msg_telemetry_message(MSG_Header *header,
                                      MSG_PACKETID_ENUM packet_id,
                                      uint16_t size_bytes)
{
    MSG_RESULT_ENUM result = MSG_RESULT_OKAY;
  
    if (header == NULL)
    {
        result = MSG_RESULT_NULL_POINTER;
    }
    else
    {
        header->packet_type = MSG_PACKETTYPE_TELEMETRY;
        header->packet_id = packet_id;
        header->length = size_bytes;
    }
  
    return result;
}

MSG_RESULT_ENUM msg_command_message(MSG_Header *header,
                                    MSG_PACKETID_ENUM packet_id,
                                    uint16_t size_bytes)
{
    MSG_RESULT_ENUM result = MSG_RESULT_OKAY;

    if (header == NULL)
    {
        result = MSG_RESULT_NULL_POINTER;
    }
    else
    {
        header->packet_type = MSG_PACKETTYPE_COMMAND;
        header->packet_id = packet_id;
        header->length = size_bytes;
    }

    return result;
}

