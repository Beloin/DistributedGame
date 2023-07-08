//
// Created by beloin on 28/06/23.
//

#include "game.h"
#include "stdlib.h"

void create_game(Game *game) {
    game->i = 500;
    game->j = 500;
    create_linked_list(&game->players);
}

void delete_game(Game *game, char should_free_pointers) {
    delete_linked_list(&game->players, should_free_pointers);
}

void add_player(Game *game, Player *player) {
    add_item(&game->players, player);
}
