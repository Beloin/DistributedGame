//
// Created by beloin on 29/06/23.
//

#ifndef DISTRIBUTED_SYS_LINKED_LIST_H
#define DISTRIBUTED_SYS_LINKED_LIST_H

struct Node {
    void *value;
    struct Node *next;
    struct Node *prev;
};

typedef struct {
    struct Node *head;
    unsigned int len;
    unsigned long max_size;
} LinkedList;

void create_linked_list(LinkedList *linkedList);

void delete_linked_list(LinkedList *linkedList, char should_free_values);

void print_linked_list_info(LinkedList *linkedList);

int add_item(LinkedList *linkedList, void *item);

/**
 *
 * This frees the node, but don't actually free the value inside it. A better approach should be `get_node`, so the user
 * can have the Node to properly free it and free the value.
 *
 * @param linkedList
 * @param n
 * @return 0 OK
 * @return 1 Error
 */
int remove_node(LinkedList *linkedList, int n);

int get_node(LinkedList *linkedList, int i, struct Node *node);

int peek_node(LinkedList *linkedList, int i, struct Node *node);


#endif //DISTRIBUTED_SYS_LINKED_LIST_H
