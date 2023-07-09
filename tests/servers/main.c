//
// Created by beloin on 09/07/23.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// Program that reads the results from servinfo
int main() {
    int sockfd, numbytes;
    char buf[100];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];
    char hostname[] = "localhost";


    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(hostname, "6463", &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
        inet_ntop(p->ai_family, p->ai_addr, s, sizeof(s));

        printf("IP: %s\n", s);
        printf("Info: %s -> %d\n", p->ai_addr->sa_data, p->ai_addr->sa_family);
        printf("Protocol: %d\n", p->ai_protocol);
        printf("IPvX: %d\n", p->ai_family);
        printf("Name: %s\n", p->ai_canonname);
    }

    freeaddrinfo(servinfo);
}