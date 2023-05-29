#include "entity.h"

void enter_door_player(tile *t);
void move_entity_to(entity *e, tile *next);
void handle_entity_enter_tile_event(entity *e, tile *t);
void handle_player_enter_tile_event(tile *t);
void attack(entity *from, entity *to);
void kill_et(entity *target);
void auto_move(entity* e);
void do_random_movement(entity *e);
void do_examine();