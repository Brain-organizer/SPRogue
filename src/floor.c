#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "floor.h"
#include "util.h"

floor *cur_floor;

void init_floor() {
    cur_floor = malloc(sizeof(floor));
    //cur_floor->cur_room = get_tmp_room();
    //cur_floor->cur_room = get_butcher_room();
    cur_floor->cur_room = get_start_room();
    cur_floor->rooms = NULL;
    cvector_push_back(cur_floor->rooms, cur_floor->cur_room);
    
    //player를 room에 집어넣는다. 
    push_player_into_room(cur_floor->cur_room->r/2, cur_floor->cur_room->c/2);
}
void draw() {
    draw_room(cur_floor->cur_room);
    refresh();
}
void update(floor *f) {

}

//현재 방의 포인터를 반환
room *get_cur_room(){
    return cur_floor->cur_room;
}

void free_floor() {
    floor *f = cur_floor;
    int i;
    for(i = 0; i < cvector_size(f->rooms); ++i) free_room(f->rooms[i]);
    cvector_free(f->rooms);
}