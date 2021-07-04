/**
 * @file mb_test.c
 *
 * @brief Message Bus Module Unit Tests
 *
 * @author Noah Ryan
 *
 * This file contains the unit tests for the Message Bus module.
 */
#include "stddef.h"
#include "stdlib.h"
#include "string.h"

#include "unity.h"
#include "unity_fixture.h"

#include "fsw_definitions.h"
#include "msg_definitions.h"
#include "em_interface.h"
#include "msg_definitions.h"
#include "msg_interface.h"

#include "mb_definitions.h"
#include "mb_interface.h"


// These unit tests send only headers over queues, so the message size is the
// size of the MSG_Header.
#define FSW_MB_TEST_MSG_SIZE sizeof(MSG_Header)

// Allocate a few messages per queue so we can fill up the queues easily.
#define FSW_MB_TEST_NUM_MSGS 5


/** 
 * We need access to the MB state to assert on its contents
 */
extern MB_State gvMB_state;


TEST_GROUP(FSW_MB);

TEST_SETUP(FSW_MB)
{
    // The test setup re-initializes MB for each test case.
    FSW_RESULT_ENUM result;
    result = mb_initialize();
    TEST_ASSERT_EQUAL(FSW_RESULT_OKAY, result);
}

TEST_TEAR_DOWN(FSW_MB)
{
    // Ideally the queues would be collected here, but instead they
    // are allowed to leak as the queue abstraction has no way to
    // clean them up.
}

/**
 * Test creating a pipe with a NULL pointer.
 */
TEST(FSW_MB, create_pipe_null)
{
    MB_RESULT_ENUM result;
    result = mb_create_pipe(NULL, FSW_MB_TEST_NUM_MSGS, FSW_MB_TEST_MSG_SIZE);
    TEST_ASSERT_EQUAL(MB_RESULT_NULL_POINTER, result);
    TEST_ASSERT_EQUAL(0, gvMB_state.num_pipes);
}

/**
 * Test creating a pipe with nominal inputs.
 */
TEST(FSW_MB, create_pipe)
{
    MB_RESULT_ENUM result;

    MB_Pipe pipe;
    result = mb_create_pipe(&pipe, FSW_MB_TEST_NUM_MSGS, FSW_MB_TEST_MSG_SIZE);
    TEST_ASSERT_EQUAL(MB_RESULT_OKAY, result);
    TEST_ASSERT_EQUAL(1, gvMB_state.num_pipes);
}

/**
 * Test filling up the pipes array, and show that an overflow is detected.
 */
TEST(FSW_MB, create_many_pipes)
{
    MB_RESULT_ENUM result;

    for (uint32_t pipeIndex = 0; pipeIndex < MB_MAX_NUM_PIPES; pipeIndex++)
    {
        MB_Pipe pipe;
        result = mb_create_pipe(&pipe, FSW_MB_TEST_NUM_MSGS, FSW_MB_TEST_MSG_SIZE);
        TEST_ASSERT_EQUAL(MB_RESULT_OKAY, result);
        TEST_ASSERT_EQUAL(pipeIndex + 1, gvMB_state.num_pipes);
    }

    MB_Pipe pipe;
    result = mb_create_pipe(&pipe, FSW_MB_TEST_NUM_MSGS, FSW_MB_TEST_MSG_SIZE);
    TEST_ASSERT_EQUAL(MB_RESULT_MAX_PIPES_REACHED, result);
    TEST_ASSERT_EQUAL(MB_MAX_NUM_PIPES, gvMB_state.num_pipes);
}

/**
 * Test NULL input when registering a packet.
 */
TEST(FSW_MB, register_packet_null)
{
    MB_Pipe pipe = 10000;

    MB_RESULT_ENUM result;
    result = mb_register_packet(pipe, MSG_PACKETID_HEALTHANDSTATUS);
    TEST_ASSERT_EQUAL(MB_RESULT_INVALID_PIPE, result);
}

/**
 * Test registering more than the maximum number of packets.
 */
TEST(FSW_MB, register_over_max)
{
    MB_Pipe pipe = 0;

    for (uint32_t packetIndex = 0; packetIndex < MB_MAX_PIPES_PER_PACKET; packetIndex++)
    {
        MB_RESULT_ENUM result;
        result = mb_register_packet(pipe, MSG_PACKETID_HEALTHANDSTATUS);
        TEST_ASSERT_EQUAL(MB_RESULT_OKAY, result);
    }

    MB_RESULT_ENUM result;
    result = mb_register_packet(pipe, MSG_PACKETID_HEALTHANDSTATUS);
    TEST_ASSERT_EQUAL(MB_RESULT_MAX_PIPES_REACHED, result);
}

/**
 * Test registering a command.
 */
TEST(FSW_MB, register_packet_command)
{
    MB_RESULT_ENUM result;

    MB_Pipe pipe;
    result = mb_create_pipe(&pipe, FSW_MB_TEST_NUM_MSGS, FSW_MB_TEST_MSG_SIZE);
    TEST_ASSERT_EQUAL(MB_RESULT_OKAY, result);
    
    result = mb_register_packet(pipe, MSG_PACKETID_COMMAND);
    TEST_ASSERT_EQUAL(MB_RESULT_OKAY, result);
}

/**
 * Test registering telemetry.
 */
