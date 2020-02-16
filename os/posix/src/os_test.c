/**
 * @file os_test.c
 *
 * @author Noah Ryan
 *
 * This file contains the unit tests for the OS module of the Protoflight software.
 * The test are Unity (ThrowTheSwitch) test fixtures.
 */
#if defined(FSW_UNIT_TEST)

#include "string.h"

#include "errno.h"

#include "unity.h"
#include "unity_fixture.h"

#include "os_definitions.h"
#include "os_queue.h"


const uint32_t OS_QUEUE_TEST_MSG_SIZE = 8;
const uint32_t OS_QUEUE_TEST_NUM_MSGS = 3;

OS_Queue gvOS_testQueue;
OS_Timer gvOS_testTimer;

bool gvOS_timerFlag = false;


/* Test Queues */
TEST_GROUP(OS_QUEUE);

TEST_SETUP(OS_QUEUE)
{
  os_queue_create(&gvOS_testQueue, OS_QUEUE_TEST_NUM_MSGS, OS_QUEUE_TEST_MSG_SIZE);
}

TEST_TEAR_DOWN(OS_QUEUE)
{
  memset(&gvOS_testQueue, 0, sizeof(OS_Queue));
}

TEST(OS_QUEUE, queue_create_null)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;
  OS_Queue queue;

  result = os_queue_create(NULL, 10, 10);
  TEST_ASSERT_EQUAL(OS_RESULT_NULL_POINTER, result);
}

TEST(OS_QUEUE, queue_create_size_0)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;
  OS_Queue queue;

  result = os_queue_create(&queue, 0, 10);
  TEST_ASSERT_EQUAL(OS_RESULT_INVALID_ARGUMENTS, result);
}

TEST(OS_QUEUE, queue_create_msgs_0)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;
  OS_Queue queue;

  result = os_queue_create(&queue, 10, 0);
  TEST_ASSERT_EQUAL(OS_RESULT_INVALID_ARGUMENTS, result);
}

TEST(OS_QUEUE, queue_create_okay)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;
  OS_Queue queue;

  result = os_queue_create(&queue, 10, 10);
  TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);
}

TEST(OS_QUEUE, queue_send_null_queue)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;

  uint8_t buffer[8];

  result = os_queue_send(NULL, buffer, OS_QUEUE_TEST_MSG_SIZE, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_NULL_POINTER, result);
}

TEST(OS_QUEUE, queue_send_null_buffer)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;

  uint32_t msg_size = 8;

  result = os_queue_send(&gvOS_testQueue, NULL, msg_size, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_NULL_POINTER, result);
}

TEST(OS_QUEUE, queue_send_okay)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;

  uint8_t buffer[8];

  result = os_queue_send(&gvOS_testQueue, buffer, OS_QUEUE_TEST_MSG_SIZE, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);

  result = os_queue_send(&gvOS_testQueue, buffer, OS_QUEUE_TEST_MSG_SIZE, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);
}

TEST(OS_QUEUE, queue_send_full)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;

  uint8_t buffer[8];

  result = os_queue_send(&gvOS_testQueue, buffer, OS_QUEUE_TEST_MSG_SIZE, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);

  result = os_queue_send(&gvOS_testQueue, buffer, OS_QUEUE_TEST_MSG_SIZE, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);

  result = os_queue_send(&gvOS_testQueue, buffer, OS_QUEUE_TEST_MSG_SIZE, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);

  result = os_queue_send(&gvOS_testQueue, buffer, OS_QUEUE_TEST_MSG_SIZE, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_TIMEOUT, result);
}

TEST(OS_QUEUE, queue_receive_null_queue)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;

  OS_Queue queue;

  uint32_t size = OS_QUEUE_TEST_MSG_SIZE;

  uint8_t buffer[8];

  // Test a NULL queue
  result = os_queue_receive(NULL, buffer, &size, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_NULL_POINTER, result);
}

TEST(OS_QUEUE, queue_receive_null_buffer)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;

  uint32_t size = OS_QUEUE_TEST_MSG_SIZE;

  // Test a NULL buffer
  result = os_queue_receive(&gvOS_testQueue, NULL, &size, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_NULL_POINTER, result);
}

TEST(OS_QUEUE, queue_receive_okay)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;

  uint32_t size = OS_QUEUE_TEST_MSG_SIZE;

  uint8_t buffer[8];

  /* test a valid message can be received */
  result = os_queue_send(&gvOS_testQueue, buffer, OS_QUEUE_TEST_MSG_SIZE, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);

  size = OS_QUEUE_TEST_MSG_SIZE;
  result = os_queue_receive(&gvOS_testQueue, buffer, &size, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);
  TEST_ASSERT_EQUAL(OS_QUEUE_TEST_MSG_SIZE, size);
}

TEST(OS_QUEUE, queue_receive_small)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;

  uint32_t size = OS_QUEUE_TEST_MSG_SIZE;

  uint8_t buffer[8];

  /* test a valid message with a smaller size */
  result = os_queue_send(&gvOS_testQueue, buffer, size - 1, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);

  size = OS_QUEUE_TEST_MSG_SIZE;
  result = os_queue_receive(&gvOS_testQueue, buffer, &size, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);
  TEST_ASSERT_EQUAL(OS_QUEUE_TEST_MSG_SIZE - 1, size);
}

