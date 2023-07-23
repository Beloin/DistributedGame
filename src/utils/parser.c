//
// Created by beloin on 23/07/23.
//

#include <stdint-gcc.h>
#include "parser.h"


static char size = -1;
static struct Address addresses[100]; // TODO: Create a config file to add this
static char *port;

/**
 * Args will be `dist_game 3031 localhost:3092 localhost:3093 localhost:3093`
 * @param argc
 * @param argv
 */
int parse_args(int argc, char **argv) {
    int i;
    char *c_arr;

    if (argc < 3) {
        return -1;
    }

    for (i = 2; i < argc; ++i) {
        c_arr = argv[i];

        // While c[x] != '\0'

        size++;
    }
}

int get_address(struct Address *address) {
    return size;
}

int get_port(char **c) {
    if (size == -1) { // TODO: There must be a way to use only bitwise operators
        return -1;
    }

    *c = port;

    return size;
}