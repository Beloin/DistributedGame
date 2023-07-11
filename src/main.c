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
//  1. Test calling the connection from another function (Run two applications calling client/server)
//  2. Send player information as a protocol:
//      |   2bit  |   8bit  |  9bit   |  9bit   |     16bit      | = 44 bits
//      | command |    id   | x - pos | y - pos | internal clock |
//      Command: 0 -> new Player
//      Command: 1 -> Update position
//      Command: 2 -> Exit group
//      Command: 3 -> Do a barrel row
//  3. Simulate the interface using printf();

Game game;
volatile sig_atomic_t should_quit; // This is quite interesting, but it's necessary in this thing?

void *server_worker(void *arg) {
    char *port = (char *) arg;
    server(port);
}

void handle_sigterm() {
    should_quit = 1; // Use a semaphore instead?
}

void *simplethread(void *arg) {
    printf("started thread\n");
    while (1) {
        if (should_quit) {
            printf("quitting from simple thread\n");
            return NULL;
        }
        sleep(2);
        printf("I am running!\n");
    }
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
    err = pthread_create(&server_thread, NULL, simplethread, argv[1]);
    if (err != 0) {
        printf("\ncan't create thread :[%s]", strerror(err));
        exit(1);
    }

    // TODO: How to run client?
//    client("3092");

    printf("My PID is %d\n", getpid());

    (void) signal(SIGTERM, handle_sigterm);
    pause(); // This method waits for signal... Is it in the main thread?

    if (should_quit) {
        printf("killing\n");
        pthread_join(server_thread, NULL);
    }

    delete_game(&game, 1);
    return 0;
}