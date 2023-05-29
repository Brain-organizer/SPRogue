#include <ncurses.h>
#include <stdlib.h>
#include "floor.h"
#include "entity.h"
#include "player.h"
#include "util.h"
#include "sidebar.h"

#define ASSIGN_TILE_MACRO(type, row, col) rm->map[row][col] = get_tile_template(type); rm->map[row][col].r = row; rm->map[row][col].c = col
#define ASSIGN_DOOR_MACRO(type, dir, row, col) ASSIGN_TILE_MACRO(type, row, col); rm->map[row][col].door_id = dir; rm->doors[dir].prev = rm->map[row] + col; rm->doors[dir].next = NULL; rm->doors[dir].room_id = -1
#define ASSIGN_STAT_MACRO(flg, row, col) rm->map[row][col].status |= flg

bool is_door_ok(door d) {
    return d.prev && !d.next;
}

room * get_tmp_room() {
    room * rm;
    int r, c;
    rm = malloc(sizeof(room));
    memset(rm, 0, sizeof(rm));

    rm->r = 7;
    rm->c = 12;
    
    rm->map = malloc(sizeof(tile *) * rm->r);

    for(r = 0; r < rm->r; ++r) {
        rm->map[r] = malloc(sizeof(tile) * rm->c);
    }

    rm->dirty = malloc(sizeof(bool *) * rm->r);

    for(r = 0; r < rm->r; ++r) {
        rm->dirty[r] = malloc(sizeof(bool) * rm->c);
        memset(rm->dirty[r], true, sizeof(bool) * rm->c);
    }

    for(r = 0; r < rm->r; ++r) {
        for(c = 0; c < rm->c; ++c) {
            rm->map[r][c] = get_tile_template(TT_DARK);
            rm->map[r][c].r = r;
            rm->map[r][c].c = c;
        }
    }
    
    r = 1;
    for(c = 1; c < 11; ++c) {
        rm->map[r][c] = get_tile_template(TT_WOOD_WALL_HOR);
        rm->map[r][c].r = r;
            rm->map[r][c].c = c;
    }
    r = 5;
    for(c = 1; c < 11; ++c) {
        rm->map[r][c] = get_tile_template(TT_CAVE_WALL);
        rm->map[r][c].r = r;
        rm->map[r][c].c = c;
    }

    c = 1;
    for(r = 2; r < 5; ++r) {
        rm->map[r][c] = get_tile_template(TT_WOOD_WALL_VER);
        rm->map[r][c].r = r;
        rm->map[r][c].c = c;
    }
    c = 10;
    for(r = 2; r < 5; ++r) {
        rm->map[r][c] = get_tile_template(TT_CAVE_WALL);
        rm->map[r][c].r = r;
        rm->map[r][c].c = c;
    }

    for(r = 2; r < 4; ++r) {
        for(c = 2; c < 10; ++c) {
            rm->map[r][c] = get_tile_template(TT_CAVE_FLOOR);
            rm->map[r][c].r = r;
            rm->map[r][c].c = c;
        }
    }

    for(r = 4; r < 5; ++r) {
        for(c = 2; c < 10; ++c) {
            rm->map[r][c] = get_tile_template(TT_GRASS_FLOOR);
            rm->map[r][c].r = r;
            rm->map[r][c].c = c;
        }
    }

    rm->map[1][5].status |= TS_FIRE;
    rm->map[5][5].status |= TS_FIRE;
    rm->map[3][1].status |= TS_FIRE;
    rm->map[3][10].status |= TS_FIRE;

    rm->map[2][5].status |= TS_FIRE;
    rm->map[4][5].status |= TS_FIRE;

    rm->entities = NULL;

    push_entity_into_room(rm, create_entity(ET_RABBIT), 4, 4);

    return rm;
}

