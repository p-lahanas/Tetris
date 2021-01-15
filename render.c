#include "render.h"

#define BORDER_CHAR "|"
#define BOTTOM_CHAR "#"
#define BLANK " "
#define BLOCK "X"

void draw_border(State* gameState) {
    for (int j = 0; j < gameState->height + 1; j++) {
        for (int i = 0; i < gameState->width + 2; i++) {
            if (j == gameState->height) {
                mvprintw(j, i, BOTTOM_CHAR);
            } else if (i == 0 || i == gameState->width + 1) {
                mvprintw(j, i, BORDER_CHAR);
            } 
        }
    }
}

void update_screen(State* gameState) {
    for (int j = 0; j < gameState->height; j++) {
        // start 1 from left to account for border
        for (int i = 0; i < gameState->width; i++) {
            char element = gameState->field[i][j];
            // print 1 extra from left to account for border 
            if (element == FREE_CELL) {
                mvprintw(j, i + 1, BLANK);
            } else {
                mvprintw(j, i + 1, BLOCK);
            }
        }
    }
}