#ifndef MODEL_H
#define MODEL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>

#define HEIGHT 20
#define WIDTH 25
#define FREE_CELL 0
#define PIECE_CELL 1
#define ACTIVE_PIECE_CELL 2
#define MAT 4 // the size of the piece matrix

#define ACTIVE '1'

typedef struct Coordinate {
    int x, y;
} Coordinate;

/* Model pieces as a grid with the middle set as the axis of rotation*/
typedef struct Piece {
    Coordinate position;
    char matrix[4][4];  // the grid layout of the piece   
} Piece;

typedef struct GameState {
    char** field; // tetris board
    int score, width, height; // current game score
    Piece* currentPiece; // a pointer to the current piece being moved
} State;

typedef enum Move {
    ROTATE = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 3,
    ERROR = -1
} Move;

State* game_start(int width, int height);
void game_end(int width, int height, State* gameState);

char get_cell_value(Coordinate coor, State* gameState);
void set_cell_value(Coordinate coor, State* gameState, char setValue); 
void game_update(State* gameState);

#endif