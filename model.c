#include "model.h"


char L[4][4] = 
{
    {' ', '1', ' ', ' '},
    {' ', '1', ' ', ' '},
    {'1', '1', ' ', ' '},
    {' ', ' ', ' ', ' '}
};

/* Creates a new piece and add it to gameState*/
Piece* new_piece(State* gameState, char pieceType[MAT][MAT]) {
    Piece* newPiece = (Piece*) calloc(1, sizeof(Piece));
    for (int i = 0; i < MAT; i++) {
        for (int j = 0; j < MAT; j++) {
            newPiece->matrix[i][j] = pieceType[i][j];
        }
    }
    // shift left by half length of piece grid
    newPiece->position.x = WIDTH/2 - (MAT/2);
    newPiece->position.y = 0;
    gameState->currentPiece = newPiece;
    return newPiece;
}

/* Frees the memory allocated to the current piece*/
void deactivate_piece(State* gameState) {
    free(gameState->currentPiece);
}

/* Transforms the input square matrix into it's transpose*/
void rotate(char matrix[4][4], int dim) {
    // Allocate new matrix
    char** tempMatrix = (char**) calloc(dim, sizeof(char*));
    for (int k = 0; k < dim; k++) {
        tempMatrix[k] = (char*) calloc(dim, sizeof(char));
    }
    // transpose old matrix in new matrix
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            tempMatrix[j][i] = matrix[i][j];
        }
    }
    // assign new matrix values to old matrix in reverse order
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            matrix[i][j] = tempMatrix[i][dim - j - 1];
        }
    }

    // free new matrix
    for (int l = 0; l < dim; l++) {
        free(tempMatrix[l]);
    }
    free(tempMatrix);
}


// returns true if the current piece is still active
bool is_piece_active(State* gameState) {
    for (int j = 0; j < MAT; j++) {
        for (int i = 0; i < MAT; i++) {
            // check if we are at the bottom
            Coordinate coor = {i + gameState->currentPiece->position.x, 
                    j + gameState->currentPiece->position.y};
            if (gameState->currentPiece->matrix[i][j] == ACTIVE && 
                    coor.y >= gameState->height) {
                return false;
            }
            // is next square occupied
            if (gameState->currentPiece->matrix[i][j] == ACTIVE && 
                    get_cell_value(coor, gameState) == PIECE_CELL) {
                return false;
            }
        }
    }
    return true;
}

/* Initialises the game state   
    
    Parameters:
        int width: the width of the tetris field
        int height: the height of the tetris field

    Returns:
        State*: a pointer to the allocated State */
State* game_start(int width, int height) {
    State* newGame = (State*) calloc(1, sizeof(State));

    newGame->field = (char**) calloc(width, sizeof(char*));

    for (int i = 0; i < width; i++) {
        newGame->field[i] = (char*) calloc(height, sizeof(char));
    }
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            newGame->field[i][j] = FREE_CELL;
        }
    }

    newGame->width = WIDTH;
    newGame->height = HEIGHT;
    newGame->currentPiece = new_piece(newGame, L);

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

void clear_inactive_cells(State* gameState) {
    for (int i = 0; i < gameState->width; i++) {
        for (int j = 0; j < gameState->height; j++) {
            if (gameState->field[i][j] == ACTIVE_PIECE_CELL) {
                gameState->field[i][j] = FREE_CELL;
            }            
        }
    }
}
// Returns direction on success otherwise ERR
Move get_move() {
    char input = getch();
    switch(input) {
        case 'w':
            return ROTATE;
        case 'a':
            return LEFT;
        case 'd':
            return RIGHT;
        case 's':
            return DOWN;
        default:
            return ERROR;
    }
    return ERROR;
}

void move_piece(State* gameState) {
    Move move = get_move();
    // THIS IS DODGY CAUSE IT JUST CHECKS TOP LEFT CORNER
    int x = gameState->currentPiece->position.x;

    switch(move) {
        // check for valid move here
        case ROTATE:
            rotate(gameState->currentPiece->matrix, MAT);
        case LEFT:
            if (x - 1 >= 0) {
                gameState->currentPiece->position.x--;
            }
        case RIGHT:
            if (x + 1 < gameState->width) {
                gameState->currentPiece->position.x++;
            }
        case DOWN:
            gameState->currentPiece->position.y++;
        case ERROR:
            ;
    }
}

void game_update(State* gameState) {
    /// clear previous printing
    clear_inactive_cells(gameState);
    move_piece(gameState);
    if (is_piece_active(gameState)) {
        int x, y;
        x = gameState->currentPiece->position.x;
        y = gameState->currentPiece->position.y;

        for (int i = 0; i < MAT; i++) {
            for (int j = 0; j < MAT; j++) {
                if (gameState->currentPiece->matrix[i][j] == ACTIVE) {
                    gameState->field[i + x][j + y] = ACTIVE_PIECE_CELL;
                }
            }
        }

    } else {
        for (int i = 0; i < MAT; i++) {
            for (int j = 0; j < MAT; j++) {
                if (gameState->currentPiece->matrix[i][j] == ACTIVE) {
                    gameState->field[i + gameState->currentPiece->position.x][j + 
                            gameState->currentPiece->position.y - 1] = PIECE_CELL;
                }
            }
        }
        deactivate_piece(gameState);
        // CHANGE THIS TO BE RANDOM
        new_piece(gameState, L);
    }
}