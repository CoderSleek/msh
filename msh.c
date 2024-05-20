// REPL Shell M(odern)sh
#define _GNU_SOURCE

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>

#include "constants.h"
#include "mshutils.h"
#include "errors.h"

char *PS1 = "😊 ";

int main(void){
    char **env = NULL, *buffer_input = (char*)malloc(sizeof(char) * BUF_SIZE);
    size_t buf_size = BUF_SIZE;
    unsigned int return_value = 0;

    if(!buffer_input){
        pr_stderr(BUFFER_ALLOC_ERROR, NULL);
        return BUFFER_ALLOC_CODE;
    }
    
    env = set_env();

    clear_terminal();
    printf("%s", MSH_STARTUP);

    while(1){
        char **argv = NULL;

        printf("%s", PS1);
        get_input(buffer_input, &buf_size);
        if(strcmp(buffer_input, "") == 0) {
            continue;
        }

        argv = tokenize_string(buffer_input);

        if(strcmp(argv[0], "exit") == 0) {
            free(argv);
            break;
        } else if (strcmp(argv[0], "goto") == 0) {
            chdir(argv[1]);
            continue;
        }

        pid_t pid = fork();

        if(pid == -1) {
            pr_stderr(FORKING_FAILED_ERROR, NULL);
            return_value = FORKING_FAILED_CODE;
            break;
        }

        if(pid == 0){
            int ret = execvpe(argv[0], argv, env);

            if(ret == -1){
                pr_stderr(CMD_EXEC_FAILED_ERROR, argv[0]);
                return CMD_EXEC_FAILED_CODE;
            }

            exit(0);
        } else if(pid != 0) {
            wait(NULL);
        }

        free(argv);
    }

    // free env using for loop
    for(int i = 0; env[i]; ++i) {
        free(env[i]);
    }

    free(buffer_input), free(env);
    printf("%s", MSH_FINISH);
    return return_value;
}