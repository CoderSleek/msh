#ifndef MSHUTILS_H
#define MSHUTILS_H

extern char **environ;

void get_input(char*, size_t *);
char** tokenize_string(char*, char*);
void strtrim(char*);
void set_env();
void set_terminal();
void pr_stderr(char*, char*);
void set_PS1(char*);
char* command_in_path(char*);
void write_to_history(char*);

#endif