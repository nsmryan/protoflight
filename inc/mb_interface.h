/**
 * @file mb_interface.h
 *
 * @author Noah Ryan
 *
 * @brief Message Bus Module Interface
 *
 * The message bus module handles passing packets between tasks or modules.
 */
#ifndef __MB_INTERFACE_H__
#define __MB_INTERFACE_H__

#include "stdint.h"

#include "os_queue.h"

#include "fsw_definitions.h"
#include "msg_definitions.h"

#include "mb_definitions.h"


/**
 * @brief This function initializes the Message Bus module
 *
 * @author Noah Ryan
 *
 * @return A status enum indicating either success (FSW_RESULT_OKAY),
 * or an error code describing the error.
 */
FSW_RESULT_ENUM mb_initialize(void);

/**
 * @brief This function sends a message on the message bus. This will place the
 * message on all listening pipes.
 *
 * @author Noah Ryan
 *
 * @param[in] message - a pointer to the message to send.
 * @param[in] msg_size - the size of the message, in bytes.
 * @param[in] timeout - a timeout value for how long to wait for a message.
 *
 * @return Either success (MB_RESULT_OKAY), or an error code indicating the
 *         source of the error.
 */
MB_RESULT_ENUM mb_send(MSG_Header *message, OS_Timeout timeout);

/**
 * @brief This function receives a message from a particular pipe. This will
 * fill out the message buffer provided with a message, or return an error
 * code.
 *
 * @param[in] pipeId - the pipe to receive from.
 * @param[out] message - the message buffer to fill out with a new. message.
 * @param[in] msg_size - the size of the message buffer, in bytes.
 * @param[in] timeout - the timeout indicating how long to wait for a message (in
 *                      system clock ticks).
 *
 * @return Either success (MB_RESULT_OKAY), or an error code indicating the
 *         source of the error.
 */
MB_RESULT_ENUM mb_receive(MB_Pipe *pipeId,
                          MSG_Header *message,
                          OS_Timeout timeout);

/**
 * @brief This function creates a new message pipe, returning a handle to the pipe
 * to be used later for receiving.
 *
 * @param[out] pipe - a pointer to a message pipe handle, which will be filled out with
 *               a new message pipe handle.
 * @param[in] msg_size_bytes - the maximum numnber of bytes in a message
 * @param[in] num_msgs - the maximum number of messages that can be queued for this pipe
 *
 * @return Either success (MB_RESULT_OKAY), or an error code indicating the
 *         source of the error.
 */
MB_RESULT_ENUM mb_create_pipe(MB_Pipe *pipe,
                              uint16_t msg_size_bytes,
                              uint16_t num_msgs);

/**
 * @brief This function registers a message to receive on a pipe. Messages
 * sent on the message bus will be delivered to any pipe listening for that
 * message type.
 *
 * @param[in] pipe - the pipe to register a new message on
 * @param[in] packet_type - the packet type to listen to for the given pipe
 *
 * @return Either success (MB_RESULT_OKAY), or an error code indicating the
 *         source of the error.
 */
MB_RESULT_ENUM mb_register_packet(MB_Pipe *pipe, MSG_PACKETTYPE_ENUM packet_type);

/**
 * @brief mb_get_status
 *
 * This function provides the Message Bus module's status.
 * No status is returned- if a null pointer is provided, it
 * will do nothing.
 */
void mb_get_status(MB_Status *status);

#endif // ndef __MB_INTERFACE_H__ */
