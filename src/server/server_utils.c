//
// Created by beloin on 17/07/23.
//

#include "server.h"

//  |   2bit  |   8bit  |  9bit   |  9bit   |     16bit      | = 44 bits ~= 6 bytes
//  | command |    id   | x - pos | y - pos | internal clock |

void send_message(const Message *msg, int sockfd) {
    // TODO: Implement
}

void wrap_protocol(Message *const message, const char bytes[]) {
    // TODO: Implement
    message->command = bytes[0] >> 5; // 0bXXX00000 >> 5
}

void unwrap_protocol(Message *const message, char bytes[]) {
    // TODO: Implement

}