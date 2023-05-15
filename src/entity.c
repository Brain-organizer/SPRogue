#include <stdlib.h>
#include <stdbool.h>
#include "entity.h"
#include "color.h"
#include "floor.h"
#include "tiles.h"
#include "util.h"

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
    tile *nexts[MOVE_TYPES];
    int i;
    bool check = true;      // 움직일 곳이 있는지 확인

    for(i = 0; i < MOVE_TYPES; ++i) {
        nexts[i] = get_tile_at(e->r+MOVE_DIRS[i][0], e->c+MOVE_DIRS[i][1]);

        if(nexts[i] && !(nexts[i]->flags & TF_PASSABLE)) nexts[i] = NULL;

        if(nexts[i]) check = false;
    }

    if(check) return;

    do {
        i = rand() % MOVE_TYPES;
    } while(nexts[i] == NULL);

    move_entity_to(e, nexts[i]);
}

void update_all_entities(){ //player(당근)을 제외한 모든 entity에 대해 딜레이를 확인하고, 딜레이가 0이면 random movement를 진행.

    room *cur_room = get_cur_room();
    entity *target_ent;

    for(int row = 0 ; row <  cur_room->r ; row++){
        for(int col = 0; col < cur_room->c ; col++){
            if(cur_room->map[row][col].entity_id == -1)
                continue;

            target_ent = get_entity_at(row,col);
            // 일단 움직임 테스트하기 위해 코멘트함
            //if(target_ent->type != ET_CARROT){
                update_entity(target_ent);
            //}
        }
    }
}

//현재 방의 row행, col열에 있는 entity의 포인터를 반환하는 함수
entity *get_entity_at(int row, int col){
    room *cur_room = get_cur_room();
    return cur_room->entities[cur_room->map[row][col].entity_id];
}

void update_entity(entity *e) {
    if(e->delay) {
        --(e->delay);
    }
    else {
        do_random_movement(e);
        e->delay = 10000;
    }
}
void free_entity(entity *e) {
    free(e);
}