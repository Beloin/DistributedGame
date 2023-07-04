//
// Created by beloin on 29/06/23.
//

#include "linked_list.h"

void create_linked_list(LinkedList *linkedList) {
    linkedList->len = 0;
    linkedList->head = NULL;
}

int add_item(LinkedList *linkedList, void *item) {
    int index = 0;
    Node *c_node = linkedList->head;
    for (; c_node != NULL; index++) {
        if (c_node->next == NULL) {
            Node *new_node = (Node *) malloc(sizeof(struct Node *));

            new_node->value = item;
            new_node->prev = (struct Node *) c_node;
            c_node->next = (struct Node *) new_node;

            break;
        } else {
            c_node = (Node *) (c_node->next);
        }
    }

    return index;
}