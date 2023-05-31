#ifndef ROOM_HEADER
#define ROOM_HEADER

#include <stdbool.h>

#include "tiles.h"
#include "entity.h"

#include "cvector.h"

typedef struct door {
    tile *prev;
    tile *next;
    int room_id;
} door;

typedef struct room {
    int r, c;
    int roff, coff;
    tile **map;
    bool **dirty;
    cvector_vector_type(door) doors;
    cvector_vector_type(entity *) entities;
    char *name;
    char *desc;
} room;

room * get_tmp_room();
room * get_butcher_room();
room * get_start_room();
room * get_corridor_room();
room * get_storage_room();

bool is_door_ok(door d);

void draw_room(room *rm);

void free_room(room *rm);

void pop_entity_from_room(room *rm, entity *e);
void push_entity_into_room(room *rm, entity *e, int row, int col, int ind);
void push_player_into_room(int row, int col);

#endif