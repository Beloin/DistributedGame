//
// Created by beloin on 17/07/23.
//

#include <netinet/in.h>
#include "server.h"

//  |   3bit  |   8bit  |  9bit   |  9bit   |     16bit      | = 45 bits ~= 6 bytes
//  | command |    id   | x - pos | y - pos | internal clock |

void send_message(const Message *msg, int sockfd) {
    // TODO: Implement
}

void wrap_protocol(Message *const message, const unsigned char bytes[]) {
    // TODO: Implement using ntohs
    //  ntohs();

    // Example: 111-10100101-111111010-101010101-1110001110100101
    // Packed: [11110100][10111111][10101010][10101111][00011101][00101(000)]
    //  command = 0b00000111
    //  id = 0b10100101
    //  x_pos = 0b00000001_11111010
    //  y_pos = 0b00000001_01010101
    //  internal_clock= 0b11100011_10100101

    message->command = bytes[0] >> 5; // 0b11110100 >> 5 = 0b00000111
    message->id = bytes[0] << 3; // 0b11110100 << 3 = 0b10100000
    message->id |= bytes[1] >> 5; // 0b10111111 >> 5 = 0b00000101

    message->x_pos = bytes[1] << 3; // 0b10111111 << 3 = 0b11111000
    message->x_pos <<= 1; // Should be in the 9th to 5th
    message->x_pos |= bytes[2] >> 4; // 0b10101010 >> 4 = 0b00001010

    message->y_pos = bytes[2] << 4; // 0b10101010 << 4 = 0b10100000
    message->y_pos <<= 1; // Should be in the 9th to 6th
    message->y_pos |= bytes[3] >> 3; // 0b10101111 >> 3 = 0b00010101

    message->internal_clock = bytes[3] << 5; // 0b10101111 << 5 = 0b11100000
    message->internal_clock <<= 3;
    message->internal_clock |= bytes[4];
    message->internal_clock <<= 5;
    message->internal_clock |= bytes[5] >> 3; // 0b00101(000) >> 3 = 0b00000101
}

void unwrap_protocol(Message *const message, unsigned char bytes[]) {
    // TODO: Implement using htons
    //  htons();
    bytes[0] = message->command << 5;
    bytes[0] = bytes[0] | (message->id >> 3);

    bytes[1] = message->id << 5;
    bytes[1] = bytes[1] | (message->x_pos >> 4);

    bytes[2] = message->x_pos << 4;
    bytes[2] = bytes[2] | (message->y_pos >> 5);

    bytes[3] = message->y_pos << 3;
    bytes[3] = bytes[3] | (message->internal_clock >> 13);

    bytes[4] = message->internal_clock >> 5;
    bytes[5] = message->internal_clock << 3;
}