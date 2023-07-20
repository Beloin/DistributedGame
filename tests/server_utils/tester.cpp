//
// Created by beloin on 19/07/23.
//

#include "gtest/gtest.h"
#include "server_utils.c" // Problem trying to import only the header

// Example: 111-10100101-111111010-101010101-1110001110100101
// Packed: [11110100][10111111][10101010][10101111][00011101][00101(000)]
//  command = 0b00000111
//  id = 0b10100101
//  x_pos = 0b00000001_11111010
//  y_pos = 0b00000001_01010101
//  internal_clock= 0b11100011_10100101

TEST(ServerUtilsTests, should_unwrap_example) {
    Message message;
    message.command = 0b00000111;
    message.id = 0b10100101;
    message.x_pos = 0b0000000111111010;
    message.y_pos = 0b0000000101010101;
    message.internal_clock = 0b1110001110100101;
    // | 11110100 | 10111111 | 10101010 | 10101111 | 00011101 | 00101(000)

    unsigned char buf[6];
    unwrap_protocol(&message, buf);

    ASSERT_EQ(buf[0], 0b11110100);
    ASSERT_EQ(buf[1], 0b10111111);
    ASSERT_EQ(buf[2], 0b10101010);
    ASSERT_EQ(buf[3], 0b10101111);
    ASSERT_EQ(buf[4], 0b00011101);
    ASSERT_EQ(buf[5], 0b00101000);
}