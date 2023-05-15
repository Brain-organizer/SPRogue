#include "player.h"
#include "action.h"
#include "entity.h"
#include "floor.h"
#include "tiles.h"

//player를 현재위치에서 row행 col열로 이동시키는 함수. 해당 위치에 문이나 몬스터가 있으면 알맞은 행동을 취한다. 
void move_player_to(int row, int col){
    entity *player = get_player();
    entity *target = get_entity_at(row, col);
    int prev_row = player->r;
    int prev_col = player->c;
    tile *prev_tile = get_tile_at(prev_row, prev_col);
    tile *new_tile = get_tile_at(row,col);

    // if(){ // row,col이 문이면 다른 방으로 이동

    // } else if
    if(!is_passable(row,col)){
        return;
    }
    // else if(){// 아이템 있으면 획득

    // }
    else if(target == NULL){ //entity 아무것도 없으면 이동
        new_tile->entity_id = prev_tile->entity_id;
        prev_tile->entity_id = -1;

        player->r = row;
        player->c = col;
    }
    else if(target->type != ET_CARROT){ // 공격가능한 대상이 있으면 공격
        attack(player, target);
    }

}

//to do : from이 to에게 데미지를 입힘. 데미지는 from->power에 정의?
void attack(entity *from, entity *to){
    
}
