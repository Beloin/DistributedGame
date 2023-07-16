//
// Created by beloin on 25/06/23.
//
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include "addr_utils.h"
#include "server.h"

// TODO: In client we will need to save the server sock_fd and sent when necessary, we will have MAX_SERVICES servers

// TODO NEXT: Do the same for the client, close all socket connections when client thread is killed
//  We neeed too to find a way to add various server to this guy and send to eache new messages
int client(char *port) {
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];
    char hostname[] = "localhost";


    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(hostname, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }


    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *) p->ai_addr), s, sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo);

    send(sockfd, "Hello!", 6, 0);
    printf("client sent 6 bytes\n");


    ssize_t i = send(sockfd, "Hello!", 6, 0);
    close(sockfd); // Later on keep this open

    return 0;
}