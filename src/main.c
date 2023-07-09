//
// Created by beloin on 28/06/23.
//

#include "game/game.h"
#include "stdlib.h"

// What to do:
//  1. Test calling the connection from another function (Run two applications calling client/server)
//  2. Send player information as a protocol:
//      |   2bit  |   8bit  |  9bit   |  9bit   |
//      | command |    id   | x - pos | y - pos |
//      Command: 0 -> new Player
//      Command: 1 -> Update position
//      Command: 2 -> Exit
//      Command: 3 -> Do a barrel row
//  3. Simulate the interface using printf();

int main() {
    Game game;
    Player *me = malloc(sizeof(Player));
    create_game(&game);
    add_player(&game, me);


    delete_game(&game, 1);
    return 0;
}