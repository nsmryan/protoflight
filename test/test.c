/**
 * @file test.c
 *
 * @author Noah Ryan
 *
 * This file contains the unit test main function
 * for protoflight. Building protoflight for unit tests
 * will execute each module's unit tests as Unity fixtures.
 */
#include "unity.h"
#include "unity_fixture.h"


static void Test_RunAllTests(void)
{
    // OS Test Groups
    RUN_TEST_GROUP(OS_QUEUE);
    RUN_TEST_GROUP(OS_TIME);
    RUN_TEST_GROUP(OS_TIMER);
    RUN_TEST_GROUP(OS_TASK);
    RUN_TEST_GROUP(OS_MUTEX);
    RUN_TEST_GROUP(OS_SEM);

    // FSW Test Groups
    RUN_TEST_GROUP(FSW_MB);
    RUN_TEST_GROUP(FSW_MSG);
    RUN_TEST_GROUP(FSW_EM);
}

int main(int argc, char const *argv[])
{
    UnityMain(argc, argv, Test_RunAllTests);
}

