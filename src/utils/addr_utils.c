//
// Created by beloin on 28/06/23.
//

#include <sys/socket.h>
#include <netinet/in.h>

#include "addr_utils.h"

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}