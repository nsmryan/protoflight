/**
 * @file os_test.c
 *
 * @author Noah Ryan
 *
 * This file contains the unit tests for the OS module of the Protoflight software.
 * The test are Unity (ThrowTheSwitch) test fixtures.
 */
#include "stdbool.h"
#include "string.h"

#include "errno.h"

#include "unity.h"
#include "unity_fixture.h"

#include "os_definitions.h"
#include "os_queue.h"
#include "os_timer.h"
#include "os_time.h"
#include "os_task.h"
#include "os_mutex.h"
#include "os_sem.h"


const uint32_t OS_QUEUE_TEST_MSG_SIZE = 8;
const uint32_t OS_QUEUE_TEST_NUM_MSGS = 3;

OS_Queue gvOS_test_queue;
OS_Timer gvOS_test_timer;
OS_Mutex gvOS_test_mutex;
OS_Sem gvOS_test_sem;

bool gvOS_timerFlag = false;
bool gvOS_taskFlag = false;


/* Test Queues */
TEST_GROUP(OS_QUEUE);

TEST_SETUP(OS_QUEUE)
{
  os_queue_create(&gvOS_test_queue, OS_QUEUE_TEST_NUM_MSGS, OS_QUEUE_TEST_MSG_SIZE);
}

TEST_TEAR_DOWN(OS_QUEUE)
{
  memset(&gvOS_test_queue, 0, sizeof(OS_Queue));
}

TEST(OS_QUEUE, queue_create_null)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;

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

  result = os_queue_send(&gvOS_test_queue, NULL, msg_size, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_NULL_POINTER, result);
}

TEST(OS_QUEUE, queue_send_okay)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;

  uint8_t buffer[8];

  result = os_queue_send(&gvOS_test_queue, buffer, OS_QUEUE_TEST_MSG_SIZE, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);

  result = os_queue_send(&gvOS_test_queue, buffer, OS_QUEUE_TEST_MSG_SIZE, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);
}

TEST(OS_QUEUE, queue_send_full)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;

  uint8_t buffer[8];

  result = os_queue_send(&gvOS_test_queue, buffer, OS_QUEUE_TEST_MSG_SIZE, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);

  result = os_queue_send(&gvOS_test_queue, buffer, OS_QUEUE_TEST_MSG_SIZE, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);

  result = os_queue_send(&gvOS_test_queue, buffer, OS_QUEUE_TEST_MSG_SIZE, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);

  result = os_queue_send(&gvOS_test_queue, buffer, OS_QUEUE_TEST_MSG_SIZE, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_TIMEOUT, result);
}

TEST(OS_QUEUE, queue_receive_null_queue)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;

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
  result = os_queue_receive(&gvOS_test_queue, NULL, &size, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_NULL_POINTER, result);
}

TEST(OS_QUEUE, queue_receive_okay)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;

  uint32_t size = OS_QUEUE_TEST_MSG_SIZE;

  uint8_t buffer[8];

  /* test a valid message can be received */
  result = os_queue_send(&gvOS_test_queue, buffer, OS_QUEUE_TEST_MSG_SIZE, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);

  size = OS_QUEUE_TEST_MSG_SIZE;
  result = os_queue_receive(&gvOS_test_queue, buffer, &size, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);
  TEST_ASSERT_EQUAL(OS_QUEUE_TEST_MSG_SIZE, size);
}

TEST(OS_QUEUE, queue_receive_small)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;

  uint32_t size = OS_QUEUE_TEST_MSG_SIZE;

  uint8_t buffer[8];

  /* test a valid message with a smaller size */
  result = os_queue_send(&gvOS_test_queue, buffer, size - 1, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);

  size = OS_QUEUE_TEST_MSG_SIZE;
  result = os_queue_receive(&gvOS_test_queue, buffer, &size, 1);
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
  result = os_queue_receive(&gvOS_test_queue, buffer, &size, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_TIMEOUT, result);
  TEST_ASSERT_EQUAL(OS_QUEUE_TEST_MSG_SIZE, size);

  // put in a message, read it out, and then test empty queue again
  result = os_queue_send(&gvOS_test_queue, buffer, size - 1, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);

  result = os_queue_receive(&gvOS_test_queue, buffer, &size, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);

  size = OS_QUEUE_TEST_MSG_SIZE;
  result = os_queue_receive(&gvOS_test_queue, buffer, &size, 1);
  TEST_ASSERT_EQUAL(OS_RESULT_TIMEOUT, result);
  TEST_ASSERT_EQUAL(OS_QUEUE_TEST_MSG_SIZE, size);
}


/* Test Timers */
TEST_GROUP(OS_TIMER);

TEST_SETUP(OS_TIMER)
{
  os_timer_create(&gvOS_test_timer);

  gvOS_timerFlag = false;
}

TEST_TEAR_DOWN(OS_TIMER)
{
  os_timer_stop(&gvOS_test_timer);
  memset(&gvOS_test_timer, 0, sizeof(OS_Timer));
}

