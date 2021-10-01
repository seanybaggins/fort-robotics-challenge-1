#ifdef TEST

#include "unity.h"

#include "pid_stack.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_pid_stack_init(void)
{
    PidStack pidStack;
    Pid_Stack_init(&pidStack);

    TEST_ASSERT_EQUAL_INT32(pidStack.top, 0);
}

#endif // TEST
