#include <stdlib.h>
#include <stdbool.h>
#include "entity.h"
#include "color.h"
#include "floor.h"
#include "tiles.h"
#include "util.h"
#include "rooms.h"
#include "action.h"

entity entity_template[ENTITY_NUM];
void (*draw_entity_func[ENTITY_NUM])(entity *);

void set_entity_template(entity_type et) {
    entity_template[et].delay = 100;
    entity_template[et].fg = -1;
    entity_template[et].bg = -1;
    entity_template[et].blink = 0;
}

void set_entity_template_ET_NULL_func() {
    entity_template[ET_NULL].type = ET_NULL;
}
void draw_entity_func_ET_NULL(entity *entity) {
    return;
}

void set_entity_template_ET_CARROT_func() {
    entity_template[ET_CARROT].delay = 5000;
    entity_template[ET_CARROT].hp = 200;
    entity_template[ET_CARROT].power = 20;
    entity_template[ET_CARROT].is_enemy = 0;
    entity_template[ET_CARROT].type = ET_CARROT;
    entity_template[ET_CARROT].attack_de = 7000;
    entity_template[ET_CARROT].mv_de = 5000;
    entity_template[ET_CARROT].name = "You";
    entity_template[ET_CARROT].desc = "This is you. You are a carrot born not from the wombs of any mother, but by the wishes of the oppressed of this farm. They wished for vengeance, and paid with their blood. Now you are here to do their bidding.";
}
void draw_entity_func_ET_CARROT(entity *entity) {
    if(entity->fg > 0) unget_color_id(entity->fg);
    
    entity->fg = get_color_id(139, 0, 0);
    entity->bg = entity->tile->bg;
    entity->col = PAIR_COLOR(entity->fg, entity->bg);
    entity->icon = "Y";

    SET_COLOR(entity->col);
    mvaddstr(entity->r, entity->c, entity->icon);
    UNSET_COLOR(entity->col);
}

void set_entity_template_ET_RABBIT_func() {
    entity_template[ET_RABBIT].delay = 7000;
    entity_template[ET_RABBIT].hp = 100;
    entity_template[ET_RABBIT].power = 20;
    entity_template[ET_RABBIT].is_enemy = 1;
    entity_template[ET_RABBIT].type = ET_RABBIT;
    entity_template[ET_RABBIT].attack_de = 10000;
    entity_template[ET_RABBIT].mv_de = 7000;
    entity_template[ET_RABBIT].name = "Rabbit";
    entity_template[ET_RABBIT].desc = "You are looking at a rabbit, a race of nomadic warriors who have invaded the village. Before their arrival, this place was called the Carrot-carrot village. Now, they call it the Bunny-bunny farm.";
}
void draw_entity_func_ET_RABBIT(entity *entity) {
    if(entity->fg > 0) unget_color_id(entity->fg);
    
    entity->fg = get_color_id(255, 255, 255);
    entity->bg = entity->tile->bg;
    entity->col = PAIR_COLOR(entity->fg, entity->bg);
    entity->icon = "R";

    SET_COLOR(entity->col);
    mvaddstr(entity->r, entity->c, entity->icon);
    UNSET_COLOR(entity->col);
}

void set_entity_template_ET_POTATOBOOM_func() {
    entity_template[ET_POTATOBOOM].delay = 5000;
    entity_template[ET_POTATOBOOM].hp = 50;
    entity_template[ET_POTATOBOOM].power = 100;
    entity_template[ET_POTATOBOOM].is_enemy = 0;
    entity_template[ET_POTATOBOOM].type = ET_POTATOBOOM;
    entity_template[ET_POTATOBOOM].attack_de = 7000;
    entity_template[ET_POTATOBOOM].mv_de = 5000;
    entity_template[ET_POTATOBOOM].name = "Potato Bomb";
    entity_template[ET_POTATOBOOM].desc = "This is a bomb, made from dead potato. Once a living creature of joy and vigor, it now seeks to spread death through explosion.";
}
void draw_entity_func_ET_POTATOBOOM(entity * entity) {
    if(entity->fg > 0) unget_color_id(entity->fg);
    
    entity->fg = get_color_id(139, 0, 0);
    entity->bg = entity->tile->bg;
    entity->col = PAIR_COLOR(entity->fg, entity->bg);
    entity->icon = "o";
    
    SET_COLOR(entity->col);
    mvaddstr(entity->r, entity->c, entity->icon);
    UNSET_COLOR(entity->col);
}

