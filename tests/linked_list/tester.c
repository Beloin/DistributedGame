#include <assert.h>
#include "stdio.h"
#include "linked_list.h"

int main() {
    int err;
    float p = 12.F;

    LinkedList linkedList;
    create_linked_list(&linkedList);
    print_linked_list_info(&linkedList);

    add_item(&linkedList, &p);
    if (linkedList.len != 1) return 1;

    print_linked_list_info(&linkedList);

    struct Node peek;
    peek_node(&linkedList, 0, &peek);
    if (linkedList.len != 1) return 1;

    if (*(float *) peek.value != 12) return 1;

    return 0;
}