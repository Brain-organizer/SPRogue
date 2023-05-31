#include "entity.h"

#ifndef PLAYER_HEADER
#define PLAYER_HEADER

#include <ncurses.h>


void init_player();
bool update_player();
void free_player();
void make_potatoboom();
void call_peer();
void throw_leaf();
entity *get_player();

#endif