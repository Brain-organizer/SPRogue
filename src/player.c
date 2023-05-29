#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "player.h"
#include "entity.h"
#include "floor.h"
#include "action.h"
#include "client.h"
#include "color.h"

char msg[200];

entity *player;

void init_player() {
    player = create_entity(ET_CARROT);
}

void place_player() {
    
}

bool update_player() {
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
            case 'q':
                make_potatoboom();
                break;
            case 'w':
                call_peer();
                break;
            case 'i': case 'I':
                //인벤토리 창

            case 'p': case 'P':
                //종료(테스트용)
                struct rank_info rinfo = post_rank(get_tick());
                if(rinfo.prec_playtime==0){
                    fprintf(stderr,"server disconected\n");
                }
                sprintf(msg, "당신의 순위는 %d위(%d틱만에 클리어), 바로 앞 순위의 플레이어는 %d틱만에 클리어했어요!", rinfo.rank, get_tick(), rinfo.prec_playtime);
                close_connect();
                SET_COLOR(0);
                mvaddstr(0,1,msg);
                refresh();
                sleep(5);
                endwin();
                exit(0);
                break;

            //...
            default:
                return false;
        }
    }
    return true;
}

//player entity의 포인터를 반환
entity *get_player(){
    return player;
}

void free_player() {
    free(player);
}

void make_potatoboom(){
    int row, col;
    switch(getch()) {
    case KEY_RIGHT:
        row = player->r; col = player->c+1;
        break;
    case KEY_LEFT:
        row = player->r; col = player->c-1;
        break;
    case KEY_UP:
        row = player->r-1; col = player->c;
        break;
    case KEY_DOWN:
        row = player->r+1; col = player->c;
        break;
    default:
        make_potatoboom();
        return;
    }

    if(is_passable(row,col) && get_entity_at(row,col) == NULL){
        push_entity_into_room(NULL, create_entity(ET_POTATOBOOM), row, col);
        sprintf(msg, "player make_potatoboom at row:%d, col:%d", row, col);
        post_log(msg);
    }
    else
        make_potatoboom();

    player->delay = 15000;
}

void call_peer(){
    int row, col;
    entity *e;
    switch(getch()) {
    case KEY_RIGHT:
        row = player->r; col = player->c+1;
        break;
    case KEY_LEFT:
        row = player->r; col = player->c-1;
        break;
    case KEY_UP:
        row = player->r-1; col = player->c;
        break;
    case KEY_DOWN:
        row = player->r+1; col = player->c;
        break;
    default:
        call_peer();
        return;
    }

    if(is_passable(row,col) && get_entity_at(row,col) == NULL){
        e = create_entity(ET_EGGPLANT);
        e->power = player->power / 2;
        e->hp = player->hp / 2;
        e->delay = player->delay;
        e->attack_de = player->attack_de;
        e->mv_de = player->mv_de;
        push_entity_into_room(NULL, e, row, col);

        sprintf(msg, "player call peer at row:%d, col:%d", row, col);
        post_log(msg);
    }
    else
        call_peer();

    player->delay = 5000;
}