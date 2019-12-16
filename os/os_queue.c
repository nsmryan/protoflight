/**
 * @file os_queue.c
 *
 * @author Noah Ryan
 *
 * This file contains the implementation of the OS abstraction for message
 * queues.
 */
#include "os_definitions.h"
#include "os_queue.h"

OS_RESULT_ENUM os_queue(OS_Queue *queue,
		        uint32_t num_msgs,
			uint32_t msg_size_bytes)
{
	// NOTE need implementation for message queues
}

OS_RESULT_ENUM os_queue_send(uint32_t num_msgs, uint32_t msg_size_bytes)
{
	// NOTE need implementation for message queues
}

OS_RESULT_ENUM os_queue_receive(uint32_t num_msgs, uint32_t msg_size_bytes)
{
	// NOTE need implementation for message queues
}

