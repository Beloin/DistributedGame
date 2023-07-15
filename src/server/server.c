//
// Created by beloin on 18/06/23.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>

#include "addr_utils.h"
#include "server.h"

#define MAX_SERVICES 4
#define BACKLOG 10

typedef char size_client;

struct client_t {
    size_client id;
    int fd;
    pthread_t thread;
};

struct client_t clients[MAX_SERVICES];
char current_client_index = 0;
char current_client_id = 1;

void *handle_client(void *args);

size_t rbytes(int client_sd, const char *buf);

void handle_socket(int new_fd);

#define clean_clients_def(arg) { \
                                  \
}

void clean_clients(void *arg) {

}

void initialize_clients() {
    size_client i;
    for (i = 0; i < MAX_SERVICES; i++) {
        struct client_t *cur_client = &clients[i];
        cur_client->id = -1;
        cur_client->fd = -1;
        cur_client->thread = -1;
    }
}

void remove_client(size_client id, int sock) {
    size_client i, j;
    close(sock);
    for (i = 0; i < MAX_SERVICES; i++) {
        struct client_t *cur_client = &clients[i];
        if (cur_client->id == id) {
            cur_client->id = -1;
            cur_client->fd = -1;
            cur_client->thread = -1;

            for (j = MAX_SERVICES - 1; j > i; --j) {
                clients[i] = clients[j];
                i++;
            }

            break; // This doesn't need, since we will be doing i++
        }
    }
}

int server(char *port) {
    int server_fd;
    struct addrinfo hints, *servinfo, *p;

    int yes = 1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    initialize_clients();
//    pthread_cleanup_push(clean_clients, NULL);

    char *name = "0.0.0.0"; // Could be localhost too. But since we are using AI_PASSIVE, the host is the same machine as this
    if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
        // This get socket information
        if ((server_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        // This enables re-utilization of ports
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(server_fd, p->ai_addr, p->ai_addrlen) == -1) {
            close(server_fd);
            perror("server: bind");
            continue;
        }

        break;
    }
    freeaddrinfo(servinfo);


    if (p == NULL) {
        fprintf(stderr, "server: failed to bind");
        exit(1);
    }

    if (listen(server_fd, BACKLOG) == -1) {
        perror("listen");
    }

    printf("server (%s): waiting for connections...\n", port);

    while (1) {
        struct sockaddr_storage their_addr;
        socklen_t sin_size = sizeof their_addr;
        // Accept is a cancellation point, so we should be able to kill the server thread here
        int new_fd = accept(server_fd, (struct sockaddr *) &their_addr, &sin_size);

        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *) &their_addr), s, sizeof s);
        printf("server: got connection from %s\n", s);

        // TODO FROM HERE -> Create handler for this threads and a way to kill it when necessary remembering to call close(fd);
        handle_socket(new_fd);
    }

    return 0;
}

void handle_socket(int new_fd) {
    if (current_client_index < MAX_SERVICES) {
        pthread_t client_thread;

        struct client_t *client = &clients[current_client_index];
        client->id = current_client_id++;
        client->fd = new_fd;

        pthread_create(&client_thread, NULL, handle_client, client);

        client->thread = client_thread;
        current_client_index++;
    } else {
        printf("could not create another client: we are full\n");
        close(new_fd);
    }
}

void *handle_client(void *args) {
    char client_id;
    int client_sd;
    client_sd = ((struct client_t *) args)->fd;
    client_id = ((struct client_t *) args)->id;

    // 44 bits
    char buf[6];
    size_t bytes_read;
    while (1) {
        bytes_read = rbytes(client_sd, buf);

        if (bytes_read != PROTOCOL_BYTES) {
            printf("could not read this message on client %d\n", client_id);
            break; // Normally errors like this are some kind of connection end like: EINTR
        }

        printf("server recv 6 bytes\n");
        // TODO: Unwrap protocol
        printf("Value: %s\n", buf);
    }

    close()

}

size_t rbytes(int client_sd, const char *buf) {
    size_t bytes, missing_bytes, total = 0;
    while (total != PROTOCOL_BYTES) { // Read all bytes into buf
        missing_bytes = PROTOCOL_BYTES - total;
        size_t index = total;

        if ((bytes = recv(client_sd, (void *) (buf + index), missing_bytes, 0)) == -1) { // or &buf[index]
            perror("could not get bytes");
            total = -1;
            break;
        }

        total += bytes;
    }

    return total;
}
