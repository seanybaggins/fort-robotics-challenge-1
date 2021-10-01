#define _GNU_SOURCE
#include <sys/wait.h>
#include <sys/utsname.h>
#include <sched.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
} while (0)

static int childFunc(void *arg)
{
    printf("Init here. My pid is %d\n", getpid());
    return 0;           /* Child terminates now */
}

#define STACK_SIZE (1024 * 1024)    /* Stack size for cloned child */

int main2()
{
    char *stack;                    /* Start of stack buffer */
    char *stackTop;                 /* End of stack buffer */
    pid_t pid;
    printf("Entry here. Pid is %d.\n", getpid());

    /* Allocate memory to be used for the stack of the child. */
    stack = mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE,
            MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);
    if (stack == MAP_FAILED)
        errExit("mmap");
    stackTop = stack + STACK_SIZE;  /* Assume stack grows downward */

    pid = clone(childFunc, stackTop, CLONE_NEWPID | SIGCHLD, NULL);

    if (pid == -1)
        errExit("clone");
    printf("clone() returned %jd\n", (intmax_t) pid);

    sleep(1);
    /* Parent falls through to here */

    exit(EXIT_SUCCESS);
}

