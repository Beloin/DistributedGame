//
// Created by beloin on 29/06/23.
//

#ifndef DISTRIBUTED_SYS_LINKED_LIST_H
#define DISTRIBUTED_SYS_LINKED_LIST_H

#include "stdlib.h"

typedef struct {
    void *value;
    struct Node *next;
    struct Node *prev;
} Node;

typedef struct {
    Node *head;
    int len;
    unsigned char max_size;
} LinkedList;

void create_linked_list(LinkedList *linkedList);

void free_linked_list(LinkedList *linkedList);

int add_item(LinkedList *linkedList, void *item);

void remove_node(LinkedList *linkedList, int n);

void get_item(LinkedList *linkedList, int index, void *p);


#endif //DISTRIBUTED_SYS_LINKED_LIST_H
