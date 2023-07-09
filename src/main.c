//
// Created by beloin on 28/06/23.
//

#include "game/game.h"
#include "stdlib.h"
#include "server.h"
#include <stdio.h>
#include <string.h>
#include "pthread.h"

// What to do:
//  1. Test calling the connection from another function (Run two applications calling client/server)
//  2. Send player information as a protocol:
//      |   2bit  |   8bit  |  9bit   |  9bit   |     16bit      | = 44 bits
//      | command |    id   | x - pos | y - pos | internal clock |
//      Command: 0 -> new Player
//      Command: 1 -> Update position
//      Command: 2 -> Exit
//      Command: 3 -> Do a barrel row
//  3. Simulate the interface using printf();

Game game;

void *server_worker(void *arg) {
    char *port = (char *) arg;
    server(port);
    return NULL;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("err: need option");
        exit(1);
    }

    pthread_t server_thread;

    Player *me = malloc(sizeof(Player));
    create_game(&game);
    add_player(&game, me);

    pthread_create(&server_thread, NULL, server_worker, argv[1]);

//    client("3092");

    pthread_join(server_thread, NULL);
    delete_game(&game, 1);
    return 0;
}