#ifndef FLOOR_HEADER
#define FLOOR_HEADER

#include "rooms.h"
#include "player.h"

typedef struct floor {
    room **rooms;
    room *cur_room;
    player *player;
} floor;

floor * init_floor();
void draw(floor *f);
void update(floor *f);
void free_floor(floor *f);

#endif