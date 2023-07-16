//
// Created by beloin on 28/06/23.
//

#ifndef DISTRIBUTED_SYS_GAME_H
#define DISTRIBUTED_SYS_GAME_H

#include "../player/player.h"
#include "linked_list.h"

typedef struct {
    int i, j;
    LinkedList players;
} Game;


// TODO: Make sense use a matrix to update the screen? Or is better to act only as a list of itens
void create_game(Game *game);
void delete_game(Game *game, char should_free_pointers);

void add_player(Game *game, Player *player);

#endif //DISTRIBUTED_SYS_GAME_H
