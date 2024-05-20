#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include<stdlib.h>

#include "errors.h"
#include "mshutils.h"

void run_pipe_cmd(char **left, char **right){
    int pipefds[2]; //ind 0 for read, ind 1 for write
    pipe(pipefds);

    pid_t pid = fork();

    if(pid == -1){
        pr_stderr(FORKING_FAILED_ERROR, NULL);
        return;
    }

    if(pid == 0){
        close(pipefds[0]);
        char *msg = "hello from alternate brother";
        int size = strlen(msg) + 1;
        write(pipefds[1], &size, sizeof(size));
        write(pipefds[1], msg, size);
        close(pipefds[1]);
        return;
    }

    waitpid(pid, NULL, 0);
    pid_t pid2 = fork();

    if(pid2 == -1){
        pr_stderr(FORKING_FAILED_ERROR, NULL);
        return;
    }

    if(pid2 == 0){
        int size;
        char *msg = NULL;

        close(pipefds[1]);
        read(pipefds[0], &size, sizeof(size));
        msg = (char*)malloc((size + 1) * sizeof(char));
        read(pipefds[0], msg, size);

        printf("inside child 2 %s\n", msg);
        free(msg);
        close(pipefds[0]);
        return;
    }
    waitpid(pid2, NULL, 0);
    close(pipefds[0]);
    close(pipefds[1]);
}