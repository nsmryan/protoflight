/**
 * @file em_test.c
 *
 * @brief Message Module Unit Tests
 *
 * @author Noah Ryan
 *
 * This file contains the unit tests for the Message module.
 */
#include "stddef.h"
#include "stdlib.h"
#include "string.h"

#include "unity.h"
#include "unity_fixture.h"

#include "fsw_definitions.h"
#include "mb_definitions.h"
#include "mb.h"

#include "em_definitions.h"
#include "em.h"


#define FSW_EM_TEST_EVENT_ID 1
#define FSW_EM_TEST_LINE_ID 2


// We need access to the EM state to assert on its fields. 
extern EM_State gvEM_state;

TEST_GROUP(FSW_EM);

TEST_SETUP(FSW_EM)
{
    // this call never fails.
    (void)em_initialize();
}

TEST_TEAR_DOWN(FSW_EM)
{
}

TEST(FSW_EM, invalid_module)
{
    em_event(FSW_MODULEID_INVALID,
             FSW_EM_TEST_EVENT_ID,
             FSW_EM_TEST_LINE_ID,
             1, 2, 3, 4, 5);
    TEST_ASSERT_EQUAL(0, gvEM_state.status.messages_received);
    TEST_ASSERT_EQUAL(1, gvEM_state.status.invalid_msg_received);

    em_event(FSW_MODULEID_NUM_IDS,
             FSW_EM_TEST_EVENT_ID,
             FSW_EM_TEST_LINE_ID,
             1, 2, 3, 4, 5);
    TEST_ASSERT_EQUAL(0, gvEM_state.status.messages_received);
    TEST_ASSERT_EQUAL(2, gvEM_state.status.invalid_msg_received);
}

TEST(FSW_EM, msg_error)
{
    em_event(FSW_MODULEID_EM,
             FSW_EM_TEST_EVENT_ID,
             FSW_EM_TEST_LINE_ID,
             1, 2, 3, 4, 5);
    TEST_ASSERT_EQUAL(0, gvEM_state.status.messages_received);
    TEST_ASSERT_EQUAL(1, gvEM_state.status.message_errors);
}

TEST(FSW_EM, msg_valid)
{
    MB_RESULT_ENUM mb_result;
    MB_Pipe pipe;
    mb_result = mb_create_pipe(&pipe, 1, sizeof(EM_Event));
    TEST_ASSERT_EQUAL(MB_RESULT_OKAY, mb_result);

    mb_result = mb_register_packet(pipe, MSG_PACKETID_EVENT);
    TEST_ASSERT_EQUAL(MB_RESULT_OKAY, mb_result);

    em_event(FSW_MODULEID_EM,
             FSW_EM_TEST_EVENT_ID,
             FSW_EM_TEST_LINE_ID,
             1, 2, 3, 4, 5);
    TEST_ASSERT_EQUAL(0, gvEM_state.status.message_errors);
    TEST_ASSERT_EQUAL(1, gvEM_state.status.messages_received);
}

TEST_GROUP_RUNNER(FSW_EM)
{
    RUN_TEST_CASE(FSW_EM, invalid_module);
    RUN_TEST_CASE(FSW_EM, msg_error);
    RUN_TEST_CASE(FSW_EM, msg_valid);
}

