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
//    ntohs() // TODO: Use this? Only after you understand how it works


    message->command = bytes[0] >> 5; // 0bXXX00000 >> 5 = 0b00000XXX
    message->id = 0;
    //                          First 5 Bits of 8
    message->id = message->id | (bytes[0] << 3);
    message->id = message->id | bytes[1];
}

void unwrap_protocol(Message *const message, unsigned char bytes[]) {
    // TODO: Implement using htons
//    htons();
    bytes[0] = message->command << 5;
    // 0b10101010
    bytes[0] = bytes[0] | message->id >> 3;
    bytes[1] = message->id << 5;
}