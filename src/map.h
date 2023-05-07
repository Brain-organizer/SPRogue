#include "tiles.h"

#ifndef MAP_HEADER

#define MAP_HEADER

typedef struct room {
    int r, c;
    tile **map;
} room;

room * get_tmp_room();

void draw_room(room *rm);

void free_room(room *rm);

#endif