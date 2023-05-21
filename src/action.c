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
    else if(target->is_enemy){ // 공격가능한 대상이 있으면 공격
        attack(player, target);
    }

}

//from 엔티티가 to 엔티티를 공격하는 함수. from의 power에 해당하는 값만큼 to의 hp가 줄어든다. 만약 hp가 0이 된다면 to는 죽게된다.
void attack(entity *from, entity *to){
    switch(from->type){
    case ET_CARROT: case ET_RABBIT:
        if(to->hp <= from->power){
            to->hp = 0;
            kill_et(to);
        }
        else{
            to->hp -= from->power;
            //todo : to의 hp가 줄어들었다는 메세지 or entity들의 hp 잔량을 옆에 표시해줌
            
        }
        
    //todo : 투사체의 공격은 다른 케이스. 
    }
}

//entity를 죽이는 함수. entity를 map에서 삭제하고, 죽었다는 메세지를 표시해줌.
void kill_et(entity *target){
    remove_entity(target);

    //todo :죽었다는 메세지 표시해주기.
}
