#include "player.h"
#include "action.h"
#include "entity.h"
#include "floor.h"
#include "tiles.h"
#include "util.h"
#include "automove.h"
#include "color.h"
#include "client.h"
#include "message.h"
#include "sidebar.h"

void enter_door_entity() {
    fputs("Entity cannot to walk into a door!\n", stderr);
}

void enter_door_player(tile *t) {
    entity *player = get_player();
    floor *f = get_cur_floor();
    room *rm = get_cur_room();
    door dr;
    int r, c;

    fputs("Player tried to enter a door\n", stderr);
    
    if(t->door_id == -1) {
        fputs("This tile is not a door!\n", stderr);
        return;
    }

    dr = rm->doors[t->door_id];
    
    pop_entity_from_room(rm, player);

    reset_colors();

    for(r = 0; r < f->cur_room->r; ++r) {
        for(c = 0; c < f->cur_room->c; ++c) {
            f->cur_room->map[r][c].fg = f->cur_room->map[r][c].bg = -1;
            f->cur_room->dirty[r][c] = true;
        }
    }
    for(r = 0; r < cvector_size(f->cur_room->entities); ++r)
        f->cur_room->entities[r]->bg = f->cur_room->entities[r]->fg = -1;
    clear_sidebar();
    
    f->cur_room = f->rooms[dr.room_id];

    push_player_into_room(dr.next->r, dr.next->c);

    f->room_changed = true;

    for(r = 0; r < f->cur_room->r; ++r) {
        for(c = 0; c < f->cur_room->c; ++c) {
            f->cur_room->map[r][c].fg = f->cur_room->map[r][c].bg = -1;
            f->cur_room->dirty[r][c] = true;
        }
    }
    for(r = 0; r < cvector_size(f->cur_room->entities); ++r)
        f->cur_room->entities[r]->bg = f->cur_room->entities[r]->fg = -1;

    fputs("Player successfully moved into a door!\n", stderr);

    add_message("You pass through the %s and enter into the %s", t->name, f->cur_room->name);
} 

void move_entity_to(entity *e, tile *next) {
    room *rm = get_cur_room();
    tile *cur = get_tile_at(e->r, e->c);

    if(cur == NULL) {
        raise("move_entity_to");
    }

    if(next->entity_id != -1) return;   // 이 함수는 오직 움직임 만을 처리

    next->entity_id = cur->entity_id;

    cur->entity_id = -1;

    rm->dirty[e->r][e->c] = true;
    rm->dirty[next->r][next->c] = true;

    e->r = next->r;
    e->c = next->c;

    e->delay = e->mv_de;
    e->tile = next;
}

void handle_entity_enter_tile_event(entity *e, tile *new_tile) {
    entity *target;
    if(!new_tile) return;
    if(is_passable_tile(new_tile)) {
        if(new_tile->entity_id != -1) {
            target = get_entity_at_tile(new_tile);
            if(e->is_enemy != target->is_enemy){ // 공격가능한 대상이 있으면(현재 엔티티와 타겟 엔티티가 적 관계이면) 공격
                fputs("Entity tried to attack a player\n", stderr);
                attack(e, target);
            }
            else {
                e->delay = e->attack_de;
                add_message("%s fumbles around %s...", e->name, target->name);
            }
        }
        else {
            move_entity_to(e, new_tile);
        }
    }
    else if(is_door_tile(new_tile)) {
        enter_door_entity();
    }
}

