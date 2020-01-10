/**
 * @file msg_interface.h
 *
 * @author Noah Ryan
 *
 * This file contains interface definitions for the Message module
 */
#ifndef __MSG_INTERFACE_H__
#define __MSG_INTERFACE_H__

#include "msg_definitions.h"


/**
 * @brief msg_telemetry_message
 *
 * @author Noah Ryan
 *
 * This function sets up a telemetry message header given the required information.
 *
 * @param[out] header - a pointer to the message header to fill out.
 * @param[in] packet_id - the packet id of the message.
 * @param[in] size_bytes - the message size in bytes.
 *
 * @return Either MSG_RESULT_OKAY indicating that the message was successfully
 * filled out, or MSG_RESULT_NULL_POINTER indicating that the message input
 * was NULL.
 */
MSG_RESULT_ENUM msg_telemetry_message(MSG_Header *header, MSG_PACKETID_ENUM packet_id, uint16_t size_bytes); 

/**
 * @brief msg_command_message
 *
 * @author Noah Ryan
 *
 * This function sets up a command's message header given the required information.
 *
 * @param[out] header - a pointer to the message header to fill out.
 * @param[in] packet_id - the packet id of the message.
 * @param[in] size_bytes - the message size in bytes.
 *
 * @return Either MSG_RESULT_OKAY indicating that the message was successfully
 * filled out, or MSG_RESULT_NULL_POINTER indicating that the message input
 * was NULL.
 */
MSG_RESULT_ENUM msg_command_message(MSG_Header *header, MSG_PACKETID_ENUM packet_id, uint16_t size_bytes); 

#endif // ndef __MSG_INTERFACE_H__ */
