#include "main.h"
#include "init.h"
#include <stdlib.h>   // For EXIT_SUCCESS or EXIT_FAILURE 
#include <stdio.h>    // For printf                       
#include <unistd.h>   // For getpid                       
#include <sys/wait.h> // For wait                         

int main() {
    printf("Entry Process here. My pid is %d.\n", getpid());
    Init_ProcessInfo initProcessInfo;
    pid_t initPid = Init_processCreate(&initProcessInfo);
    if (initPid == -1) {
        perror("Failed to create init process");
        return EXIT_FAILURE;
    }

    printf("Entry Process here. The initPid is %d.\n", initPid);

    int wait_status;
    // Zero means wait indefinately for process to finish
    int ret = waitpid(initPid, &wait_status, 0); 
    if (ret == -1) {
        perror("Failed to wait for init process");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