//player를 현재위치에서 row행 col열로 이동시키는 함수. 해당 위치에 문이나 몬스터가 있으면 알맞은 행동을 취한다. 
void handle_player_enter_tile_event(tile *new_tile){
    entity *player = get_player();
    entity *target = get_entity_at_tile(new_tile);
    int prev_row = player->r;
    int prev_col = player->c;
    tile *prev_tile = get_tile_at(prev_row, prev_col);

    if(!new_tile) return;

    if(is_door_tile(new_tile)) {
        enter_door_player(new_tile);
        return;
    }
    else if(!is_passable_tile(new_tile)){
        add_message("You bump you head at the %s at (%d, %d)", new_tile->name, new_tile->r, new_tile->c);
        return;
    }
    // else if(){// 아이템 있으면 획득

    // }
    else if(target == NULL){ //entity 아무것도 없으면 이동
        move_entity_to(player, new_tile);
        add_message("You move into the %s at (%d, %d)", new_tile->name, new_tile->r, new_tile->c);

        if(new_tile->status & TS_BOMB) {
            new_tile->status ^= TS_BOMB;
            ++player->bombs;
            add_message("You pick up a potato bomb from %s", new_tile->name);
        }
        if(new_tile->status & TS_PEER) {
            new_tile->status ^= TS_PEER;
            ++player->peers;
            add_message("You pick up a dead eggplant from %s", new_tile->name);
        }
    }
    else if(target->is_enemy){ // 공격가능한 대상이 있으면 공격
        attack(player, target);
    }

}

void auto_move(entity* e){
    dir_type dir = get_next_step(e);
    tile *new_tile = get_tile_at(e->r + MOVE_DIRS[dir][0], e->c + MOVE_DIRS[dir][1]);

    handle_entity_enter_tile_event(e, new_tile);
}

//from 엔티티가 to 엔티티를 공격하는 함수. from의 power에 해당하는 값만큼 to의 hp가 줄어든다. 만약 hp가 0이 된다면 to는 죽게된다.
void attack(entity *from, entity *to){
    switch(from->type){
    case ET_CARROT: case ET_RABBIT: case ET_EGGPLANT:
        add_message("%s attack(s) the %s at (%d, %d)", from->name, to->name, to->r, to->c);
        if(to->hp <= from->power){
            to->hp = 0;
            kill_et(to);
        }
        else{
            to->hp -= from->power;
            //todo : to의 hp가 줄어들었다는 메세지 or entity들의 hp 잔량을 옆에 표시해줌
            
        }
        break;
        
    case ET_POTATOBOOM:
        add_message("%s explodes! %s is struck down with shrapnels of flesh and bone.", from->name, to->name);
        
        if(to->hp < from->power){
            from->power -= to->hp;
            kill_et(to);
        }
        else if(to->hp > from->power){
            to->hp -= from->power;
            kill_et(from);
        }
        else{
            kill_et(to);
            kill_et(from);
        }
        break;
    }
    from->delay = from->attack_de;
}

//entity를 죽이는 함수. entity를 map에서 삭제하고, 죽었다는 메세지를 표시해줌.
void kill_et(entity *target){
    add_message("%s at (%d, %d) has died...", target->name, target->r, target->c);

    pop_entity_from_room(NULL, target);
    free(target);

    //todo :죽었다는 메세지 표시해주기.
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

    handle_entity_enter_tile_event(e, nexts[i]);
}

void do_examine() {
    int c;
    room *rm = get_cur_room();
    entity *pl = get_player();

    add_message("You begin to examine your surroundings.");
    draw_message();

    switch((c = getch())) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            c -= '0';
            if(cvector_size(rm->entities) <= c)
                add_message("There is no entity with id \'%d\'", c);
            else
                add_message(rm->entities[c]->desc);
            break;
        case 'r':
            add_message(get_cur_room()->desc);
            break;
        case KEY_RIGHT:
            describe_tile(pl->r, pl->c+1);
            break;
        case KEY_LEFT:
            describe_tile(pl->r, pl->c-1);
            break;
        case KEY_UP:
            describe_tile(pl->r-1, pl->c);
            break;
        case KEY_DOWN:
            describe_tile(pl->r+1, pl->c);
            break;
        case '.':
            describe_tile(pl->r, pl->c);
            break;
        default:
            add_message("You withdraw your gaze from your surroundings.");
            add_message("To examine an entity, type in their id. To examine objects around you, press the arrow key or the letter \'.\'. To examine the room, type in the letter \'r\'");
    }
}