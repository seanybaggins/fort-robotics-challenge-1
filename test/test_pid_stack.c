#ifdef TEST

#include "unity.h"

#include "pid_stack.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void assert_expected_buffer_equals_actual(pid_t* expected, pid_t* actual, int size) {
    for (int i = 0; i < size; i++) {
        TEST_ASSERT_EQUAL_INT(expected[i], actual[i]);
    }
}

void test_pid_stack_init(void) {
    PidStack pidStack;
    Pid_Stack_init(&pidStack);

    pid_t expected_buffer[MAX_SIZE] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

    TEST_ASSERT_EQUAL_INT(-1, pidStack.top );
    TEST_ASSERT_TRUE(Pid_Stack_isEmpty(&pidStack));
    assert_expected_buffer_equals_actual(expected_buffer, pidStack.buffer, MAX_SIZE);
}

void test_pid_stack_correct_contents(void) {
    PidStack pidStack;
    Pid_Stack_init(&pidStack);
    pid_t expected_buffer[MAX_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    for (int i = 0; i < MAX_SIZE; i++) {
        Pid_Stack_push(&pidStack, expected_buffer[i]);
    }

    TEST_ASSERT_TRUE(Pid_Stack_isFull(&pidStack));
    assert_expected_buffer_equals_actual(expected_buffer, pidStack.buffer, MAX_SIZE);
}

void test_pid_stack_empty_pop(void) {
    PidStack pidStack;
    Pid_Stack_init(&pidStack);
    pid_t pid = Pid_Stack_pop(&pidStack);

    TEST_ASSERT_EQUAL_INT(-1, pid);
    TEST_ASSERT_EQUAL_INT(-1, pidStack.buffer[0]);
    TEST_ASSERT_EQUAL_INT(-1, pidStack.top);
}

void test_attempt_stack_overflow(void) {
    PidStack pidStack;
    Pid_Stack_init(&pidStack);
    pid_t expected_buffer[MAX_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    // Fill the stack
    for (int i = 0; i < MAX_SIZE; i++) {
        Pid_Stack_push(&pidStack, expected_buffer[i]);
    }
    TEST_ASSERT_TRUE(Pid_Stack_isFull(&pidStack));

    // Attempt to overwrite push out of bounds
    int ret = Pid_Stack_push(&pidStack, 11);

    TEST_ASSERT_EQUAL_INT(-1, ret);
    TEST_ASSERT_EQUAL_INT(9, pidStack.top);
}

#endif // TEST
