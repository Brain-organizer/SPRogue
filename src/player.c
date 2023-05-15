#include <stdlib.h>
#include <string.h>
#include "player.h"
#include "entity.h"
#include "floor.h"

entity *player;

void init_player() {
    player = malloc(sizeof(entity));
    player->delay = 100;
    *player = get_entity_template(ET_CARROT);
}

//player를 현재 room에다 배치하는 함수. map의 row행 col열에 배치되며, room의 entities[idx]에 player의 entity가 들어가게 된다. 
//room이 변경된 후 반드시 호출해주어야 하는 함수.
void push_player_into_room(int row, int col){
    push_entity_into_room(player, row, col);
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