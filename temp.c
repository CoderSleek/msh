#include <stdio.h>
#include<string.h>

int main() {
    char * a="hello from another | world | welcome";
    char *tok = NULL;
    tok = strtok(a, "|");
    if (tok) printf("%s\n", tok);
    while(tok) {
        tok = strtok(NULL, "|");
        if (tok) printf("%s\n", tok);
    }
    return 0;
}