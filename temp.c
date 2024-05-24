#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<dirent.h>
#include<sys/stat.h>
#include<stdio.h>
#include<limits.h>
#include<libgen.h>
#include<signal.h>

int main(void) {
    const char* str = "😊";
    printf("%d", strlen(str));
}