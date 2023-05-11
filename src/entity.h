#ifndef ENTITY_HEADER
#define ENTITY_HEADER

typedef struct entity {
    int delay;
} entity;

entity * get_tmp_entity();
void update_entity(entity *e);
void free_entity(entity *e);

#endif