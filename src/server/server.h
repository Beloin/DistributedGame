//
// Created by beloin on 09/07/23.
//

#ifndef DISTRIBUTED_SYS_SERVER_H
#define DISTRIBUTED_SYS_SERVER_H

#include <bits/types/sig_atomic_t.h>

#define PROTOCOL_BYTES 6

typedef struct {
    unsigned char command;
    unsigned char id;
    unsigned short x_pos;
    unsigned short y_pos;
    unsigned short internal_clock;
    // TODO: Implement checksum
} Message;

static sig_atomic_t should_quit;

/**
 * typedef defining the type to hold a value for applications arrays and indexes 😀
 */
typedef unsigned char app_size;

int server(char *port);

int connect_to(char *host, char *port);

void close_connections();

void send_message(const Message *msg, int sockfd);

/**
 * Function that wraps the bytes into struct `Message`.
 * @param message
 * @param bytes Must be PROTOCOL_BYTES sized
 */
void wrap_protocol(Message *const message, const unsigned char bytes[]);

/**
 * Function that unwraps the struct `Message` into bytes.
 * @param message
 * @param bytes Must be PROTOCOL_BYTES sized
 */
void unwrap_protocol(Message *const message, unsigned char bytes[]);

#endif //DISTRIBUTED_SYS_SERVER_H
