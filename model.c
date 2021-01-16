#include "model.h"

#define NUM_BLOCKS 7

char J[4][4] = 
{
    {' ', '1', ' ', ' '},
    {' ', '1', ' ', ' '},
    {' ', '1', '1', ' '},
    {' ', ' ', ' ', ' '}
};

char O[4][4] = 
{
    {' ', ' ', ' ', ' '},
    {' ', '1', '1', ' '},
    {' ', '1', '1', ' '},
    {' ', ' ', ' ', ' '}
};

char L[4][4] = 
{
    {' ', '1', ' ', ' '},
    {' ', '1', ' ', ' '},
    {'1', '1', ' ', ' '},
    {' ', ' ', ' ', ' '}
};

char I[4][4] = 
{
    {' ', '1', ' ', ' '},
    {' ', '1', ' ', ' '},
    {' ', '1', ' ', ' '},
    {' ', '1', ' ', ' '}
};

char Z[4][4] = 
{
    {' ', ' ', ' ', ' '},
    {' ', '1', '1', ' '},
    {'1', '1', ' ', ' '},
    {' ', ' ', ' ', ' '}
};

char S[4][4] = 
{
    {' ', ' ', ' ', ' '},
    {'1', '1', ' ', ' '},
    {' ', '1', '1', ' '},
    {' ', ' ', ' ', ' '}
};

char T[4][4] = 
{
    {' ', '1', ' ', ' '},
    {'1', '1', ' ', ' '},
    {' ', '1', ' ', ' '},
    {' ', ' ', ' ', ' '}
};


char** add_piece_types() {
    char *Lp, *Op, *Jp, *Ip, *Sp, *Tp, *Zp;
    Lp = &L[0][0];
    Op = &O[0][0];
    Jp = &J[0][0];
    Ip = &I[0][0];
    Sp = &S[0][0];
    Tp = &T[0][0];
    Zp = &Z[0][0];

    char** pieces = (char**) calloc(NUM_BLOCKS, sizeof(char*));
    pieces[0] = Lp;
    pieces[1] = Op;
    pieces[2] = Jp;
    pieces[3] = Ip;
    pieces[4] = Sp;
    pieces[5] = Tp;
    pieces[6] = Zp;
    
    return pieces;
}

/* Creates a new piece and add it to gameState*/
Piece* new_piece(State* gameState) {
    Piece* newPiece = (Piece*) calloc(1, sizeof(Piece));
    int k = rand() % 7;
    char* pieceType;
    pieceType = gameState->pieceTypes[k];
    
    for (int i = 0; i < MAT; i++) {
        for (int j = 0; j < MAT; j++) {
            // calculate the contiguous assigned array to the dyanimically allocated array
            newPiece->matrix[i][j] = pieceType[(i * MAT + j) * sizeof(char)];
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
    newGame->pieceTypes = add_piece_types();
    newGame->width = WIDTH;
    newGame->height = HEIGHT;
    newGame->currentPiece = new_piece(newGame);

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
void check_full_rows(State* gameState) {
    for (int j = 0; j < gameState->height; j++) {
        bool fullRow = true;
        for (int i = 0; i < gameState->width; i++) {
            if (gameState->field[i][j] != PIECE_CELL) {
                fullRow = false;
                break;
            }
        }
        if (fullRow) {
            // clear the row
            Coordinate pos;
            pos.y = j;
            for (int k = 0; k < gameState->width; k++) {
                pos.x = k;
                set_cell_value(pos, gameState, FREE_CELL);
                // move all above pieces down
                for (int y = j; y > 0; y--) {
                    gameState->field[k][y] = gameState->field[k][y - 1];
                }
                
            }

            // increment the score
            gameState->score++;
        }
    }

}


bool is_valid_move(State* gameState, Coordinate newPos) {
    Piece* piece = gameState->currentPiece;
    for (int i = 0; i < MAT; i++) {
        for (int j = 0; j < MAT; j++) {
            // check for a boundary
            if ((piece->matrix[i][j] == ACTIVE && newPos.x + i >= gameState->width) ||
                    (piece->matrix[i][j] == ACTIVE && newPos.x + i < 0)) {
                return false;
            } else if (piece->matrix[i][j] == ACTIVE && 
                    gameState->field[i + newPos.x][j + newPos.y] == PIECE_CELL) {
                return false;
            } else if (piece->matrix[i][j] == ACTIVE && newPos.y + j >= gameState->height) {
                return false;
            }
        }
    }
    return true;
}

void move_piece(State* gameState) {
    Move move = get_move();
    // THIS IS DODGY CAUSE IT JUST CHECKS TOP LEFT CORNER
    Coordinate pos;
    pos.x = gameState->currentPiece->position.x;
    pos.y = gameState->currentPiece->position.y;
    switch(move) {
        // check for valid move here
        case ROTATE:
            rotate(gameState->currentPiece->matrix, MAT);
            if (is_valid_move(gameState, pos)) {
                break;
            } else {
                // rotate it back to original state
                rotate(gameState->currentPiece->matrix, MAT);
                rotate(gameState->currentPiece->matrix, MAT);
                rotate(gameState->currentPiece->matrix, MAT);
            }
            break;
        case RIGHT:
            pos.x++;
            if (is_valid_move(gameState, pos)) {
                gameState->currentPiece->position.x++;
                break;
            }
            break;
        case LEFT:
            pos.x--;
            if (is_valid_move(gameState, pos)) {
                gameState->currentPiece->position.x--;
                break;
            }
            break;
        case DOWN:
            gameState->currentPiece->position.y++;
            break;
        case ERROR:
            break;
    }
}

void game_update(State* gameState) {
    /// clear previous printing
    check_full_rows(gameState);
    clear_inactive_cells(gameState);
   
    Coordinate pos;
    pos.x = gameState->currentPiece->position.x;
    pos.y = gameState->currentPiece->position.y;

    move_piece(gameState);
    if (is_valid_move(gameState, pos)) {
        for (int i = 0; i < MAT; i++) {
            for (int j = 0; j < MAT; j++) {
                if (gameState->currentPiece->matrix[i][j] == ACTIVE) {
                    gameState->field[i + pos.x][j + pos.y] = ACTIVE_PIECE_CELL;
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
        new_piece(gameState);
    }
}