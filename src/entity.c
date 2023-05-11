#include <stdlib.h>
#include "entity.h"

void do_random_movement(entity *e) {

}

entity * get_tmp_entity() {
    entity *e = malloc(sizeof(entity));
    e->delay = 100;
    return e;
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