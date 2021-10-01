#ifdef TEST

#define _GNU_SOURCE /* Needed for clone to be defined */
#include "unity.h"

#include "main2.c"
#include <sys/wait.h> /* For SIGCHLD */
#include <sched.h> /* For clone */

void setUp(void)
{
}

void tearDown(void)
{
}

void test_main2_NeedToImplement(void)
{
    main2();
}

#endif // TEST
