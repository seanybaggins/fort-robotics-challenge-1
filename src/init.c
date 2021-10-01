#define STACK_SIZE (1024 * 1024)   
#define _GNU_SOURCE    // Needed for clone to be defined

#include "init.h"
#include <sys/mman.h>  // For nman memory allocation
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>    // For getpid
#include <sys/wait.h>  // For SIGCHLD
#include <sched.h>     // For clone


// Returns 0 if successful. Otherwise returns -1.
static int _allocate_mem_init_process(Init_ProcessInfo* initProcessInfo) {
    initProcessInfo->stackBottom = mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);
    if (initProcessInfo->stackBottom == MAP_FAILED)
        return -1;
    // Assume stack grows downward 
    initProcessInfo->stackTop = initProcessInfo->stackBottom + STACK_SIZE; 
    return 0;
}

// TODO: 
// 1. Set the signal handlers for this process.
// 2. When recieving SIGUSR1 - create a new child thread within the PID namespace.
// 3. When recieving SIGINT - kill the most recently created child in the PID namespace
// 4. When recieving SIGUSR2 - write a listing of the existing children and their PIDs (within the
//            created namespace) to a file 'pidlist.txt'
static void _init_process_sigusr1_handler() {
    printf("Init Process here. My pid is %d. SIGUSR1 recieved.\n", getpid());
}

static void _init_process_sigusr2_handler() {
    printf("Init Process here. My pid is %d. SIGUSR2 recieved.\n", getpid());
}

static void _init_process_sigint_handler() {
    printf("Init Process here. My pid is %d. SIGINT recieved.\n", getpid());
}

static int _init_process_handler(void* arg) {
    signal(SIGUSR1, _init_process_sigusr1_handler);
    signal(SIGUSR2, _init_process_sigusr2_handler);
    signal(SIGINT, _init_process_sigint_handler);
    printf("Init Process here. My pid is %d.\n", getpid());
    while (1) {

    }
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

