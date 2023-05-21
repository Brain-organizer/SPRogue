#include <stdlib.h>
#include <string.h>
#include "player.h"
#include "entity.h"
#include "floor.h"
#include "action.h"

entity *player;

void init_player() {
    player = create_entity(ET_CARROT);
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
                handle_player_enter_tile_event(get_tile_at(player->r, player->c+1));
                break;
            case KEY_LEFT:
                handle_player_enter_tile_event(get_tile_at(player->r, player->c-1));
                break;
            case KEY_UP:
                handle_player_enter_tile_event(get_tile_at(player->r-1, player->c));
                break;
            case KEY_DOWN:
                handle_player_enter_tile_event(get_tile_at(player->r+1, player->c));
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

    fputs("Player update successful!\n", stderr);
}

//player entity의 포인터를 반환
entity *get_player(){
    return player;
}

void free_player() {
    free(player);
}