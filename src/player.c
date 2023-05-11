#include <stdlib.h>
#include <string.h>
#include "player.h"

player * init_player() {
    player *p = malloc(sizeof(player));
    p->delay = 100;
    return p;
}

void update_player(player *p) {
    if(p->delay) {
        --(p->delay);
    }
    else {
        getch();
        p->delay = 100;
    }
}

void free_player(player *p) {
    free(p);
}