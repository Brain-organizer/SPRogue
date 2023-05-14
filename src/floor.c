#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "floor.h"

floor *cur_floor;

void init_floor() {
    floor *f = malloc(sizeof(floor));
    f->cur_room = get_tmp_room();
    f->rooms = malloc(sizeof(floor *) * 2);
    f->rooms[0] = f->cur_room;
    f->rooms[1] = 0;
    
    cur_floor = f;
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