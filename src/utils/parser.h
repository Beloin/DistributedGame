//
// Created by beloin on 23/07/23.
//

#ifndef DISTRIBUTED_SYS_PARSER_H
#define DISTRIBUTED_SYS_PARSER_H

// TODO: Remember to FREE the port and host?
struct Address {
    char *arr;
    unsigned char _portlen; // port_offset = _hostlen
    unsigned char _hostlen;
};

struct IAddress { // TODO: Try to extends this
    void (*get_port)(char *port);
    void (*get_host)(char *host);
};

struct Address2 {
    void (*get_port)(char *port);
    void (*get_host)(char *host);

    char *arr;
    unsigned char _portlen;
    unsigned char _hostlen;
};

int get_address(struct Address *address);

int get_server_port(char **c);

int parse_args(int argc, char **argv);

#endif //DISTRIBUTED_SYS_PARSER_H
