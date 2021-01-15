#ifndef RENDER_H
#define RENDER_H

#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>

#include "model.h"



void draw_border(State* gameState);
void update_screen(State* gameState);

#endif
