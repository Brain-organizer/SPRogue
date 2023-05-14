#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "floor.h"

floor *cur_floor;

void init_floor() {
    cur_floor = malloc(sizeof(floor));
    cur_floor->cur_room = get_tmp_room();
    cur_floor->rooms = malloc(sizeof(floor *) * 2);
    cur_floor->rooms[0] = cur_floor->cur_room;
    cur_floor->rooms[1] = 0;
    
    //player를 room에 집어넣는다. 
    push_player_into_room(3,4,0);
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
    f->cur_room = f->rooms[0];
    while(f->cur_room) {
        free_room(f->cur_room);
        ++(f->cur_room);
    }
    free(f->rooms);
}