#include "pid_stack.h"
#include <stdio.h>

void Pid_Stack_init(PidStack* pidStack) {
    pidStack->top = -1;
    for (int i = 0; i < MAX_SIZE; i++) {
        pidStack->buffer[i] = -1;
    }
}

pid_t Pid_Stack_pop(PidStack* pidStack) {
    if (Pid_Stack_isEmpty(pidStack)) {
        return -1;
    }
    pid_t poppedPid = pidStack->buffer[pidStack->top];
    pidStack->buffer[pidStack->top] = -1;
    pidStack->top--;
    return poppedPid;
}

int Pid_Stack_push(PidStack* pidStack, pid_t pid) {
    if (pidStack->top == (MAX_SIZE - 1)) {
        return -1;
    }
    pidStack->top++;
    pidStack->buffer[pidStack->top] = pid;
    return 0;
}

bool Pid_Stack_isEmpty(const PidStack* pidStack) {
    return pidStack->top == -1;
}

bool Pid_Stack_isFull(const PidStack* pidStack) {
    return pidStack->top == (MAX_SIZE - 1);
}

void Pid_Stack_printToStdOut(const PidStack* pidStack) {
    printf("[");
    if (!Pid_Stack_isEmpty(pidStack)) {
        printf("%d", pidStack->buffer[0]);
    }
    if (pidStack->top >= 1) {
        for (int i = 1; i <= pidStack->top; i++) {
            printf(" ,%d", pidStack->buffer[i]);
        }
    }
    printf("]\n");
}


