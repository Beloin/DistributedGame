//
// Created by beloin on 29/06/23.
//

#include "linked_list.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"

void create_linked_list(LinkedList *linkedList) {
    linkedList->len = 0;
    linkedList->head = NULL;
    linkedList->max_size = (long) pow(2, sizeof(unsigned int) * 8);
}

void print_linked_list_info(LinkedList *linkedList) {
    printf("{\n");
    printf("\tlen: %d\n", linkedList->len);
    printf("\tmax_size: %lu\n", linkedList->max_size);
    printf("}\n");
}

int add_item(LinkedList *linkedList, void *item) {
    if (linkedList->max_size == linkedList->len) {
        return -1;
    }

    int index = 0;
    struct Node *c_node = linkedList->head;

    struct Node *new_node = (struct Node *) malloc(sizeof(struct Node *));
    new_node->value = item;
    new_node->next = NULL;
    new_node->prev = NULL;

    if (c_node == NULL) {
        linkedList->head = new_node;
        linkedList->len++;
        return index;
    }

    for (; c_node->next != NULL; index++) {
        c_node = (c_node->next);
    }

    new_node->prev = c_node;
    c_node->next = new_node;

    linkedList->len++;

    return index;
}

void free_linked_list_nodes(LinkedList *linkedList) {
    struct Node *cur_node;
    cur_node = linkedList->head;
    while (cur_node != NULL) {
        struct Node *temp = cur_node;
        cur_node = cur_node->next;
        free(temp);
    }
}

int remove_node(LinkedList *linkedList, int n) {
    unsigned int index = 0;
    struct Node *cur_node = linkedList->head;

    if (linkedList->len < n + 1) {
        return -1;
    }

    while (index != n) {
        cur_node = cur_node->next;
        index++;
    }

    if (index == 0) {
        linkedList->head = cur_node->next;
        cur_node->next->prev = NULL;
    } else {
        cur_node->prev->next = cur_node->next;

        if (cur_node->next != NULL) {
            cur_node->next->prev = cur_node->prev;
        }
    }

    free(cur_node); // The value are not freed

    linkedList->len--;

    return n;
}

int get_node(LinkedList *linkedList, int i, struct Node *node) {
    unsigned int index = 0;
    struct Node *cur_node = linkedList->head;

    if (linkedList->len < i + 1) {
        return 1;
    }

    while (index != i) {
        cur_node = cur_node->next;
        index++;
    }

    *node = *cur_node; // This copy values

    if (index == 0) {
        linkedList->head = cur_node->next;
        cur_node->next->prev = NULL;
    } else {
        cur_node->prev->next = cur_node->next;

        if (cur_node->next != NULL) {
            cur_node->next->prev = cur_node->prev;
        }
    }

    free(cur_node);

    return 0;
}


int peek_node(LinkedList *linkedList, int i, struct Node *node) {
    unsigned int index = 0;
    struct Node *cur_node = linkedList->head;

    if (linkedList->len < i + 1) {
        return 1;
    }

    while (index != i) {
        cur_node = cur_node->next;
        index++;
    }

    *node = *cur_node; // This copy values

    return 0;
}

