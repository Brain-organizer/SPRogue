#ifndef ENTITY_HEADER
#define ENTITY_HEADER

#include "tiles.h"

typedef enum entity_type {
    ET_NULL=0,
    ET_CARROT,
    ET_RABBIT,
    ET_POTATOBOOM,
    ET_EGGPLANT,
    ENTITY_NUM,
} entity_type;

typedef struct entity {
    int r, c;
    int delay;
    int fg, bg, blink;
    int col;
    int hp;
    int power;
    int is_enemy; //적이거나 적 소유의 무언가이면 1, player나 player 소유물이면 0
    int attack_de;
    int mv_de;
    tile *tile;
    entity_type type;
} entity;

void init_entities();
void draw_entity(entity *entity);
void update_entity(entity *e);
entity get_entity_template(entity_type et);
void free_entity(entity *e);
void update_all_entities();
entity *get_entity_at(int row, int col);
entity *get_entity_at_tile(tile *t);
entity *create_entity(entity_type et);

#endif