#ifndef PLAYER_HEADER
#define PLAYER_HEADER

#include <ncurses.h>


void init_player();
void update_player();
void free_player();
void push_player_into_room(int row, int col);

#endif