TEST(FSW_MB, register_packet_telemetry)
{
    MB_RESULT_ENUM result;

    MB_Pipe pipe;
    result = mb_create_pipe(&pipe, FSW_MB_TEST_NUM_MSGS, FSW_MB_TEST_MSG_SIZE);
    TEST_ASSERT_EQUAL(MB_RESULT_OKAY, result);
    
    result = mb_register_packet(pipe, MSG_PACKETID_HEALTHANDSTATUS);
    TEST_ASSERT_EQUAL(MB_RESULT_OKAY, result);
}

/**
 * Test sending and receiving a packet.
 */
TEST(FSW_MB, send_receive)
{
    MB_RESULT_ENUM result;

    MB_Pipe pipe;
    result = mb_create_pipe(&pipe, FSW_MB_TEST_NUM_MSGS, FSW_MB_TEST_MSG_SIZE);
    TEST_ASSERT_EQUAL(MB_RESULT_OKAY, result);
    
    result = mb_register_packet(pipe, MSG_PACKETID_HEALTHANDSTATUS);
    TEST_ASSERT_EQUAL(MB_RESULT_OKAY, result);

    MSG_Header header;
    MSG_RESULT_ENUM msgResult;
    msgResult = msg_telemetry_message(&header, MSG_PACKETID_HEALTHANDSTATUS, 0);
    TEST_ASSERT_EQUAL(MSG_RESULT_OKAY, msgResult);

    result = mb_send(&header, OS_TIMEOUT_NO_WAIT);
    TEST_ASSERT_EQUAL(MB_RESULT_OKAY, result);

    MSG_Header recvHeader;
    uint32_t msg_size = sizeof(MSG_Header);
    result = mb_receive(pipe, &recvHeader, &msg_size, OS_TIMEOUT_NO_WAIT);
    TEST_ASSERT_EQUAL(MB_RESULT_OKAY, result);
    TEST_ASSERT_EQUAL(msg_size, sizeof(MSG_Header));
    TEST_ASSERT_EQUAL_MEMORY(&header, &recvHeader, sizeof(header));
}

/**
 * Test sending a packet, and receiving the same packet on multiple pipes.
 */
TEST(FSW_MB, send_receive_two_pipes)
{
    MB_RESULT_ENUM result;

    MB_Pipe pipe1;
    result = mb_create_pipe(&pipe1, FSW_MB_TEST_NUM_MSGS, FSW_MB_TEST_MSG_SIZE);
    TEST_ASSERT_EQUAL(MB_RESULT_OKAY, result);

    MB_Pipe pipe2;
    result = mb_create_pipe(&pipe2, FSW_MB_TEST_NUM_MSGS, FSW_MB_TEST_MSG_SIZE);
    TEST_ASSERT_EQUAL(MB_RESULT_OKAY, result);

    TEST_ASSERT_NOT_EQUAL(pipe1, pipe2);
    
    result = mb_register_packet(pipe1, MSG_PACKETID_COMMAND);
    TEST_ASSERT_EQUAL(MB_RESULT_OKAY, result);

    result = mb_register_packet(pipe2, MSG_PACKETID_COMMAND);
    TEST_ASSERT_EQUAL(MB_RESULT_OKAY, result);

    MSG_Header header;
    MSG_RESULT_ENUM msgResult;
    msgResult = msg_command_message(&header, MSG_PACKETID_COMMAND, 0);
    TEST_ASSERT_EQUAL(MSG_RESULT_OKAY, msgResult);

    result = mb_send(&header, OS_TIMEOUT_NO_WAIT);
    TEST_ASSERT_EQUAL(MB_RESULT_OKAY, result);

    MSG_Header recvHeader1;
    uint32_t msg_size = sizeof(MSG_Header);
    result = mb_receive(pipe1, &recvHeader1, &msg_size, OS_TIMEOUT_NO_WAIT);
    TEST_ASSERT_EQUAL(MB_RESULT_OKAY, result);
    TEST_ASSERT_EQUAL(msg_size, sizeof(MSG_Header));
    TEST_ASSERT_EQUAL_MEMORY(&header, &recvHeader1, sizeof(header));

    MSG_Header recvHeader2;
    result = mb_receive(pipe2, &recvHeader2, &msg_size, OS_TIMEOUT_NO_WAIT);
    TEST_ASSERT_EQUAL(MB_RESULT_OKAY, result);
    TEST_ASSERT_EQUAL(msg_size, sizeof(MSG_Header));
    TEST_ASSERT_EQUAL_MEMORY(&header, &recvHeader2, sizeof(header));
}

TEST_GROUP_RUNNER(FSW_MB)
{
    RUN_TEST_CASE(FSW_MB, create_pipe_null);
    RUN_TEST_CASE(FSW_MB, create_pipe);
    RUN_TEST_CASE(FSW_MB, create_many_pipes);
    RUN_TEST_CASE(FSW_MB, register_packet_command);
    RUN_TEST_CASE(FSW_MB, register_packet_telemetry);
    RUN_TEST_CASE(FSW_MB, register_packet_null);
    RUN_TEST_CASE(FSW_MB, register_over_max);
    RUN_TEST_CASE(FSW_MB, send_receive);
    RUN_TEST_CASE(FSW_MB, send_receive_two_pipes);
}

