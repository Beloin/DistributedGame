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
#include "linked_list.h"

#include "stdlib.h"


// TODO: In connect_to we will need to save the server sock_fd and sent when necessary, we will have MAX_SERVICES servers

struct server_t {
    int fd;
    app_size id;
};

LinkedList servers;
app_size current_server_id = 0;
uint8_t has_initialized = 0;

int get_socketfd(char *host, char *port, int *server_fd);

void initialize_server_list();

void add_server(int fd, app_size id);

void initialize_server_list() {
    if (has_initialized) {
        return;
    }
    create_linked_list(&servers);
    has_initialized = 1;
}

void add_server(int fd, app_size id) {
    struct server_t *server = (struct server_t *) malloc(sizeof(struct server_t));
    if (server == NULL) {
        // TODO: Make a better handler
        close(fd);
        perror("add_server:");
        return;
    }
    server->fd = fd;
    server->id = id;

    add_item(&servers, server); // When cleaning linked_list, remember to free the servers inside linked list
}

void close_servers() {
    struct Node *node = servers.head;
    while (node != NULL) {
        const struct server_t *server = (struct server_t *) node->value;
        close(server->fd);
        node = node->next;
    }

    delete_linked_list(&servers, 1);
}

void broadcast_message(const Message message) {
    struct Node *node = servers.head;
    while (node != NULL) {
        const struct server_t *server = (struct server_t *) node->value;
        send_message(&message, server->fd);
        node = node->next;
    }
}

// TODO NEXT: Do the same for the client, close all socket connections when connect_to thread is killed
//  We need too to find a way to add various server to this guy and send to each new messages
int connect_to(char *host, char *port) {
    int server_fd;

    initialize_server_list();
    get_socketfd(host, port, &server_fd);

    add_server(server_fd, ++current_server_id);
    return 0;
}

int get_socketfd(char *host, char *port, int *server_fd) {
    struct addrinfo hints, *servinfo, *p;
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(host, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((*server_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("connect_to: socket");
            continue;
        }

        if (connect(*server_fd, p->ai_addr, p->ai_addrlen) == -1) {
            close(*server_fd);
            perror("connect_to: connect");
            continue;
        }

        break;
    }


    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n"); // Printing directly to stderr
        return 3;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *) p->ai_addr), s, sizeof s);
    printf("client: connecting to %s\n", s);
    freeaddrinfo(servinfo);

    return 0;
}