#include <ncurses.h>
#include <stdlib.h>
#include "floor.h"
#include "entity.h"
#include "player.h"
#include "util.h"

#define ASSIGN_TILE_MACRO(type, row, col) rm->map[row][col] = get_tile_template(type); rm->map[row][col].r = row; rm->map[row][col].c = col
#define ASSIGN_STAT_MACRO(flg, row, col) rm->map[row][col].status |= flg

room * get_tmp_room() {
    room * rm;
    int r, c;
    rm = malloc(sizeof(room));

    rm->r = 7;
    rm->c = 12;
    
    rm->map = malloc(sizeof(tile *) * rm->r);

    for(r = 0; r < rm->r; ++r) {
        rm->map[r] = malloc(sizeof(tile) * rm->c);
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

    rm->r = 9;
    rm->c = 6;
    
    rm->map = malloc(sizeof(tile *) * rm->r);

    for(r = 0; r < rm->r; ++r) {
        rm->map[r] = malloc(sizeof(tile) * rm->c);
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

    rm->map[3][rm->c-3].status |= TS_BLOOD;

    rm->entities = NULL;

    return rm;
}

room * get_start_room() {
    room * rm;
    int r, c, tv;
    bool flg;
    rm = malloc(sizeof(room));

    rm->r = 13;
    rm->c = 11;
    
    rm->map = malloc(sizeof(tile *) * rm->r);

    for(r = 0; r < rm->r; ++r) {
        rm->map[r] = malloc(sizeof(tile) * rm->c);
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

    return rm;
}

void draw_room(room *rm) {
    int r, c;

    for(r = 0; r < rm->r; ++r) {
        for(c = 0; c < rm->c; ++c) {
            if(rm->map[r][c].entity_id == -1)
                draw_tile(rm->map[r] + c);
            else
                draw_entity(rm->entities[rm->map[r][c].entity_id]);
        }
    }
}

void free_room(room *rm) {
    int r, c;
    int i;

    for(r = 0; r < rm->r; ++r) {
        free_tile(rm->map[r]);
    }

    for(i = 0; i < cvector_size(rm->entities); ++i) {
        if(rm->entities[i]->type == ET_CARROT) continue;
        free_entity(rm->entities[i]);
    }

    cvector_free(rm->entities);

    free(rm->map);
    free(rm);
}

void push_entity_into_room(room *rm, entity *e, int row, int col) {
    if(rm == NULL) rm = get_cur_room();
    
    if(row < 0 || row >= rm->r || col < 0 || col >= rm->c || rm->map[row][col].entity_id != -1) {
        raise("push_entity_into_room");
    }
    rm->map[row][col].entity_id = cvector_size(rm->entities);
    cvector_push_back(rm->entities, e);
    e->r = row;
    e->c = col;
}