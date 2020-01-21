/**
 * @file os_test.c
 *
 * @author Noah Ryan
 *
 * This file contains the unit tests for the OS module of the Protoflight software.
 * The test are Unity (ThrowTheSwitch) test fixtures.
 */
#if defined(FSW_UNIT_TEST)

#include "unity.h"
#include "unity_fixture.h"

#include "os_definitions.h"
#include "os_queue.h"


TEST_GROUP(OS);

TEST_SETUP(OS)
{
}

TEST_TEAR_DOWN(OS)
{
}

TEST(OS, queue_create)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;

  OS_Queue queue;

  result = os_queue_create(NULL, 10, 10);
  TEST_ASSERT_EQUAL(OS_RESULT_NULL_POINTER, result);

  result = os_queue_create(&queue, 0, 10);
  TEST_ASSERT_EQUAL(OS_RESULT_INVALID_ARGUMENTS, result);

  result = os_queue_create(&queue, 10, 0);
  TEST_ASSERT_EQUAL(OS_RESULT_INVALID_ARGUMENTS, result);

  result = os_queue_create(&queue, 10, 10);
  TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);
}

TEST(OS, queue_send)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;

  OS_Queue queue;

  uint32_t msg_size = 8;
  uint32_t num_msgs = 3;

  uint8_t buffer[8];

  result = os_queue_create(&queue, num_msgs, msg_size);
  TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);

  result = os_queue_send(&queue, NULL, msg_size, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_NULL_POINTER, result);

  result = os_queue_send(&queue, buffer, msg_size, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);

  result = os_queue_send(&queue, buffer, msg_size, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);

  result = os_queue_send(&queue, buffer, msg_size, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_TIMEOUT, result);
}

TEST(OS, queue_receive)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;

  OS_Queue queue;

  uint32_t msg_size = 8;
  uint32_t num_msgs = 3;
  uint32_t size = msg_size;

  uint8_t buffer[8];

  result = os_queue_create(&queue, num_msgs, msg_size);
  TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);

  // Test a NULL buffer
  result = os_queue_receive(&queue, NULL, &size, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_NULL_POINTER, result);

  // Test a NULL msg_size_bytes
  result = os_queue_receive(&queue, buffer, NULL, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_NULL_POINTER, result);

  /* test a valid message can be received */
  result = os_queue_send(&queue, buffer, msg_size, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);

  size = msg_size;
  result = os_queue_receive(&queue, buffer, &size, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);
  TEST_ASSERT_EQUAL(msg_size, size);

  /* test a valid message with a smaller size */
  result = os_queue_send(&queue, buffer, msg_size - 1, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);

  size = msg_size;
  result = os_queue_receive(&queue, buffer, &size, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);
  TEST_ASSERT_EQUAL(msg_size - 1, size);

  /* receive from empty queue */
  size = msg_size;
  result = os_queue_receive(&queue, buffer, &size, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_TIMEOUT, result);
  TEST_ASSERT_EQUAL(msg_size, size);
}


TEST_GROUP_RUNNER(OS)
{
  RUN_TEST_CASE(OS, queue_create);
  RUN_TEST_CASE(OS, queue_send);
  RUN_TEST_CASE(OS, queue_receive);
}

#endif