TEST(OS_TIMER, timer_create)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;

  result = os_timer_create(&gvOS_test_timer);
  TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);
  os_timer_stop(&gvOS_test_timer);

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

  result =
      os_timer_start(NULL,
                     (OS_TIMER_FUNC)os_timer_test_single,
                     (void*)&gvOS_timerFlag,
                     timeout);

  TEST_ASSERT_EQUAL(OS_RESULT_NULL_POINTER, result);
}

TEST(OS_TIMER, timer_start_single)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;

  OS_Timeout timeout = 10;

  // test a single timer call
  result =
      os_timer_start(&gvOS_test_timer,
                     (OS_TIMER_FUNC)os_timer_test_single,
                     (void*)&gvOS_timerFlag,
                     timeout);
  TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);

  TEST_ASSERT_EQUAL(false, gvOS_timerFlag);

  os_task_delay(timeout + 2);
  TEST_ASSERT_EQUAL(true, gvOS_timerFlag);

  // ensure remains false- does not retrigger
  os_task_delay(timeout);
  TEST_ASSERT_EQUAL(true, gvOS_timerFlag);

  os_task_delay(timeout);
  TEST_ASSERT_EQUAL(true, gvOS_timerFlag);

  os_timer_stop(&gvOS_test_timer);
}

TEST(OS_TIMER, timer_start_reset)
{
  OS_RESULT_ENUM result = OS_RESULT_OKAY;

  OS_Timeout timeout = 10;

  // test a single timer call
  result =
      os_timer_start(&gvOS_test_timer,
                     (OS_TIMER_FUNC)os_timer_test_reset,
                     (void*)&gvOS_timerFlag,
                     timeout);
  TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);

  TEST_ASSERT_EQUAL(false, gvOS_timerFlag);

  os_task_delay(timeout + 2);
  TEST_ASSERT_EQUAL(true, gvOS_timerFlag);

  os_task_delay(timeout + 1);
  TEST_ASSERT_EQUAL(false, gvOS_timerFlag);

  os_task_delay(timeout + 2);
  TEST_ASSERT_EQUAL(true, gvOS_timerFlag);

  result = os_timer_stop(&gvOS_test_timer);

  // does not retrigger
  os_task_delay(timeout);
  TEST_ASSERT_EQUAL(true, gvOS_timerFlag);

  os_task_delay(timeout);
  TEST_ASSERT_EQUAL(true, gvOS_timerFlag);

  os_timer_stop(&gvOS_test_timer);
}

TEST_GROUP(OS_TIME);

TEST_SETUP(OS_TIME)
{
}

TEST_TEAR_DOWN(OS_TIME)
{
}

TEST(OS_TIME, time_not_zero)
{
    double time_double = os_timestamp_double();

    TEST_ASSERT_TRUE(time_double != 0.0);

    OS_TimeStamp timestamp = os_timestamp();

    TEST_ASSERT_TRUE((timestamp.seconds != 0) || (timestamp.nanoseconds != 0));
}

TEST(OS_TIME, time_delay)
{
    double before = os_timestamp_double();

    os_task_delay(1);

    double after = os_timestamp_double();

    TEST_ASSERT_TRUE(after > before);

    // 2.5 comes from the fact that a delay can be up to 2 clock ticks,
    // plus some extra given that the timeout may not wakeup exactly
    // on time.
    double error = 2.5 * (1.0 / ((double)OS_CONFIG_CLOCK_RATE));

    bool cond = (after - before) < error;

    if (!cond)
    {
        printf("Time diff = %.9f\n", after - before);
    }

    TEST_ASSERT_TRUE(cond);
}

void os_test_task(void *argument)
{
    bool *taskFlag = (bool*)argument;

    *taskFlag = !(*taskFlag);
}

TEST_GROUP(OS_TASK);

TEST_SETUP(OS_TASK)
{
    gvOS_taskFlag = false;
}

TEST_TEAR_DOWN(OS_TASK)
{
}

TEST(OS_TASK, task_spawn_invalid)
{
    OS_RESULT_ENUM result = OS_RESULT_OKAY;
    OS_Task task;

    int argument = 0;

    result = os_task_spawn(NULL, os_test_task, &argument, 10, 1024 * 10);
    TEST_ASSERT_EQUAL(OS_RESULT_NULL_POINTER, result);

    result = os_task_spawn(&task, NULL, &argument, 10, 1024 * 10);
    TEST_ASSERT_EQUAL(OS_RESULT_NULL_POINTER, result);

    result = os_task_spawn(&task, os_test_task, &argument, 10, 0);
    TEST_ASSERT_EQUAL(OS_RESULT_INVALID_ARGUMENTS, result);

    // NOTE assumes that no OS has 1025 priorities
    result = os_task_spawn(&task, os_test_task, &argument, 1025, 0);
    TEST_ASSERT_EQUAL(OS_RESULT_INVALID_ARGUMENTS, result);
}

