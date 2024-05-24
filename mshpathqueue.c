#include<stdlib.h>
#include<string.h>

#include "mshpathqueue.h"

void enqueue(Queue *queue, char *path) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->path = path;
    newNode->next = NULL;

    if (queue->head == NULL) {
        queue->head = newNode;
        queue->tail = newNode;
    } else {
        queue->tail->next = newNode;
        queue->tail = newNode;
    }
}

struct Node* dequeue(Queue *queue) {
    if (queue->head == NULL) {
        return NULL;
    }

    if(queue->head == queue->tail) {
        struct Node *temp = queue->head;
        queue->head = queue->tail = NULL;
        return temp;
    } else {
        struct Node *temp = queue->head;
        queue->head = queue->head->next;
        return temp;
    }
}

inline void free_pathnode(struct Node* node) {
    if (node == NULL) {
        return;
    }

    free(node->path);
    free(node);
}

void free_all_queue_nodes(Queue *queue) {
    struct Node* curr = queue->head, *temp = NULL;
    while (curr != NULL) {
        temp = curr;
        curr = curr->next;
        free_pathnode(temp);
    }
}