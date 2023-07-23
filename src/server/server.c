//
// Created by beloin on 18/06/23.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "addr_utils.h"
#include "server.h"

#define BACKLOG 10


struct client_t {
    int fd;
    app_size id; // Add a padding?
    uint8_t is_enabled;
    pthread_t thread;
};

struct client_t clients[MAX_SERVICES];
char current_client_index = 0;
app_size current_client_id = 1;

void *handle_client(void *args);

void handle_socket(int new_fd);

void initialize_clients() {
    app_size i;
    for (i = 0; i < MAX_SERVICES; i++) {
        struct client_t *cur_client = &clients[i];
        cur_client->id = 1;
        cur_client->fd = -1;
        cur_client->thread = -1;
        cur_client->is_enabled = 0;
    }
}

// TODO: This is not thread safe, please use semaphore
void remove_client(app_size id, int sock_fd) {
    app_size i, c_index;
    close(sock_fd);

    // [ 1, (2), 3]
    // [ 1, 3, -1 ]
    for (i = 0; i < MAX_SERVICES; i++) {
        struct client_t *cur_client = &clients[i];
        if (cur_client->id == id) {
            c_index = i;
        }

        if (i >= c_index && i + 1 != MAX_SERVICES) {
            clients[i] = clients[i + 1];
        } else if (i + 1 == MAX_SERVICES) {
            struct client_t *client = &clients[i + 1];
            client->id = -1;
            client->fd = -1; // Could close socket here instead of receiving socket from parameter
            client->thread = 0; // Won't be needing to pthread_cancel these clients since when a `remove_client` is called, the threads should already be returned
            client->is_enabled = 0;
        }
    }

    current_client_index--;
}

static void clean_clients(void *) {
    printf("cleaning still active clients\n");
    app_size i;
    for (i = 0; i < MAX_SERVICES; i++) {
        struct client_t *cur_client = &clients[i];
        if (cur_client->is_enabled) {
            printf("killing client (%d)\n", cur_client->id);
            close(cur_client->fd);
            pthread_cancel(cur_client->thread);
        }
    }
}

int serve(char *port) {
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
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
    pthread_cleanup_push(clean_clients, NULL) ;

            char *name = "0.0.0.0"; // Could be localhost too. But since we are using AI_PASSIVE, the host is the same machine as this
            if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
                fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
            }

            for (p = servinfo; p != NULL; p = p->ai_next) {
                // This get socket information
                if ((server_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
                    perror("serve: socket");
                    continue;
                }

                // This enables re-utilization of ports
                if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
                    perror("setsockopt");
                    exit(1);
                }

                if (bind(server_fd, p->ai_addr, p->ai_addrlen) == -1) {
                    close(server_fd);
                    perror("serve: bind");
                    continue;
                }

                break;
            }
            freeaddrinfo(servinfo);


            if (p == NULL) {
                fprintf(stderr, "serve: failed to bind");
                exit(1);
            }

            if (listen(server_fd, BACKLOG) == -1) {
                perror("listen");
                exit(1);
            }

            printf("serve (%s): waiting for connections...\n", port);

            while (should_quit != 1) {
                struct sockaddr_storage their_addr;
                socklen_t sin_size = sizeof their_addr;
                // Accept is a cancellation point, so we should be able to kill the serve thread here
                int new_fd = accept(server_fd, (struct sockaddr *) &their_addr, &sin_size);

                if (new_fd == -1) {
                    perror("accept");
                    continue;
                }

                inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *) &their_addr), s, sizeof s);
                printf("serve: got connection from %s\n", s);

                handle_socket(new_fd);
            }

    pthread_cleanup_pop(0); // This helps to add cleanup to threads

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
        client->is_enabled = 1;

        pthread_detach(client_thread); // Detaching this thread since we will not be waiting for any client
        current_client_index++;
    } else {
        printf("could not create another connect_to: we are full\n");
        close(new_fd);
    }
}

void *handle_client(void *args) {
    app_size client_id;
    int client_sd;
    client_sd = ((struct client_t *) args)->fd;
    client_id = ((struct client_t *) args)->id;

    // 45 bits, inside 48
    size_t bytes_read;
    Message message;
    while (1) {
        bytes_read = read_message(&message, client_sd); // TODO: We could use `select` instead of `recv`.

        if (bytes_read != PROTOCOL_BYTES) {
            if (bytes_read == -1) {
                printf("could not read this message on client (%d)\n", client_id);
            } else {
                printf("client (%d) disconnected\n", client_id);
            }
            break; // Normally errors like this are some kind of connection end like: EINTR
        }

        // TODO: Create a callback?
        printf("serve recv 6 bytes\n");
        printf("{\n\tcommand: %d,\n\tid: %d,\n\tx_pos: %d,\n\ty_pos: %d,\n\tinternal_clock: %d\n}\n",
               message.command, message.id, message.x_pos, message.y_pos, message.internal_clock);
    }

    remove_client(client_id, client_sd);

    return NULL; // Thread stop
}

