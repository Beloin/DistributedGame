//
// Created by beloin on 06/07/23.
//

#ifndef DISTRIBUTED_SYS_LINKEDLISTTESTS_HPP
#define DISTRIBUTED_SYS_LINKEDLISTTESTS_HPP

#include <gtest/gtest.h>
#include "linked_list.h"

class LinkedListTests : public testing::Test {

    void SetUp() override {}

    void TearDown() override {}

    TEST_F(LinkedListTests, should_add_item_to_linked_list) {
        EXPECT_EQ(13, 13);
    }

};


#endif //DISTRIBUTED_SYS_LINKEDLISTTESTS_HPP
