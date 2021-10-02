#ifndef PID_STACK_H
#define PID_STACK_H
#define MAX_SIZE 10
#include <sys/types.h>
#include <stdbool.h>

typedef struct {
    int top;
    pid_t buffer[MAX_SIZE];
} PidStack;

void Pid_Stack_init(PidStack* pidStack);

bool Pid_Stack_isEmpty(const PidStack* pidStack);

bool Pid_Stack_isFull(const PidStack* pidStack);

// Return:
//      Success: The pid of the element off the top of the stack.
//      Failure: -1
pid_t Pid_Stack_pop(PidStack* pidStack);

// Return: 
//      Success: 0
//      Failure: -1
int Pid_Stack_push(PidStack* pidStack, pid_t pid);

#endif // PID_STACK_H
