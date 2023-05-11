#include <stdlib.h>
#include <string.h>
#include "floor.h"

floor * init_floor() {
    floor *f = malloc(sizeof(floor));
    f->cur_room = get_tmp_room();
    f->player = init_player();
    f->rooms = malloc(sizeof(floor *) * 2);
    f->rooms[0] = f->cur_room;
    f->rooms[1] = 0;
    return f;
}
void draw(floor *f) {
    draw_room(f->cur_room);
}
void update(floor *f) {

}

void free_floor(floor *f) {
    f->cur_room = f->rooms[0];
    while(f->cur_room) {
        free_room(f->cur_room);
        ++(f->cur_room);
    }
    free(f->rooms);
}