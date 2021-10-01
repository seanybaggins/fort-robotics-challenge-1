#define STACK_SIZE (1024 * 1024)   
#define _GNU_SOURCE /* Needed for clone to be defined */

#include "init.h"
#include <sys/mman.h> /* For nman memory allocation */
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h> /* For getpid */
#include <sys/wait.h> /* For SIGCHLD */
#include <sched.h> /* For clone */


// Returns 0 if successful. Otherwise returns -1.
int _allocate_mem_init_process(Init_ProcessInfo* initProcessInfo) {
    initProcessInfo->stackBottom = mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);
    if (initProcessInfo->stackBottom == MAP_FAILED)
        return -1;
    /* Assume stack grows downward */
    initProcessInfo->stackTop = initProcessInfo->stackBottom + STACK_SIZE; 
    return 0;
}

static int _init_process_handler(void* arg) {
    printf("Init Process here. My pid is %d.\n", getpid());
    return 0;
}

pid_t Init_processCreate(Init_ProcessInfo* initProcessInfo) {
    if (_allocate_mem_init_process(initProcessInfo) == -1) {
        printf("memory allocation failed\n");
        return -1;
    }
    return clone(_init_process_handler, initProcessInfo->stackTop, CLONE_NEWPID
        | SIGCHLD, NULL);
}

