#ifndef FLOOR_HEADER
#define FLOOR_HEADER

#include "rooms.h"
#include "player.h"
#include "cvector.h"

typedef struct floor {
    cvector_vector_type(room *) rooms;
    room *cur_room;
} floor;

void init_floor();
void draw();
void update(floor *f);
void free_floor();
room *get_cur_room();

#endif