room * get_butcher_room() {
    room * rm;
    int r, c;
    rm = malloc(sizeof(room));
    memset(rm, 0, sizeof(rm));

    rm->name = "Butcher's Room";
    rm->desc = "This is the butcher's room, a place where the carrots are turned into sustenance for the rabbit lords. You can smell the blood of the innocents from its red-stained floors.";

    rm->r = 9;
    rm->c = 6;
    
    rm->map = malloc(sizeof(tile *) * rm->r);

    for(r = 0; r < rm->r; ++r) {
        rm->map[r] = malloc(sizeof(tile) * rm->c);
    } 

    rm->dirty = malloc(sizeof(bool *) * rm->r);

    for(r = 0; r < rm->r; ++r) {
        rm->dirty[r] = malloc(sizeof(bool) * rm->c);
        memset(rm->dirty[r], true, sizeof(bool) * rm->c);
    }

    r = 0;
    for(c = 0; c < rm->c; ++c) {
        rm->map[r][c] = get_tile_template(TT_WOOD_WALL_HOR);
        rm->map[r][c].r = r;
        rm->map[r][c].c = c;
    }

    r = rm->r-1;
    for(c = 0; c < rm->c; ++c) {
        rm->map[r][c] = get_tile_template(TT_WOOD_WALL_HOR);
        rm->map[r][c].r = r;
        rm->map[r][c].c = c;
    }

    c = 0;
    for(r = 0; r < rm->r; ++r) {
        rm->map[r][c] = get_tile_template(TT_WOOD_WALL_VER);
        rm->map[r][c].r = r;
        rm->map[r][c].c = c;
    }

    c = rm->c-1;
    for(r = 0; r < rm->r; ++r) {
        rm->map[r][c] = get_tile_template(TT_WOOD_WALL_VER);
        rm->map[r][c].r = r;
        rm->map[r][c].c = c;
    }

    rm->map[0][0] = get_tile_template(TT_WOOD_WALL_NW); rm->map[0][0].r = 0, rm->map[0][0].c = 0;
    rm->map[0][rm->c-1] = get_tile_template(TT_WOOD_WALL_NE); rm->map[0][rm->c-1].r = 0, rm->map[0][rm->c-1].c = rm->c-1;
    rm->map[rm->r-1][0] = get_tile_template(TT_WOOD_WALL_SW); rm->map[rm->r-1][0].r = rm->r-1, rm->map[rm->r-1][0].c = 0;
    rm->map[rm->r-1][rm->c-1] = get_tile_template(TT_WOOD_WALL_SE); rm->map[rm->r-1][rm->c-1].r = rm->r-1, rm->map[rm->r-1][rm->c-1].c = rm->c-1;

    for(r = 1; r < rm->r - 1; ++r) {
        for(c = 1; c < rm->c - 1; ++c) {
            rm->map[r][c] = get_tile_template(TT_WOOD_FLOOR);
            rm->map[r][c].r = r;
            rm->map[r][c].c = c;
        }
    }

    c = rm->c-2;
    for(r = 3; r < rm->r - 3; ++r) {
        rm->map[r][c] = get_tile_template(TT_WOOD_TABLE);
        rm->map[r][c].r = r;
        rm->map[r][c].c = c;
    }

    ASSIGN_DOOR_MACRO(TT_WOOD_DOOR_VER, DD_WEST, 4, 0);

    rm->map[3][rm->c-3].status |= TS_BLOOD;

    rm->entities = NULL;

    return rm;
}

