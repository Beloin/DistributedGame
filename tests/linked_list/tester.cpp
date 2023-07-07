//
// Created by beloin on 06/07/23.
//


#include "gtest/gtest.h"
#include "linked_list.c" // Problem trying to import only the header

TEST(LinkedListTests, should_add_one_item) {
    LinkedList linkedList;
    float p = 12.F;

    create_linked_list(&linkedList);
    print_linked_list_info(&linkedList);
    add_item(&linkedList, &p);

    ASSERT_EQ(1, linkedList.len) << "Length was not modified";
    ASSERT_EQ(12.F, *((float *) linkedList.head->value)) << "Value is not accessed";
}

TEST(LinkedListTests, should_get_first_node_when_theres_one) {
    LinkedList linkedList;
    float p = 12.F;

    create_linked_list(&linkedList);
    print_linked_list_info(&linkedList);
    add_item(&linkedList, &p);

    struct Node node{};
    get_node(&linkedList, 0, &node);

    ASSERT_EQ(0, linkedList.len) << "Length was not modified";
    ASSERT_EQ(12.F, *((float *) node.value)) << "Value is not accessed";
    ASSERT_EQ(NULL, linkedList.head) << "Head should be deleted";
}

TEST(LinkedListTests, should_get_second_node) {
    LinkedList linkedList;
    float v1 = 12.F;
    float v2 = 17.F;
    float v3 = 45.F;

    create_linked_list(&linkedList);
    print_linked_list_info(&linkedList);

    add_item(&linkedList, &v1);
    add_item(&linkedList, &v2);
    add_item(&linkedList, &v3);

    struct Node node{};
    int err = get_node(&linkedList, 1, &node);
    ASSERT_EQ(0, err) << "err: Cannot find node";
    return;

    ASSERT_EQ(0, linkedList.len) << "Length was not modified";
    ASSERT_EQ(v2, *((float *) node.value)) << "Value is not accessed";

    ASSERT_NE((void *) NULL, linkedList.head) << "Head should not be null";
    ASSERT_EQ(v1, *((float *) linkedList.head->value)) << "Head should not be null";

    ASSERT_NE((void *) NULL, linkedList.head->next) << "Next should not be null";
    ASSERT_EQ(v3, *((float *) linkedList.head->next)) << "Next should be the third one";

    ASSERT_EQ((void *) NULL, linkedList.head->next->next) << "The third one should be Null since it is now the second";
}

TEST(LinkedListTests, should_free_all_linked_list) {
    LinkedList linkedList;
    float v1 = 12.F;
    float v2 = 17.F;
    float v3 = 45.F;

    create_linked_list(&linkedList);

    add_item(&linkedList, &v1);
    add_item(&linkedList, &v2);
    add_item(&linkedList, &v3);

    free_linked_list_nodes(&linkedList);
    void *pointer_to_throw;
    ASSERT_EXIT((pointer_to_throw = linkedList.head->value), ::testing::KilledBySignal(SIGSEGV), ".*");
    ASSERT_EQ((void *) NULL, linkedList.head) << "Head should be null";
}

TEST(LinkedListTests, should_remove_second_node) {
    LinkedList linkedList;
    float v1 = 12.F;
    float v2 = 17.F;
    float v3 = 45.F;

    create_linked_list(&linkedList);
    add_item(&linkedList, &v1);
    add_item(&linkedList, &v2);
    add_item(&linkedList, &v3);

    int err = remove_node(&linkedList, 1);
    ASSERT_EQ(0, err) << "err: Cannot find node";

    ASSERT_NE((void *) NULL, linkedList.head) << "Head should not be null";
    ASSERT_EQ(v1, *((float *) linkedList.head->value)) << "Head should not be null";

    ASSERT_NE((void *) NULL, linkedList.head->next) << "Next should not be null";
    ASSERT_EQ(v3, *((float *) linkedList.head->next)) << "Next should have the third value";

    ASSERT_EQ((void *) NULL, linkedList.head->next->next) << "The third one should be Null since it is now the second";
}