#include "main.h"
#include "init.h"
#include <stdlib.h> /* For EXIT_SUCCESS or EXIT_FAILURE */
#include <stdio.h> /* For printf */
#include <unistd.h> /* For getpid */


int my_main() {
    printf("Entry Process here. My pid is %d.\n", getpid());
    Init_ProcessInfo initProcessInfo;
    pid_t initPid = Init_processCreate(&initProcessInfo);

    printf("Entry Process here. the initPid is %d.\n", initPid);

    sleep(1);
    return EXIT_SUCCESS;
}
