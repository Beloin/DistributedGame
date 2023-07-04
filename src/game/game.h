//
// Created by beloin on 28/06/23.
//

#ifndef DISTRIBUTED_SYS_GAME_H
#define DISTRIBUTED_SYS_GAME_H

#include "../player/player.h"

typedef struct {
    int i, j;
    float *mx;
    Player **players; // TODO: Use a linked list
} Matrix;

void create_matrix(Matrix *matrix, int i, int j);
void free_matrix(Matrix *matrix);

// TODO: Make sense use a matrix to update the screen? Or is better to act only as a list of itens
void add_player();

#endif //DISTRIBUTED_SYS_GAME_H
