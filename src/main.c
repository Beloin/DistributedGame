//
// Created by beloin on 28/06/23.
//

#include "game/game.h"
#include "stdlib.h"
#include "server.h"
#include <stdio.h>
#include <string.h>
#include <bits/types/sig_atomic_t.h>
#include <signal.h>
#include <unistd.h>
#include "pthread.h"

// What to do:
//  1. Test calling the connection from another function (Run two applications calling connect_to/server)
//  2. Send player information as a protocol:
//      |   3bit  |   8bit  |  9bit   |  9bit   |     16bit      | = 45 bits ~= 6 bytes
//      | command |    id   | x - pos | y - pos | internal clock |
//      Command: 0 -> new Player
//      Command: 1 -> Update position
//      Command: 2 -> Exit group
//      Command: 3 -> Server Response... Se below in 2.2
//      Command: 4 -> Do a barrel row
//      Command: 5 -> Do a barrel row
//      2.1 The positions will be integer values representing 0 -> 500
//      2.2 Server Response: // TODO: Implement this only later
//          2.2.1 Message:
//              |   011   |            8Bit            |
//              | command | numbers of services (`ns`) |
//          2.2.2 Hosts list:
//              |         X Bits * `ns`         | Where X will be the host+port bytes
//              | List of hosts and their ports |
//  3. Simulate the interface using printf();

Game game;

void *server_worker(void *arg) {
    char *port = (char *) arg;
    server(port);
    return NULL;
}

void sig_handler(int sig, siginfo_t *sinfo, void *context) {
    should_quit = 1;
}

int main(int argc, char **argv) {
    int err;
    if (argc < 2) {
        printf("err: need option");
        exit(1);
    }


    Player *me = malloc(sizeof(Player));
    create_game(&game);
    add_player(&game, me);

    pthread_t server_thread;

    u_int8_t should_serve = strcmp(argv[2], "server") == 0;
    if (should_serve) {
        err = pthread_create(&server_thread, NULL, server_worker, argv[1]);

        if (err != 0) {
            printf("\ncan't create thread :[%s]", strerror(err));
            exit(1);
        }

    } else {
        char hostname[] = "127.0.0.1";
        connect_to(hostname, "3092");
    }

    printf("My PID is %d\n", getpid());

    // This Setup SIGTERM and SIGINT handlers.
    struct sigaction act_setup;
    act_setup.sa_sigaction = sig_handler;
    sigemptyset(&act_setup.sa_mask);
    act_setup.sa_flags = SA_SIGINFO | // This sends the information instead of only the signal number
                         SA_RESTART;

    if (sigaction(SIGTERM, &act_setup, NULL) == -1 || sigaction(SIGINT, &act_setup, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
    pause();

    if (should_quit) {
        printf("\nquitting application...\n");
        close_connections();

        if (should_serve) {
            // This should kill the server thread and all clients threads
            pthread_cancel(server_thread);
            pthread_join(server_thread, NULL);
        }
    }

    delete_game(&game, 1);
    return 0;
}