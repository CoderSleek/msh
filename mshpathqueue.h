#ifndef MSH_PATHQUEUE_H
#define MSH_PATHQUEUE_H

struct Node {
    char *path;
    struct Node* next;
};

typedef struct Queue {
    struct Node* head;
    struct Node* tail;
} Queue;

void enqueue(Queue*, char*);
struct Node* dequeue(Queue*);
void free_pathnode(struct Node*);
void free_all_queue_nodes(Queue*);

int _change_dir_default(const char*, char*);
char* _create_child_path(const char const*, const char const*, const int);
void msh_jump(char*, char*);

#endif