// RPEL Shell M(odern)sh

#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#include "constants.h"
#include "mshutils.h"
#include "errors.h"
#include "mshpathqueue.h"
#include "mshrun.h"


int main(void){
    char *PS1 = (char*)malloc(sizeof(char) * PS1_SIZE),
    *buffer_input = (char*)malloc(sizeof(char) * BUF_SIZE);

    size_t buf_size = BUF_SIZE;
    unsigned int return_value = 0;

    if(!buffer_input){
        pr_stderr(BUFFER_ALLOC_ERROR, NULL);
        return BUFFER_ALLOC_CODE;
    }
    
    set_env();
    set_terminal();
    set_PS1(PS1);

    while(1){
        write(STDOUT_FILENO, PS1, PS1_SIZE);
        get_input(buffer_input, &buf_size);
        char *input_copy = strdup(buffer_input);

        if(strcmp(buffer_input, "") == 0) {
            continue;
        }

        bool is_pipe = strchr(buffer_input, '|') != NULL;

        if(!is_pipe) {
            char **argv = tokenize_string(buffer_input, " ");

            if(strcmp(argv[0], "exit") == 0) {
                break;
            } else if (strcmp(argv[0], "goto") == 0) {
                chdir(argv[1]);
                set_PS1(PS1);
            } else if (!strcmp(argv[0], "jump")) {
                if(!argv[1]) {
                    pr_stderr("jump requires atleast 1 argument\n", "jump");
                } else {
                    msh_jump(argv[1], argv[2]);
                    set_PS1(PS1);
                }
            } else {
                char* command = command_in_path(argv[0]);

                if(!command) {
                    free(argv);
                    continue;
                };

                argv[0] = command;

                return_value = run_basic_command(argv);
                free(command);
            }

            free(argv);
        } else {
            char **argvleft = NULL, **argvright = NULL;
            char **temp = tokenize_string(buffer_input, "|");

            argvright = tokenize_string(temp[1], " ");
            argvleft = tokenize_string(temp[0], " ");
            free(temp);

            char* command1 = command_in_path(argvleft[0]);
            char* command2 = command_in_path(argvright[0]);

            if(command1 && command2) {
                argvleft[0] = command1;
                argvright[0] = command2;
                run_pipe_cmd(argvleft, argvright);
            }

            free(command1);
            free(command2);
            free(argvleft);
            free(argvright);

            if(!command1 || !command2) continue;
        }

        write_to_history(input_copy);
        free(input_copy);
    }

    free(buffer_input);
    free(PS1);
    write(STDOUT_FILENO, MSH_FINISH, strlen(MSH_FINISH));
    return return_value;
}
