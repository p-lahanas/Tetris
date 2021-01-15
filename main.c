#include "main.h"

#define DELAY 100000

int main(int argc, char** argv) {
    WINDOW* w = initscr();
    noecho();
    curs_set(FALSE);
    cbreak();
    nodelay(w, TRUE);

    State* gameState = game_start(WIDTH, HEIGHT);
    draw_border(gameState);
    clock_t start;
    start = clock();
    while (1){
        if ((clock() - start < DELAY)) {
            game_update(gameState);
            update_screen(gameState);
            refresh();
        } else {
            // move piece down
            gameState->currentPiece->position.y++;
            start = clock();
        }
    }
    endwin();
}