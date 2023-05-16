#ifndef ENTITY_HEADER
#define ENTITY_HEADER

#define ENTITY_NUM ET_END 

typedef enum entity_type {
    ET_NULL=0,
    ET_CARROT,
    ET_RABBIT,
    ET_END,
} entity_type;

typedef struct entity {
    int r, c;
    int delay;
    int col;
    entity_type type;
} entity;

void init_entities();
void draw_entity(entity *entity);
void update_entity(entity *e);
entity get_entity_template(entity_type et);
void free_entity(entity *e);
void update_all_entities();
entity *get_entity_at(int row, int col);
entity *create_entity(entity_type et);

#endif