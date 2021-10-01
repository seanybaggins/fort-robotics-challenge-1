#ifndef INIT_H
#define INIT_H

#include <sys/types.h> /* For file permissions passed to nman and pid_t*/

typedef struct {
    char* stackBottom;
    char* stackTop;
} Init_ProcessInfo;

// Creates the init process with it's own pid namespace.
// Arguments:
//      An uninitialized Init_InitProcessInfo strcut. During invokation of the
//      function, members of the struct will be set.
// Returns:
//      The pid of the process created. The process id will be -1 if an error
//      occured. And perror will be set.
pid_t Init_processCreate(Init_ProcessInfo* initProcessInfo); 

#endif // INIT_H
