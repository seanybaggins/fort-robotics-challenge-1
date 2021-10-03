#define STACK_SIZE (1024 * 1024)   
#define _GNU_SOURCE      // Needed for clone to be defined

#include "init.h"
#include "pid_stack.h"
#include <sys/mman.h> // For nman memory allocation
#include <sched.h>    // For clone
#include <stdio.h>    // For printf
#include <stdlib.h>   // For EXIT_FAILURE
#include <unistd.h>   // For getpid
#include <sys/wait.h> // For SIGCHLD

// Cannot avoid a global variable since arguments cannot be passed to the singal
// handlers
static PidStack pidStack;

static int _allocate_mem_init_process(Init_ProcessInfo* initProcessInfo) {
    initProcessInfo->stackBottom = mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);
    if (initProcessInfo->stackBottom == MAP_FAILED)
        return -1;
    // Assume stack grows downward 
    initProcessInfo->stackTop = initProcessInfo->stackBottom + STACK_SIZE; 
    return 0;
}

static void _create_child() {
    printf("Init Process here. My pid is %d. Creating Child.\n", getpid());
    printf("List of children before new child: ");
    Pid_Stack_printToStdOut(&pidStack);

    if (Pid_Stack_isFull(&pidStack)) {
        printf("The max number of children has been reached. "
            "Request for new child rejected.\n");
        return;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("Unable to create child");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        printf("Child here. My pid is %d. My parent is %d.\n", getpid(), getppid());
        // When the init process recieves a TERM signal. The children recieve
        // an INT signal. Lets ensure that the childen exit properly.
        while (true) {}
    } else {
        int ret = Pid_Stack_push(&pidStack, pid);
        if (ret == -1) {
            printf("Error occured when appending child to stack"
                "despite stack full pre check.\n");
            exit(EXIT_FAILURE);
        }

        printf("List of Children after new child: ");
        Pid_Stack_printToStdOut(&pidStack);
    }
}

static void _kill_youngest_child() { // Sounds rather morbid
    printf("Init Process here. My pid is %d. Attempting to kill child.\n", getpid());
    printf("List of children before kill: ");
    Pid_Stack_printToStdOut(&pidStack);

    pid_t pid = Pid_Stack_pop(&pidStack);
    if (pid == -1) {
        printf("There are no children to kill. Request denied.\n");
        return;
    } 
    int ret = kill(pid, SIGKILL);
    if (ret == -1) {
        perror("Unable to kill process");
        exit(EXIT_FAILURE);
    }

    printf("List of Children after kill: ");
    Pid_Stack_printToStdOut(&pidStack);
}

static void _graceful_shutdown() {
    printf("Init Process here. My pid %d. Attempting graceful shutdown.\n", getpid());
    while (!Pid_Stack_isEmpty(&pidStack)) {
        _kill_youngest_child();
    }
    exit(EXIT_SUCCESS);
}

static void _write_children_to_file() {
    printf("Init Process here. My pid is %d. Writing Children pids to file.\n", getpid());
    int ret = Pid_Stack_printToPidListTxt(&pidStack);
    if (ret == -1) {
        perror("unable to write children to file");
        exit(EXIT_FAILURE);
    }
}

// Would prefer to have this return void but clone expects its function arguemt
// will return an int. There will be a compiler warning if this function does
// not return an int.
static int _init_process_handler(void* arg) {
    signal(SIGUSR1, _create_child);
    signal(SIGUSR2, _write_children_to_file);
    signal(SIGINT, _kill_youngest_child);
    signal(SIGTERM, _graceful_shutdown);
    printf("Init Process here. My pid is %d.\n", getpid());
    while (true) {}
    return 0;
}

pid_t Init_processCreate(Init_ProcessInfo* initProcessInfo) {
    if (_allocate_mem_init_process(initProcessInfo) == -1) {
        printf("memory allocation failed\n");
        return -1;
    }
    Pid_Stack_init(&pidStack);
    return clone(_init_process_handler, initProcessInfo->stackTop, CLONE_NEWPID | SIGCHLD, NULL);
}