room * get_start_room() {
    room * rm;
    int r, c, tv;
    bool flg;
    rm = malloc(sizeof(room));
    memset(rm, 0, sizeof(rm));

    rm->name = "Sacrificial Room";
    rm->desc = "A place of your awakening. A chaotic mess of runes and sigils are drawn haphazardly across the floor. You think it's enscribed with carrot blood.";

    rm->r = 13;
    rm->c = 11;
    
    rm->map = malloc(sizeof(tile *) * rm->r);

    for(r = 0; r < rm->r; ++r) {
        rm->map[r] = malloc(sizeof(tile) * rm->c);
    } 

    rm->dirty = malloc(sizeof(bool *) * rm->r);

    for(r = 0; r < rm->r; ++r) {
        rm->dirty[r] = malloc(sizeof(bool) * rm->c);
        memset(rm->dirty[r], true, sizeof(bool) * rm->c);
    }

    for(r = 0; r < rm->r; ++r) {
        for(c = 0; c < rm->c; ++c) {
            ASSIGN_TILE_MACRO(TT_DARK, r, c);
        }
    }
    
    ASSIGN_TILE_MACRO(TT_CAVE_WALL, 0, 7); ASSIGN_TILE_MACRO(TT_CAVE_WALL, 0, 8); ASSIGN_TILE_MACRO(TT_CAVE_WALL, 0, 9);
    ASSIGN_TILE_MACRO(TT_CAVE_WALL, 1, 4); ASSIGN_TILE_MACRO(TT_CAVE_WALL, 1, 5); ASSIGN_TILE_MACRO(TT_CAVE_WALL, 1, 6);
    ASSIGN_TILE_MACRO(TT_CAVE_WALL, 2, 2); ASSIGN_TILE_MACRO(TT_CAVE_WALL, 2, 3);
    ASSIGN_TILE_MACRO(TT_CAVE_WALL, 3, 1); ASSIGN_TILE_MACRO(TT_CAVE_WALL, 4, 1); 
    ASSIGN_TILE_MACRO(TT_CAVE_WALL, 5, 0); ASSIGN_TILE_MACRO(TT_CAVE_WALL, 6, 0); ASSIGN_TILE_MACRO(TT_CAVE_WALL, 7, 0);
    ASSIGN_TILE_MACRO(TT_CAVE_WALL, 8, 1); ASSIGN_TILE_MACRO(TT_CAVE_WALL, 9, 1);
    ASSIGN_TILE_MACRO(TT_CAVE_WALL, 10, 2); ASSIGN_TILE_MACRO(TT_CAVE_WALL, 10, 3);
    ASSIGN_TILE_MACRO(TT_CAVE_WALL, 11, 4); ASSIGN_TILE_MACRO(TT_CAVE_WALL, 11, 5); ASSIGN_TILE_MACRO(TT_CAVE_WALL, 11, 6);
    ASSIGN_TILE_MACRO(TT_CAVE_WALL, 12, 7); ASSIGN_TILE_MACRO(TT_CAVE_WALL, 12, 8); ASSIGN_TILE_MACRO(TT_CAVE_WALL, 12, 9);
    
    for(r = 0; r < rm->r; ++r) {
        flg = false;
        for(c = 0; c < rm->c; ++c) {
            if(rm->map[r][c].type == TT_CAVE_WALL) flg = true;
            else if(flg) {
                ASSIGN_TILE_MACRO(TT_CAVE_FLOOR, r, c);
            }
        }
    }

    c = rm->c-1;
    for(r = 1; r < rm->r-1; ++r) {
        ASSIGN_TILE_MACRO(TT_WOOD_WALL_VER, r, c);
    }
    ASSIGN_TILE_MACRO(TT_CAVE_WALL, 0, c);
    ASSIGN_TILE_MACRO(TT_CAVE_WALL, 12, c);

    ASSIGN_DOOR_MACRO(TT_WOOD_DOOR_VER, DD_EAST, 6, c);

    ASSIGN_STAT_MACRO(TS_BLOOD, 5, 4);
    ASSIGN_STAT_MACRO(TS_BLOOD, 5, 5);
    ASSIGN_STAT_MACRO(TS_BLOOD, 5, 6);
    ASSIGN_STAT_MACRO(TS_BLOOD, 6, 4);
    ASSIGN_STAT_MACRO(TS_BLOOD, 6, 6);
    ASSIGN_STAT_MACRO(TS_BLOOD, 7, 4);
    ASSIGN_STAT_MACRO(TS_BLOOD, 7, 5);
    ASSIGN_STAT_MACRO(TS_BLOOD, 7, 6);

    ASSIGN_STAT_MACRO(TS_BLOOD, 4, 3);
    ASSIGN_STAT_MACRO(TS_BLOOD, 4, 7);
    ASSIGN_STAT_MACRO(TS_BLOOD, 8, 3);
    ASSIGN_STAT_MACRO(TS_BLOOD, 8, 7);

    rm->entities = NULL;

    push_entity_into_room(rm, create_entity(ET_RABBIT), 6, 4);
    push_entity_into_room(rm, create_entity(ET_RABBIT), 9, 4);
    push_entity_into_room(rm, create_entity(ET_RABBIT), 9, 5);
    push_entity_into_room(rm, create_entity(ET_RABBIT), 9, 6);
    push_entity_into_room(rm, create_entity(ET_RABBIT), 9, 7);

    return rm;
}

