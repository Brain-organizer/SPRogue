#ifndef ROOM_HEADER
#define ROOM_HEADER

#include "tiles.h"
#include "entity.h"

#include "cvector.h"

typedef struct room {
    int r, c;
    tile **map;
    cvector_vector_type(entity *) entities;
} room;

room * get_tmp_room();

void draw_room(room *rm);

void free_room(room *rm);

#endif