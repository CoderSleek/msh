#ifndef MSHUTILS_H
#define MSHUTILS_H

void get_input(char*, size_t *);
char** tokenize_string(char*);
void strtrim(char*);
char** set_env();
void clear_terminal();
void pr_stderr(char*, char*);

#endif