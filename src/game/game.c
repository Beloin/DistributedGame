//
// Created by beloin on 28/06/23.
//

#include "game.h"
#include "stdlib.h"

void create_matrix(Matrix *matrix, int i, int j) {
    matrix->mx = (float *) malloc(sizeof(float) * i * j);
}

void free_matrix(Matrix *matrix) {
    free(matrix->mx);
}