TEST(OS_QUEUE, queue_receive_empty)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;

  uint32_t size = OS_QUEUE_TEST_MSG_SIZE;

  uint8_t buffer[8];

  /* receive from empty queue */
  size = OS_QUEUE_TEST_MSG_SIZE;
  result = os_queue_receive(&gvOS_testQueue, buffer, &size, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_TIMEOUT, result);
  TEST_ASSERT_EQUAL(OS_QUEUE_TEST_MSG_SIZE, size);

  // put in a message, read it out, and then test empty queue again
  result = os_queue_send(&gvOS_testQueue, buffer, size - 1, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);

  result = os_queue_receive(&gvOS_testQueue, buffer, &size, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);

  size = OS_QUEUE_TEST_MSG_SIZE;
  result = os_queue_receive(&gvOS_testQueue, buffer, &size, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_TIMEOUT, result);
  TEST_ASSERT_EQUAL(OS_QUEUE_TEST_MSG_SIZE, size);
}


/* Test Timers */
TEST_GROUP(OS_TIMER);

TEST_SETUP(OS_TIMER)
{
  os_timer_create(&gvOS_testTimer);
}

TEST_TEAR_DOWN(OS_TIMER)
{
  memset(&gvOS_testTimer, 0, sizeof(OS_Timer));
}

TEST(OS_TIMER, timer_create)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;

  result = os_timer_create(&gvOS_testTest);
  TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);

  result = os_timer_create(NULL);
  TEST_ASSERT_EQUAL(OS_RESULT_NULL_POINTER, result);
}


bool os_timer_test_single(void *argument)
{
    bool *flag = (bool*)argument;
    *flag = !(*flag);

    return false;
}

bool os_timer_test_reset(void *argument)
{
    bool *flag = (bool*)argument;
    *flag = !(*flag);

    return true;
}

TEST(OS_TIMER, timer_start_null)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;

  OS_Timeout timeout = 10;

  OS_RESULT_ENUM result =
      os_timer_start(NULL,
                     os_timer_test_function,
                     timeout,
  TEST_ASSERT_EQUAL(OS_RESULT_NULL_POINTER, result);
}

TEST(OS_TIMER, timer_start_single)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;

  OS_Timeout timeout = 10;

  // test a single timer call
  OS_RESULT_ENUM result =
      os_timer_start(&gvOS_testTimer,
                     os_timer_test_function,
                     &gvOS_timerFlag,
                     timeout);
  TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);

  TEST_ASSERT_EQUAL(false, gvOS_timerFlag);

  os_task_delay(timeout);
  TEST_ASSERT_EQUAL(true, gvOS_timerFlag);

  // ensure remains false- does not retrigger
  os_task_delay(timeout);
  TEST_ASSERT_EQUAL(true, gvOS_timerFlag);

  os_task_delay(timeout);
  TEST_ASSERT_EQUAL(true, gvOS_timerFlag);

}

TEST(OS_TIMER, timer_start_reset)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;

  OS_Timeout timeout = 10;

  // test a single timer call
  OS_RESULT_ENUM result =
      os_timer_start(&gvOS_testTimer,
                     os_timer_test_function,
                     &gvOS_timerFlag,
                     timeout);
  TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);

  TEST_ASSERT_EQUAL(false, gvOS_timerFlag);

  os_task_delay(timeout);
  TEST_ASSERT_EQUAL(true, gvOS_timerFlag);

  os_task_delay(timeout);
  TEST_ASSERT_EQUAL(false, gvOS_timerFlag);

  os_task_delay(timeout);
  TEST_ASSERT_EQUAL(true, gvOS_timerFlag);

  OS_RESULT_ENUM result =
      os_timer_stop(&gvOS_testTimer);

  // does not retrigger
  os_task_delay(timeout);
  TEST_ASSERT_EQUAL(true, gvOS_timerFlag);

  os_task_delay(timeout);
  TEST_ASSERT_EQUAL(true, gvOS_timerFlag);
}

TEST_GROUP_RUNNER(OS_QUEUE)
{
  RUN_TEST_CASE(OS_QUEUE, queue_create_null);
  RUN_TEST_CASE(OS_QUEUE, queue_create_size_0);
  RUN_TEST_CASE(OS_QUEUE, queue_create_msgs_0);
  RUN_TEST_CASE(OS_QUEUE, queue_create_okay);
  RUN_TEST_CASE(OS_QUEUE, queue_send_null_queue);
  RUN_TEST_CASE(OS_QUEUE, queue_send_null_buffer);
  RUN_TEST_CASE(OS_QUEUE, queue_send_full);
  RUN_TEST_CASE(OS_QUEUE, queue_send_okay);
  RUN_TEST_CASE(OS_QUEUE, queue_receive_null_queue);
  RUN_TEST_CASE(OS_QUEUE, queue_receive_null_buffer);
  RUN_TEST_CASE(OS_QUEUE, queue_receive_small);
  RUN_TEST_CASE(OS_QUEUE, queue_receive_empty);
  RUN_TEST_CASE(OS_QUEUE, queue_receive_okay);

  RUN_TEST_CASE(OS_TIMER, timer_create);
  RUN_TEST_CASE(OS_TIMER, timer_start_null);
  RUN_TEST_CASE(OS_TIMER, timer_start_single);
  RUN_TEST_CASE(OS_TIMER, timer_start_reset);
}

#endif
