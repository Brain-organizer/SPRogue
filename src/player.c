#include <stdlib.h>
#include <string.h>
#include "player.h"
#include "entity.h"
#include "floor.h"
#include "action.h"

entity *player;

void init_player() {
    player = malloc(sizeof(entity));
    player->delay = 100;
    *player = get_entity_template(ET_CARROT);
}

//player를 현재 room에다 배치하는 함수. map의 row행 col열에 배치되며, room의 entities[idx]에 player의 entity가 들어가게 된다. 
//room이 변경된 후 반드시 호출해주어야 하는 함수.
void push_player_into_room(int row, int col){
    push_entity_into_room(NULL, player, row, col);
}

void place_player() {
    
}

void update_player() {
    if(player->delay) {
        --(player->delay);
    }
    else {
        
        switch(getch()) {
            case KEY_RIGHT:
                move_player_to(player->r , player->c +1);
                break;
            case KEY_LEFT:
                move_player_to(player->r , player->c -1);
                break;
            case KEY_UP:
                move_player_to(player->r -1, player->c);
                break;
            case KEY_DOWN:
                move_player_to(player->r +1, player->c);
                break;
            case '.':
                player->delay = 100;
                break;
            case 'i': case 'I':
                //인벤토리 창

            case 'p': case 'P':
                //일시정지

            //...
        }
        player->delay = 100;
    }
}

//player entity의 포인터를 반환
entity *get_player(){
    return player;
}

void free_player() {
    free(player);
}