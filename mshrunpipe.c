#define _GNU_SOURCE
#include<unistd.h>
#include<sys/wait.h>

#include "errors.h"
#include "mshutils.h"
#include "mshrun.h"

void run_pipe_cmd(char **argvleft, char **argvright){
    int pipefds[2]; //[0] for read end, [1] for write end
    pipe(pipefds);

    // for child1
    pid_t pid = fork();

    if(pid == -1){
        pr_stderr(FORKING_FAILED_ERROR, NULL);
        return;
    }

    // child1, write to pipe end
    if(pid == 0){
        close(pipefds[0]);
        dup2(pipefds[1], STDOUT_FILENO);
        close(pipefds[1]);

        execve(argvleft[0], argvleft, environ);
    }

    // for child2
    pid_t pid2 = fork();

    if(pid2 == -1){
        pr_stderr(FORKING_FAILED_ERROR, NULL);
        return;
    }

    // child2, read from pipe end
    if(pid2 == 0){
        close(pipefds[1]);
        dup2(pipefds[0], STDIN_FILENO);
        close(pipefds[0]);

        execve(argvright[0], argvright, environ);
    }

    // first close read end bcz else the child2 will wait forever for EOF
    // from read pipe. won't get EOF because the pipe is open in pipefds[0]
    // closing it gives EOF to child2 so it can exit and wait for pid2 can continue

    close(pipefds[0]);
    waitpid(pid, NULL, 0);
    close(pipefds[1]);
    waitpid(pid2, NULL, 0);
}
