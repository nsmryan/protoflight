/**
 * @file os_test.c
 *
 * @author Noah Ryan
 *
 * This file contains the unit tests for the OS module of the Protoflight software.
 * The test are Unity (ThrowTheSwitch) test fixtures.
 */
#include "unity.h"
#include "unity_fixture.h"


TEST_GROUP(OS);

TEST_SETUP(OS)
{
}

TEST_TEAR_DOWN(OS)
{
}

TEST(OS, mutex_create)
{
  TEST_ASSERT_EQUAL(0, 0);
}
