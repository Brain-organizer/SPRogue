#include <stdlib.h>
#include "entity.h"
#include "color.h"

entity entity_template[ENTITY_NUM];
void (*draw_entity_func[ENTITY_NUM])(entity *);

void set_entity_template(entity_type et) {
    entity_template[et].delay = 100;
}

void set_entity_template_ET_NULL_func() {
    entity_template[ET_NULL].type = ET_NULL;
}
void draw_entity_func_ET_NULL(entity *entity) {
    return;
}

void set_entity_template_ET_CARROT_func() {
    entity_template[ET_CARROT].type = ET_CARROT;
}
void draw_entity_func_ET_CARROT(entity *entity) {
    entity->col = colornum(2, 0, false, false);
    setcolor(entity->col);
    mvaddstr(entity->r, entity->c, "Y");

    unsetcolor(entity->col);
}

#define INIT_ENTITY_MACRO(NAME) set_entity_template(NAME); set_entity_template_ ## NAME ## _func(); draw_entity_func[NAME] = draw_entity_func_ ## NAME;

void init_entities() {
    INIT_ENTITY_MACRO(ET_NULL)
    INIT_ENTITY_MACRO(ET_CARROT)
}

void draw_entity(entity *entity) {
    draw_entity_func[entity->type](entity);
}

entity get_entity_template(entity_type et) {
    return entity_template[et];
}

void do_random_movement(entity *e) {

}

void update_entity(entity *e) {
    if(e->delay) {
        --(e->delay);
    }
    else {
        do_random_movement(e);
        e->delay = 100;
    }
}
void free_entity(entity *e) {
    free(e);
}