#ifndef MODEL_H
#define MODEL_H

#include <stdio.h>
#include <stdlib.h>

#define HEIGHT 15
#define WIDTH 10

typedef struct GameState {
    char** field; // tetris board
    int score;    // current game score 
} State;

typedef struct Coordinate {
    int x, y;
} Coordinate;

State* game_start(int width, int height);

void game_end(int width, int height, State* gameState);
char get_cell_value(Coordinate coor, State* gameState);
void set_cell_value(Coordinate coor, State* gameState, char setValue); 

#endif