#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<ctype.h>
#include<unistd.h>
#include<libgen.h>
#include<stdbool.h>
#include<sys/stat.h>
#include<limits.h>

#include "constants.h"
#include "errors.h"
#include "mshutils.h"

void get_input(char *buffer_input, size_t *size){
    int success = getline(&buffer_input, size, stdin);

    if(success == -1){
        pr_stderr(INPUT_FAILED_ERROR, NULL);
        exit(INPUT_FAILED_CODE);
    }

    if(*size > BUF_SIZE){
        pr_stderr(INPUT_TOO_LONG_ERROR, NULL);
        exit(INPUT_TOO_LONG_CODE);
    }

    strtrim(buffer_input);
    return;
}

char** tokenize_string(char *str, char *delimitor){
    char** tokens, *token = NULL;
    int token_count = 1;

    for(int i = 0; str[i] != '\0'; ++i) {
        if(isspace(str[i]) && !isspace(str[i+1])) ++token_count;
    }

    tokens = (char**)malloc(sizeof(char*) * (token_count + 1));

    if(!tokens) {
        pr_stderr(ARGV_ALLOC_FAILED_ERROR, NULL);
        exit(ARGV_ALLOC_FAILED_CODE);
    }

    int i = 0;
    token = strtok(str, delimitor);
    if(token) tokens[i++] = token;

    while(token){
        token = strtok(NULL, delimitor);
        tokens[i++] = token;
    }

    return tokens;
}

void strtrim(char *str){
    int len = strlen(str);
    char str_cpy[len+1], *start;

    strcpy(str_cpy, str);
    start = str_cpy;

    while(isspace(start[0])) start++;

    if(*start == '\0') {
        strcpy(str, "");
        return;
    }

    int i = len - 1;
    while (isspace(str_cpy[i])) str_cpy[i--] = '\0';

    strcpy(str, start);
    return;
}

void set_env() {
    size_t len = 0;
    char *homedir = getenv("HOME"), *line = NULL;
    char *path = (char*)malloc((strlen(homedir) + strlen(MSH_ENV) + 1) * sizeof(char));
    if(!path) {
        pr_stderr(MEMORY_ERROR, NULL);
        exit(MEMORY_CODE);
    }
    strcpy(path, homedir);
    strcat(path, MSH_ENV);
    
    FILE *fp = fopen(path, "r");

    if(!fp) {
        return;
    }

    while(getline(&line, &len, fp) != -1){
        strtrim(line);
        char *key = strtok(line, "="), *value = strtok(NULL, "=");
        setenv(key, value, 1);
    }

    fclose(fp);
    free(path);
}

void set_terminal(){
    const char* clear_screen = "\e[1;1H\e[2J";
    write(STDOUT_FILENO, clear_screen, strlen(clear_screen));//"\033[H\033[J";
    write(STDOUT_FILENO, MSH_STARTUP, strlen(MSH_STARTUP));
    return;
}

// function to print colored error message to stderr
void pr_stderr(char *msg, char *command){
    if(!command) {
        fprintf(stderr, "%s%s%s", RED_COLOR, msg, NO_COLOR);
    } else {
        fprintf(stderr, "%s%s: %s%s%s", YELLOW_COLOR, command, RED_COLOR, msg, NO_COLOR);
    }
    return;
}

void set_PS1(char *PS1) {
    char *username = getenv("USER"), *pwd = basename(getcwd(NULL, 0));
    memset(PS1, 0, PS1_SIZE);
    snprintf(PS1, PS1_SIZE, "%s%s %s%s 😊 %s", BLUE_COLOR, username, GREEN_COLOR, pwd, NO_COLOR);
    return;
}

char* command_in_path(char *command_name){
    char *paths = strdup(getenv("PATH")), *token = NULL;
    char *current_path = (char*)malloc(sizeof(char) * PATH_MAX);
    struct stat info;
    bool is_execable = true;

    token = strtok(paths, ":");

    while(token) {
        current_path = strcpy(current_path, token);
        strcat(current_path, "/");
        strcat(current_path, command_name);

        int ret = stat(current_path, &info);

        if(ret == 0 && (S_IXUSR & info.st_mode)) {
            free(paths);
            return current_path;
        } else if (ret == 0) {
            is_execable = false;
        }

        token = strtok(NULL, ":");
    }

    if(!is_execable) {
        pr_stderr("The file does not have executable rights\n", command_name);
    } else {
        pr_stderr("Command not found\n", command_name);
    }

    free(paths);
    return NULL;
}

void write_to_history(char* command) {
    char *homedir = getenv("HOME"), *path = (char*)malloc((strlen(homedir) + strlen(MSH_HISTORY) + 1) * sizeof(char));
    if(!path) {
        pr_stderr(MEMORY_ERROR, NULL);
        exit(MEMORY_CODE);
    }
    strcpy(path, homedir);
    strcat(path, MSH_HISTORY);

    FILE *fp = fopen(path, "a+");

    fprintf(fp, "%s\n", command);
    fclose(fp);
    free(path);
    return;
}