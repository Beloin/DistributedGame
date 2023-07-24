//
// Created by beloin on 23/07/23.
//

#include <stdint-gcc.h>
#include "stdlib.h"
#include "parser.h"
#include "string.h"


static char size = -1;
static char *port; // Server port

static struct Address2 addresses[100]; // TODO: Create a config file to add this

/**
 * Args will be `dist_game 3031 localhost:3092 localhost:3093 localhost:3093`
 * @param argc
 * @param argv
 */
int parse_args(int argc, char **argv) {
    int i, add_index = 0;
    char *c_arr;

    if (argc < 3) {
        return -1;
    }

    for (i = 2; i < argc; ++i) {
        c_arr = argv[i];

        // While c[x] != '\0'
        int c_index = 0;
        uint8_t has_parsed = 0;
        while (c_arr[c_index] != '\0') {
            if (c_arr[c_index] == ':') {
                has_parsed = 1;
                // Unwrap values

                struct IAddress *address = (struct IAddress *) &addresses[add_index];


                // abc:123
                address->host = (char *) malloc(sizeof(char) * c_index - 1);
            }
            c_index++;
        }

        if (has_parsed) {
            size++;
        }
    }
}

int get_address(struct IAddress **address) { // TODO: Try to send IAddress instead of address
    return size;
}

int get_server_port(char **c) {
    if (size == -1) { // TODO: There must be a way to use only bitwise operators
        return -1;
    }

    *c = port;

    return size;
}