/**
 * @file test.c
 *
 * @author Noah Ryan
 *
 * This file contains the unit test main function
 * for protoflight. Building protoflight for unit tests
 * will execute each module's unit tests as Unity fixtures.
 */
#if defined(FSW_UNIT_TEST)

#include "unity_fixture.h"


static void Test_RunAllTests(void)
{
  RUN_TEST_GROUP(OS)
}

int main(int argc, char const *argv[])
{
  UnityMain(argc, argv, Test_RunAllTests);
}

#endif
