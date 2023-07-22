//
// Created by beloin on 17/07/23.
//

#include <netinet/in.h>
#include <stdio.h>
#include "server.h"

//  |   3bit  |   8bit  |  9bit   |  9bit   |     16bit      | = 45 bits ~= 6 bytes
//  | command |    id   | x - pos | y - pos | internal clock |

size_t send_message(const Message *msg, int sockfd) {
    unsigned char buf[PROTOCOL_BYTES];

    unwrap_protocol(msg, buf);

    return sbytes(sockfd, buf, PROTOCOL_BYTES);
}

size_t read_message(Message *msg, int sockfd) {
    unsigned char buf[PROTOCOL_BYTES];

    size_t bytes = rbytes(sockfd, buf, PROTOCOL_BYTES);

    if (bytes == PROTOCOL_BYTES) {
        wrap_protocol(msg, buf);
    } else {
        return bytes;
    }

    return bytes;
}

void wrap_protocol(Message *const message, const unsigned char bytes[]) {
    // TODO: Implement using ntohs
    //  ntohs();
    message->command = bytes[0] >> 5;
    message->id = bytes[0] << 3;
    message->id |= bytes[1] >> 5;

    message->x_pos = 0;
    message->x_pos = (unsigned char) (bytes[1] << 3);
    message->x_pos <<= 1;
    message->x_pos |= (unsigned char) (bytes[2] >> 4);

    message->y_pos = (unsigned char) (bytes[2] << 4);
    message->y_pos <<= 1;
    message->y_pos |= (unsigned char) (bytes[3] >> 3);

    message->internal_clock = bytes[3] << 5;
    message->internal_clock <<= 3;
    message->internal_clock |= bytes[4];
    message->internal_clock <<= 5;
    message->internal_clock |= bytes[5] >> 3;
}

void unwrap_protocol(const Message *message, unsigned char bytes[]) {
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


size_t rbytes(int socketfd, const unsigned char *buf, size_t n) {
    size_t bytes, missing_bytes, total = 0;
    while (total != n) { // Read all bytes into buf
        missing_bytes = n - total;
        size_t index = total;

        // When closed, this returns 0 bytes everytime it recv
        if ((bytes = recv(socketfd, (void *) (buf + index), missing_bytes, 0)) == -1 || bytes == 0) { // or &buf[index]
            if (bytes == -1) perror("could not get bytes");
            total = bytes;
            break;
        }

        total += bytes;
    }

    return total;
}

size_t sbytes(int socketfd, const unsigned char *buf, size_t n) {
    size_t bytes, missing_bytes, total = 0;
    while (total != n) {
        missing_bytes = n - total;
        size_t index = total;

        if ((bytes = send(socketfd, (void *) (buf + index), missing_bytes, 0)) == -1 || bytes == 0) {
            if (bytes == -1) perror("could not get bytes");
            total = bytes;
            break;
        }

        total += bytes;
    }

    return total;
}