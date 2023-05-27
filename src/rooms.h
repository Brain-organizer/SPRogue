#ifndef ROOM_HEADER
#define ROOM_HEADER

#include <stdbool.h>

#include "tiles.h"
#include "entity.h"

#include "cvector.h"

typedef enum door_dir {
    DD_NORTH,
    DD_SOUTH,
    DD_WEST,
    DD_EAST,
    DOOR_DIR_NUM,
} door_dir;

const static door_dir door_opp[DOOR_DIR_NUM] = { DD_SOUTH, DD_NORTH, DD_EAST, DD_WEST };

typedef struct door {
    tile *prev;
    tile *next;
    int room_id;
} door;

typedef struct room {
    int r, c;
    tile **map;
    bool **dirty;
    door doors[DOOR_DIR_NUM];
    cvector_vector_type(entity *) entities;
} room;

room * get_tmp_room();
room * get_butcher_room();
room * get_start_room();
room * get_corridor_room();

bool is_door_ok(door d);

void draw_room(room *rm);

void free_room(room *rm);

door_dir get_door_dir_opp(door_dir dir);

void pop_entity_from_room(room *rm, entity *e);
void push_entity_into_room(room *rm, entity *e, int row, int col);
void push_player_into_room(int row, int col);

#endif