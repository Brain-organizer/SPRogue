#include "automove.h"
#include "tiles.h"
#include "floor.h"

//stepmap : 특정 row,col에서 가장 가까운 적까지 몇 스텝 더 가야하는지가 기록되어 있다. stepmap[0]에는 가장 가까운 아군(player팀)까지의 거리, stepmap[1]에는 가장 가까운 적(몬스터)까지의 거리가 기록됨.
int stepmaps[2][MAX_MAP_ROW][MAX_MAP_COL];
int row, col;

//상대편에게 최소거리로 닿기 위해서 내딛어야 하는 다음 스텝의 방향이 어딘지를 반환
dir_type get_next_step(entity *e){
    dir_type dir;
    int min_step, r, c;


    min_step = update_stepmap(!(e->is_enemy)); //상대편 stepmap을 update

    dir = 0;
     
    for(dir_type temp = DOWN; temp<MOVE_TYPES; temp++){
        r = e->r + MOVE_DIRS[temp][0];
        c = e->c + MOVE_DIRS[temp][1];
        
        if(is_passable(r,c) && stepmaps[!(e->is_enemy)][r][c] <= min_step){
            min_step = stepmaps[!(e->is_enemy)][r][c];
            dir = temp;
        }
    }

    return dir;
}

// stepmap update 후 최대값을 반환.
int update_stepmap(int enemy){
    int inf, i, j;
    entity *e;
    room *rm = get_cur_room();
    
    row = rm->r ; col = rm->c;
    inf = row * col;

    for(i=0; i<row; i++)
        for(j=0; j<col; j++)
            stepmaps[enemy][i][j] = inf;

    for(i=0; i<row; i++){
        for(j=0; j<col; j++){
            e = get_entity_at(i,j);
            if(e && e->is_enemy == enemy)
                update_stepmap_of(enemy, e->r, e->c, 0);
        }
    }

    return inf;
}

void update_stepmap_of(int enemy, int r, int c, int step){
    entity *e;
    if(!is_passable(r,c) || stepmaps[enemy][r][c]<=step || ((e = get_entity_at(r,c)) && e->is_enemy != enemy))
        return;

    stepmaps[enemy][r][c] = step;

    for(dir_type temp = DOWN; temp<MOVE_TYPES; temp++){
        update_stepmap_of(enemy, r + MOVE_DIRS[temp][0], c + MOVE_DIRS[temp][1], step+1);
    }
}



