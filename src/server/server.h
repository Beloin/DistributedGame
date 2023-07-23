//
// Created by beloin on 09/07/23.
//

#ifndef DISTRIBUTED_SYS_SERVER_H
#define DISTRIBUTED_SYS_SERVER_H

#include <bits/types/sig_atomic_t.h>

#define MAX_SERVICES 5
#define PROTOCOL_BYTES 6

typedef struct {
    unsigned char command;
    unsigned char id;
    unsigned short x_pos;
    unsigned short y_pos;
    unsigned short internal_clock;
    // TODO: Implement checksum?
} Message;

static volatile sig_atomic_t should_quit;

/**
 * typedef defining the type to hold a value for applications arrays and indexes 😀
 */
typedef unsigned char app_size;

int serve(char *port);

int connect_to(char *host, char *port);

void close_connections();

size_t send_message(const Message *msg, int sockfd);

size_t read_message(Message *msg, int sockfd);

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
void unwrap_protocol(const Message *message, unsigned char bytes[]);

/**
 * Reads N bytes from socket fd. If error, return -1
 * If client disconnected, returns 0
 * @param socketfd
 * @param buf
 * @param n
 * @return
 */
size_t rbytes(int socketfd, const unsigned char *buf, size_t n);

/**
 * Send N bytes to socket fd. If error, return -1
 * If client disconnected, returns 0
 * @param socketfd
 * @param buf
 * @param n
 * @return
 */
size_t sbytes(int socketfd, const unsigned char *buf, size_t n);

#endif //DISTRIBUTED_SYS_SERVER_H
