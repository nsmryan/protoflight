/**
 * @file os_queue.h
 * 
 * @author Noah Ryan
 *
 * This file contains the interface for message queues for the OS abstraction
 * used by the fsw.
 */
#ifndef __OS_QUEUE_H__
#define __OS_QUEUE_H__

#include "stdint.h"

#include "os_definitions.h"


/**
 * This definition gives the maximum number of queues that can
 * be allocated.
 */
// NOTE consider moving this to a os_config.h header.
#define OS_QUEUE_MAX_QUEUES 100


/**
 * @brief os_queue_create
 *
 * This function creates a new queue with a given message size
 * and a given maximum number of messages.
 *
 * @param[in,out] queue - a non-NULL pointer to a OS_Queue.
 *                 This will be filled out if the return of this function is not OS_RESULT_OKAY.
 * @param[in] num_msgs - the number of messages that this queue supports.
 * @param[in] msg_size_bytes - the maximum size of a message place on this queue.
 *
 * @return An OS result type either indicating success (OS_RESULT_OKAY)
 * or an error code indicating the cause of the error.
 */
OS_RESULT_ENUM os_queue_create(OS_Queue *queue,
                               uint32_t num_msgs,
                               uint32_t msg_size_bytes);

/**
 * @brief os_queue_send
 *
 * This function attempts to send a message on the given queue.
 *
 * @param[in] queue - a non-NULL pointer to a OS_Queue.
 * @param[in] buffer - a pointer to a buffer to place on the queue.
 * @param[in] buffer_size_bytes - the size of the buffer to copy.
 * @param[in] timeout - the timeout to wait in case the queue is full.
 *
 * @return An OS result type either indicating success (OS_RESULT_OKAY)
 * or an error code indicating the cause of the error.
 */
OS_RESULT_ENUM os_queue_send(OS_Queue *queue,
                             uint8_t *buffer,
                             uint32_t buffer_size_bytes,
                             OS_Timeout timeout);

/**
 * @brief os_queue_receive
 *
 * This function attempts to receive a message from a given queue.
 *
 * @param[in] queue - a non-NULL pointer to a OS_Queue.
 * @param[in] buffer - a pointer to a buffer to place the message in.
 * @param[in,out] buffer_size_bytes - a pointer to the size of the buffer 
 *                 to copy into. The pointer will be filled with the
 *                 actual message size if this function returns OS_RESULT_OKAY.
 * @param[in] timeout - the timeout to wait in case the queue is empty.
 *
 * @return An OS result type either indicating success (OS_RESULT_OKAY)
 * or an error code indicating the cause of the error.
 */
OS_RESULT_ENUM os_queue_receive(OS_Queue *queue,
                                uint8_t *buffer,
                                uint32_t *buffer_size_bytes,
                                OS_Timeout timeout);

#endif // ndef __OS_QUEUE_H__ */
