#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>

#include "mshpathqueue.h"
#include "mshutils.h"

inline char *_create_child_path(const char const *parent_full_path, const char const *child_name, const int parent_size) {
    char *child_full_path = (char*)malloc(sizeof(char) + (parent_size + strlen(child_name) + 3));

    strcpy(child_full_path, parent_full_path);
    if(child_full_path[parent_size - 1] != '/') {
        strcat(child_full_path, "/");
    }
    strcat(child_full_path, child_name);
    strcat(child_full_path, "/");

    return child_full_path;
}

int _change_dir_default(const char *dest, char *start) {
    Queue queue = {NULL, NULL};
    struct stat info;
    enqueue(&queue, start);
    int return_value = 1, dest_len = strlen(dest); // default, failure directory not found

    // full_path_len_limit for max string len post which will realloc & increase value
    int full_path_len_limit = 1024;
    char *full_path = (char*)malloc(sizeof(char) * full_path_len_limit);

    do {
        struct Node* node = dequeue(&queue);
        int path_len = strlen(node->path);

        if (path_len + dest_len + 1 > full_path_len_limit) {
            full_path_len_limit = path_len + dest_len + 1;
            full_path = (char*)realloc(full_path, sizeof(char) * full_path_len_limit);
        }

        strcpy(full_path, node->path);
        strcat(full_path, dest);

        int status = stat(full_path, &info);

        if(status == 0 && S_ISDIR(info.st_mode)) {
            chdir(full_path);
            return_value = 0;
            break;
        } else if (status == 0 && !S_ISDIR(info.st_mode)) {
            return_value -1; // file not a directory
        } else {
            DIR *dir = opendir(node->path);
            struct dirent *dir_details;

            if(dir) {
                dir_details = readdir(dir);

                while(dir_details != NULL) {
                    if(strcmp(dir_details->d_name, ".") == 0 || strcmp(dir_details->d_name, "..") == 0) {
                        dir_details = readdir(dir);
                        continue;
                    }

                    // +3 to account for 2 /(optional) and \0
                    char *child_path = _create_child_path(node->path, dir_details->d_name, path_len);                  

                    stat(child_path, &info);

                    if(S_ISDIR(info.st_mode)) {
                        enqueue(&queue, child_path);
                    } else {
                        free(child_path);
                    }

                    dir_details = readdir(dir);
                }
                closedir(dir);
            }
        }
        free_pathnode(node);
    } while (queue.head != NULL);

    free(full_path);
    free_all_queue_nodes(&queue);
    return return_value;
}

void msh_jump(char *argv1, char* argv2){
    char *dest = strdup(argv1), *path;

    if(argv2 == NULL) {
        path = strdup(getenv("HOME"));
    } else {
        path = strdup(argv2);
    }

    // +1 to account for final '/' which is appended
    char *full_path = (char*)malloc(sizeof(char) * (PATH_MAX + 1));

    realpath(path, full_path);

    int len = strlen(full_path);
    if(full_path[len-1] != '/') {
        strcat(full_path, "/");
    }

    int ret = _change_dir_default(dest, full_path);

    if(ret == 1) {
        pr_stderr("No such directory exists\n", "jump");
    } else if (ret == -1) {
        pr_stderr("No such directory exists\n", "jump");
    }
}