void set_entity_template_ET_EGGPLANT_func() {
    entity_template[ET_EGGPLANT].delay = 5000;
    entity_template[ET_EGGPLANT].hp = 100;
    entity_template[ET_EGGPLANT].power = 10;
    entity_template[ET_EGGPLANT].is_enemy = 0;
    entity_template[ET_EGGPLANT].type = ET_EGGPLANT;
    entity_template[ET_EGGPLANT].attack_de = 7000;
    entity_template[ET_EGGPLANT].mv_de = 5000;
    entity_template[ET_EGGPLANT].name = "Zombie Eggplant";
    entity_template[ET_EGGPLANT].desc = "A dead eggplant that has come back to life. A mindless creature, it reacts only to the scent of rabbit fur, which it seeks to chew down with vicious hate.";
}
void draw_entity_func_ET_EGGPLANT(entity * entity) {
    if(entity->fg > 0) unget_color_id(entity->fg);
    
    entity->fg = get_color_id(255, 255, 255);
    entity->bg = entity->tile->bg;
    entity->col = PAIR_COLOR(entity->fg, entity->bg);
    entity->icon = "Y";
    
    SET_COLOR(entity->col);
    mvaddstr(entity->r, entity->c, entity->icon);
    UNSET_COLOR(entity->col);
}

#define INIT_ENTITY_MACRO(NAME) set_entity_template(NAME); set_entity_template_ ## NAME ## _func(); draw_entity_func[NAME] = draw_entity_func_ ## NAME;

void init_entities() {
    INIT_ENTITY_MACRO(ET_NULL)
    INIT_ENTITY_MACRO(ET_CARROT)
    INIT_ENTITY_MACRO(ET_RABBIT)
    INIT_ENTITY_MACRO(ET_POTATOBOOM)
    INIT_ENTITY_MACRO(ET_EGGPLANT)
}

void draw_entity(entity *entity) {
    draw_entity_func[entity->type](entity);
}

entity get_entity_template(entity_type et) {
    return entity_template[et];
}

void update_all_entities(){ //player(당근)을 제외한 모든 entity에 대해 딜레이를 확인하고, 딜레이가 0이면 random movement를 진행.

    room *cur_room = get_cur_room();
    entity *target_ent;

    for(int row = 0 ; row <  cur_room->r ; row++){
        for(int col = 0; col < cur_room->c ; col++){
            if(cur_room->map[row][col].entity_id == -1)
                continue;

            target_ent = get_entity_at(row,col);
            if(target_ent->type != ET_CARROT){
                update_entity(target_ent);
            }
        }
    }
}

//현재 방의 row행, col열에 있는 entity의 포인터를 반환하는 함수. entity가 없을시 null 반환.
entity *get_entity_at(int row, int col){
    room *cur_room = get_cur_room();
    if(is_within_bound(row, col) && cur_room->map[row][col].entity_id>=0)
        return cur_room->entities[cur_room->map[row][col].entity_id];
    else
        return NULL;
}
entity *get_entity_at_tile(tile *t) {
    if(t)
        return get_entity_at(t->r, t->c);
    else
        return NULL;
}

void update_entity(entity *e) {
    if(e->delay) {
        --(e->delay);
    }
    else {
        auto_move(e);
    }
}
void free_entity(entity *e) {
    free(e);
}

entity *create_entity(entity_type et) {
    entity *out = malloc(sizeof(entity));
    *out = get_entity_template(et);
    return out;
}