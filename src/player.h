#ifndef PLAYER_HEADER
#define PLAYER_HEADER

#include <ncurses.h>

typedef struct player {
    int delay;
} player;

player * init_player();
void update_player(player *p);
void free_player(player *p);

#endif