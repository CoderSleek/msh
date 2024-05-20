#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<ctype.h>
#include<unistd.h>

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

char** tokenize_string(char *str){
    char** tokens, *delimitor = (char*)" ", *token = NULL;
    int token_count = 0;

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

char** set_env() {
    size_t len = 0, count = 0;
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
        return NULL;
    }

    while(getline(&line, &len, fp) != -1){
        strtrim(line);
        if(strcmp(line, "") != 0) ++count;
    }
    
    char **envs = (char**)malloc(sizeof(char*) * (count + 1));
    if(!envs) {
        pr_stderr(MEMORY_ERROR, NULL);
        exit(MEMORY_CODE);
    }
    fseek(fp, 0, SEEK_SET);

    int i = 0;
    while(getline(&line, &len, fp) != -1){
        strtrim(line);
        int line_length = strlen(line) + 1;
        envs[i] = (char*)malloc(sizeof(char) * line_length);
        memcpy(envs[i], line, line_length);
        ++i;
    }
    envs[i] = NULL;

    fclose(fp);
    free(path);

    return envs;
}

void clear_terminal(){
    printf("\033[H\033[J");
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