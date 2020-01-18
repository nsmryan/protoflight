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

TEST(OS, mutex_create)
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

TEST_GROUP_RUNNER(OS)
{
  RUN_TEST_CASE(OS, mutex_create);
}

#endif