TEST(OS_TASK, task_spawn)
{
    OS_RESULT_ENUM result = OS_RESULT_OKAY;
    OS_Task task;

    result = os_task_spawn(&task, os_test_task, &gvOS_taskFlag, 20, 1024 * 10);
    TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);

    os_task_delay(1);

    TEST_ASSERT_EQUAL(true, gvOS_taskFlag);
}

TEST_GROUP(OS_MUTEX);

TEST_SETUP(OS_MUTEX)
{
    os_mutex_create(&gvOS_test_mutex);
}

TEST_TEAR_DOWN(OS_MUTEX)
{
}

TEST(OS_MUTEX, mutex_invalid)
{
    OS_RESULT_ENUM result = OS_RESULT_OKAY;

    result = os_mutex_create(NULL);
    TEST_ASSERT_EQUAL(OS_RESULT_NULL_POINTER, result);

    result = os_mutex_take(NULL, 10);
    TEST_ASSERT_EQUAL(OS_RESULT_NULL_POINTER, result);

    result = os_mutex_give(NULL);
    TEST_ASSERT_EQUAL(OS_RESULT_NULL_POINTER, result);
}

TEST(OS_MUTEX, mutex_basics)
{
    OS_RESULT_ENUM result = OS_RESULT_OKAY;

    // take and give a couple of times to make sure
    // function at least act reasonably
    result = os_mutex_take(&gvOS_test_mutex, 0);
    TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);

    result = os_mutex_give(&gvOS_test_mutex);
    TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);

    result = os_mutex_take(&gvOS_test_mutex, 0);
    TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);

    result = os_mutex_give(&gvOS_test_mutex);
    TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);
}

TEST_GROUP(OS_SEM);

TEST_SETUP(OS_SEM)
{
  os_sem_create(&gvOS_test_sem);
}

TEST_TEAR_DOWN(OS_SEM)
{
}

TEST(OS_SEM, sem_invalid)
{
    OS_RESULT_ENUM result = OS_RESULT_OKAY;

    result = os_sem_create(NULL);
    TEST_ASSERT_EQUAL(OS_RESULT_NULL_POINTER, result);

    result = os_sem_take(NULL, 10);
    TEST_ASSERT_EQUAL(OS_RESULT_NULL_POINTER, result);

    result = os_sem_give(NULL);
    TEST_ASSERT_EQUAL(OS_RESULT_NULL_POINTER, result);
}

TEST(OS_SEM, sem_basics)
{
    OS_RESULT_ENUM result = OS_RESULT_OKAY;

    // take and give a couple of times to make sure
    // function at least act reasonably
    result = os_sem_give(&gvOS_test_sem);
    TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);

    result = os_sem_take(&gvOS_test_sem, 0);
    TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);

    result = os_sem_give(&gvOS_test_sem);
    TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);

    result = os_sem_take(&gvOS_test_sem, 0);
    TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);

    result = os_sem_give(&gvOS_test_sem);
    TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);
}

TEST(OS_SEM, sem_timeouts)
{
    OS_RESULT_ENUM result = OS_RESULT_OKAY;

    // take and give a couple of times to make sure
    // function at least act reasonably
    // note that the initial take fails because semaphores start empty
    result = os_sem_take(&gvOS_test_sem, 0);
    TEST_ASSERT_EQUAL(OS_RESULT_TIMEOUT, result);

    result = os_sem_take(&gvOS_test_sem, 0);
    TEST_ASSERT_EQUAL(OS_RESULT_TIMEOUT, result);

    result = os_sem_take(&gvOS_test_sem, OS_TIMEOUT_NO_WAIT);
    TEST_ASSERT_EQUAL(OS_RESULT_TIMEOUT, result);

    result = os_sem_give(&gvOS_test_sem);
    TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);

    result = os_sem_take(&gvOS_test_sem, 1);
    TEST_ASSERT_EQUAL(OS_RESULT_OKAY, result);
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
}

TEST_GROUP_RUNNER(OS_TIME)
{
    RUN_TEST_CASE(OS_TIME, time_delay);
    RUN_TEST_CASE(OS_TIME, time_not_zero);
}

TEST_GROUP_RUNNER(OS_TIMER)
{
  RUN_TEST_CASE(OS_TIMER, timer_create);
  RUN_TEST_CASE(OS_TIMER, timer_start_null);
  RUN_TEST_CASE(OS_TIMER, timer_start_single);
  RUN_TEST_CASE(OS_TIMER, timer_start_reset);
}

TEST_GROUP_RUNNER(OS_TASK)
{
    RUN_TEST_CASE(OS_TASK, task_spawn_invalid);
    RUN_TEST_CASE(OS_TASK, task_spawn);
}

TEST_GROUP_RUNNER(OS_MUTEX)
{
    RUN_TEST_CASE(OS_MUTEX, mutex_invalid);
    RUN_TEST_CASE(OS_MUTEX, mutex_basics);
}

TEST_GROUP_RUNNER(OS_SEM)
{
  RUN_TEST_CASE(OS_SEM, sem_invalid);
  RUN_TEST_CASE(OS_SEM, sem_basics);
  RUN_TEST_CASE(OS_SEM, sem_timeouts);
}
