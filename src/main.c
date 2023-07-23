//
// Created by beloin on 28/06/23.
//

#include "game/game.h"
#include "stdlib.h"
#include "server.h"
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "pthread.h"

// What to do:
//  1. Test calling the connection from another function (Run two applications calling connect_to/serve)
//  2. Send player information as a protocol:
//      |   3bit  |   8bit  |  9bit   |  9bit   |     16bit      | = 45 bits ~= 6 bytes
//      | command |    id   | x - pos | y - pos | internal clock |
//      Command: 0 -> new Player
//      Command: 1 -> Update position
//      Command: 2 -> Exit group
//      Command: 3 -> Server Response... Se below in 2.2 (1)
//      Command: 3 -> Simple Service Discovery Protocol... Se below in 2.2 (2)
//      Command: 4 -> Do a barrel row
//      Command: 5 -> Do a barrel row
//      2.1 The positions will be integer values representing 0 -> 500
// TODO: Implement this only later
//      2.2 (1) Server Response:
//          2.2.1 Message:
//              |   011   |            8Bit            |
//              | command | numbers of services (`ns`) |
//          2.2.2 Hosts list:
//              |         X Bits * `ns`         | Where X will be the host+port bytes
//              | List of hosts and their ports |
//      2.2 (2) Broadcast or use something to Discovery Services
//          2.2.1: Could use something like:
//              - [Plug And Play](https://en.wikipedia.org/wiki/Universal_Plug_and_Play)
//              - [SSDP](https://en.wikipedia.org/wiki/Simple_Service_Discovery_Protocol)
//              - (SSDP lightweight lib)[https://github.com/zlargon/lssdp]
//
//  3. Simulate the interface using printf();

// TODO NEXT: Now that we have a simple send and receive message, create a callback for the client response
//  Create too a way to easily configure client and server, chose ports etc, like: sh `dist_game 3031 localhost:3092 localhost:3093 localhost:3093`
//  then we will start to implement the game itself, starting only by moving some things
//  for now, the server will be listed in the start of the application, the *args will represent list of hosts and ports to connect.

Game game;

void *server_worker(void *arg) {
    char *port = (char *) arg;
    serve(port);
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
            // This should kill the serve thread and all clients threads
            pthread_cancel(server_thread);
            pthread_join(server_thread, NULL);
        }
    }

    delete_game(&game, 1);
    return 0;
}