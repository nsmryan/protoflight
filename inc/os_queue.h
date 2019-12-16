/**
 * @file os_queue.h
 * 
 * @author Noah Ryan
 *
 * This file contains the interface for message queues for the OS abstraction
 * used by the fsw.
 */
#include "os_definitions.h"

typedef OS_Queue int;

OS_RESULT_ENUM os_queue_create(OS_Queue *queue,
		               uint32_t num_msgs,
			       uint32_t msg_size_bytes);
