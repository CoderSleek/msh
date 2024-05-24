#define _GNU_SOURCE
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "errors.h"
#include "mshutils.h"
#include "mshrun.h"

int run_basic_command(char **argv) {
    pid_t pid = fork();

    if(pid == -1) {
        pr_stderr(FORKING_FAILED_ERROR, NULL);
        return FORKING_FAILED_CODE;
    }

    if(pid == 0){
        int ret = execve(argv[0], argv, environ);

        if(ret == -1){
            pr_stderr(CMD_EXEC_FAILED_ERROR, argv[0]);
            return CMD_EXEC_FAILED_CODE;
        }

        exit(0);
    } else if(pid != 0) {
        wait(NULL);
    }

    return 0;
}