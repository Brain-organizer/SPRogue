#include <stdlib.h>
#include <string.h>
#include "player.h"
#include "entity.h"

entity *player;

void init_player() {
    player = malloc(sizeof(entity));
    player->delay = 100;
}

void place_player() {
    
}

void update_player() {
    if(player->delay) {
        --(player->delay);
    }
    else {
        getch();
        player->delay = 100;
    }
}

void free_player() {
    free(player);
}