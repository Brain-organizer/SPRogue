#ifndef FLOOR_HEADER
#define FLOOR_HEADER

#include <stdbool.h>

#include "rooms.h"
#include "player.h"
#include "cvector.h"

typedef struct floor {
    bool room_changed;
    cvector_vector_type(room *) rooms;
    room *cur_room;
} floor;

void init_floor();
void draw();
void update(floor *f);
void free_floor();
room *get_cur_room();
floor *get_cur_floor();

void link_rooms(room *a, room *b, int da, int db);

#endif