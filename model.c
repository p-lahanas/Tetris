#include "model.h"

/* Initialises the game state
    
    Parameters:
        int width: the width of the tetris fiel
        int height: the height of the tetris field

    Returns:
        State*: a pointer to the allocated State */
State* game_start(int width, int height) {
    State* newGame = (State*) calloc(1, sizeof(State));

    newGame->field = (char**) calloc(width, sizeof(char*));

    for (int i = 0; i < width; i++) {
        newGame->field[i] = (char*) calloc(height, sizeof(char));
    }

    return newGame;
}

/* Frees any allocated memory for the game state*/
void game_end(int width, int height, State* gameState) {
    for (int i = 0; i < width; i++) {
        free(gameState->field[i]);
    }
    free(gameState->field);
}

/* Returns the char at the given field coordinate*/
char get_cell_value(Coordinate coor , State* gameState) {
    return gameState->field[coor.x][coor.y];
}

/* Sets the char at the given field coordinate*/
void set_cell_value(Coordinate coor, State* gameState, char setValue) {
    gameState->field[coor.x][coor.y] = setValue;
}




int main(int argc, char** argv) {
    State* gameState = game_start(WIDTH, HEIGHT);
    gameState->field[0][0] = 'a';

    for (int j = 0; j < HEIGHT; j++) {
        for(int i = 0; i < WIDTH; i++) {
            printf("%c", gameState->field[i][j]);
        }
        printf("\n");
    }
    game_end(WIDTH, HEIGHT, gameState);
    return 0;
}