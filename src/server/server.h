//
// Created by beloin on 09/07/23.
//

#ifndef DISTRIBUTED_SYS_SERVER_H
#define DISTRIBUTED_SYS_SERVER_H

#define PROTOCOL_BYTES 6

typedef struct {
    // ...
} Message;


int server(char *port);

int client(char *port);

/**
 * Function that wraps the struct `Message` into bytes.
 * @param message
 * @param bytes Must be PROTOCOL_BYTES sized
 */
void wrap_protocol(Message const *message, char bytes[]);

/**
 * Function that unwraps the bytes into struct `Message`.
 * @param message
 * @param bytes Must be PROTOCOL_BYTES sized
 */
void unwrap_protocol(Message const *message, char bytes[]);

#endif //DISTRIBUTED_SYS_SERVER_H
