//
// Created by beloin on 06/07/23.
//


#include "gtest/gtest.h"
#include "linked_list.h"

TEST(LinkedListTests, should_add_one_item) {
    LinkedList linkedList;
    float p = 12.F;

    create_linked_list(&linkedList);
    print_linked_list_info(&linkedList);
    add_item(&linkedList, &p);

    ASSERT_EQ(1, linkedList.len) << "Length was not modified";
    ASSERT_EQ(12.F, *((float *) linkedList.head->value)) << "Value is not accessed";
}

TEST(LinkedListTests, should_get_first_node) {
    LinkedList linkedList;
    float p = 12.F;

    create_linked_list(&linkedList);
    print_linked_list_info(&linkedList);
    add_item(&linkedList, &p);

    struct Node node{};
    get_node(&linkedList,0, &node);

    ASSERT_EQ(1, linkedList.len) << "Length was not modified";
    ASSERT_EQ(12.F, *((float *) linkedList.head->value)) << "Value is not accessed";
}