room * get_corridor_room() {
    room * rm;
    int r, c, tv;
    bool flg;
    rm = malloc(sizeof(room));
    memset(rm, 0, sizeof(rm));
    
    rm->name = "Corridor";
    rm->desc = "A luxurious passageway. Its walls are dotted with drawings of famed rabbit heroes, and its floors covered with blood-red velvet.";

    rm->r = 5;
    rm->c = 17;
    
    rm->map = malloc(sizeof(tile *) * rm->r);

    for(r = 0; r < rm->r; ++r) {
        rm->map[r] = malloc(sizeof(tile) * rm->c);
    } 

    rm->dirty = malloc(sizeof(bool *) * rm->r);

    for(r = 0; r < rm->r; ++r) {
        rm->dirty[r] = malloc(sizeof(bool) * rm->c);
        memset(rm->dirty[r], true, sizeof(bool) * rm->c);
    }

    for(r = 0; r < rm->r; ++r) {
        for(c = 0; c < rm->c; ++c) {
            ASSIGN_TILE_MACRO(TT_WOOD_FLOOR, r, c);
        }
    }

    for(c = 1; c < rm->c-1; ++c) {
        ASSIGN_TILE_MACRO(TT_WOOD_WALL_HOR, 0, c);
        ASSIGN_TILE_MACRO(TT_WOOD_WALL_HOR, rm->r-1, c);
    }
    for(r = 1; r < rm->r-1; ++r) {
        ASSIGN_TILE_MACRO(TT_WOOD_WALL_VER, r, 0);
        ASSIGN_TILE_MACRO(TT_WOOD_WALL_VER, r, rm->c-1);
    }
    ASSIGN_TILE_MACRO(TT_WOOD_WALL_NW, 0, 0);
    ASSIGN_TILE_MACRO(TT_WOOD_WALL_NE, 0, rm->c-1);
    ASSIGN_TILE_MACRO(TT_WOOD_WALL_SE, rm->r-1, rm->c-1);
    ASSIGN_TILE_MACRO(TT_WOOD_WALL_SW, rm->r-1, 0);

    ASSIGN_DOOR_MACRO(TT_WOOD_DOOR_VER, DD_WEST, rm->r/2, 0);
    ASSIGN_DOOR_MACRO(TT_WOOD_DOOR_VER, DD_EAST, rm->r/2, rm->c-1);

    for(c = 1; c < rm->c-1; ++c) {
        ASSIGN_TILE_MACRO(TT_RED_CARPET, 2, c);
    }

    rm->entities = NULL;

    return rm;
}

void draw_room(room *rm) {
    int r, c;

    for(r = 0; r < rm->r; ++r) {
        for(c = 0; c < rm->c; ++c) {
            if(rm->dirty[r][c]) {
                rm->dirty[r][c] = false;

                draw_tile(rm->map[r] + c);

                if(rm->map[r][c].entity_id != -1)
                    draw_entity(rm->entities[rm->map[r][c].entity_id]);
            }
        }
    }
}

void free_room(room *rm) {
    int r, c;
    int i;

    for(r = 0; r < rm->r; ++r) {
        free_tile(rm->map[r]);
        free(rm->dirty[r]);
    }

    for(i = 0; i < cvector_size(rm->entities); ++i) {
        if(rm->entities[i]->type == ET_CARROT) continue;
        free_entity(rm->entities[i]);
    }

    cvector_free(rm->entities);

    free(rm->map);
    free(rm->dirty);
    free(rm);
}

void pop_entity_from_room(room *rm, entity *e) {
    int i, j, size;
    if(rm == NULL) rm = get_cur_room();

    for(i = 0; i < cvector_size(rm->entities); ++i) {
        if(rm->entities[i] == e) {
            rm->map[e->r][e->c].entity_id = rm->map[e->r][e->c].player_id = -1;
            rm->dirty[e->r][e->c] = true;
            cvector_erase(rm->entities, i);

            size = cvector_size(rm->entities);
            for(j=0;j<size; j++){
                rm->map[rm->entities[j]->r][rm->entities[j]->c].entity_id = j;
            }

            return;
        }
    }

    fprintf(stderr, "Tried to pop an entity that is not in a room\n");
    exit(1);
}

void push_entity_into_room(room *rm, entity *e, int row, int col) {
    if(rm == NULL) rm = get_cur_room();
    
    if(row < 0 || row >= rm->r || col < 0 || col >= rm->c || rm->map[row][col].entity_id != -1) {
        raise("push_entity_into_room");
    }
    rm->map[row][col].entity_id = cvector_size(rm->entities);
    rm->dirty[row][col] = true;
    cvector_push_back(rm->entities, e);
    e->tile = rm->map[row] + col;
    e->r = row;
    e->c = col;
}

//player를 현재 room에다 배치하는 함수. map의 row행 col열에 배치되며, room의 entities[idx]에 player의 entity가 들어가게 된다. 
//room이 변경된 후 반드시 호출해주어야 하는 함수.
void push_player_into_room(int row, int col){
    room *rm = get_cur_room();
    push_entity_into_room(NULL, get_player(), row, col);
    rm->map[row][col].player_id = 1;
}

door_dir get_door_dir_opp(door_dir dir) {
    return door_opp[dir];
}