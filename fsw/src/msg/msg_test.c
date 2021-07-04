/**
 * @file msg_test.c
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
#include "msg_definitions.h"
#include "msg_interface.h"



TEST_GROUP(FSW_MSG);

TEST_SETUP(FSW_MSG)
{
}

TEST_TEAR_DOWN(FSW_MSG)
{
}

TEST(FSW_MSG, create_telemetry_null)
{
    MSG_RESULT_ENUM result;
    result = msg_telemetry_message(NULL,
                                   MSG_PACKETID_HEALTHANDSTATUS,
                                   0);
    TEST_ASSERT_EQUAL(MSG_RESULT_NULL_POINTER, result);
}

TEST(FSW_MSG, create_command_null)
{
    MSG_RESULT_ENUM result;
    result = msg_command_message(NULL,
                                 MSG_PACKETID_COMMAND,
                                 0);
    TEST_ASSERT_EQUAL(MSG_RESULT_NULL_POINTER, result);
}

TEST(FSW_MSG, create_telemetry_invalid_id)
{
    MSG_RESULT_ENUM result;
    MSG_Header header;

    result = msg_telemetry_message(&header,
                                   MSG_PACKETID_INVALID,
                                   0);
    TEST_ASSERT_EQUAL(MSG_RESULT_INVALID_PACKET_ID, result);

    result = msg_telemetry_message(&header,
                                   MSG_PACKETID_NUM_PACKET_IDS,
                                   0);
    TEST_ASSERT_EQUAL(MSG_RESULT_INVALID_PACKET_ID, result);
}

TEST(FSW_MSG, create_command_invalid_id)
{
    MSG_RESULT_ENUM result;
    MSG_Header header;

    result = msg_command_message(&header,
                                 MSG_PACKETID_INVALID,
                                 0);
    TEST_ASSERT_EQUAL(MSG_RESULT_INVALID_PACKET_ID, result);

    result = msg_command_message(&header,
                                 MSG_PACKETID_NUM_PACKET_IDS,
                                 0);
    TEST_ASSERT_EQUAL(MSG_RESULT_INVALID_PACKET_ID, result);
}

TEST(FSW_MSG, create_telemetry)
{
    MSG_RESULT_ENUM result;

    MSG_Header header;
    result = msg_telemetry_message(&header, 
                                   MSG_PACKETID_HEALTHANDSTATUS,
                                   sizeof(header));
    TEST_ASSERT_EQUAL(MSG_RESULT_OKAY, result);
}

TEST(FSW_MSG, create_command)
{
    MSG_RESULT_ENUM result;

    MSG_Header header;
    result = msg_command_message(&header,
                                 MSG_PACKETID_COMMAND,
                                 sizeof(header));
    TEST_ASSERT_EQUAL(MSG_RESULT_OKAY, result);
}

TEST_GROUP_RUNNER(FSW_MSG)
{
    RUN_TEST_CASE(FSW_MSG, create_telemetry_null);
    RUN_TEST_CASE(FSW_MSG, create_command_null);
    RUN_TEST_CASE(FSW_MSG, create_telemetry_invalid_id);
    RUN_TEST_CASE(FSW_MSG, create_command_invalid_id);
    RUN_TEST_CASE(FSW_MSG, create_telemetry);
    RUN_TEST_CASE(FSW_MSG, create_command);
}

