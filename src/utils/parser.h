//
// Created by beloin on 23/07/23.
//

#ifndef DISTRIBUTED_SYS_PARSER_H
#define DISTRIBUTED_SYS_PARSER_H

struct Address {
    char *port;
    char *host;
};

int get_address(struct Address *address);

int get_port(char **c);

int parse_args(int argc, char **argv);

#endif //DISTRIBUTED_SYS_PARSER_H
