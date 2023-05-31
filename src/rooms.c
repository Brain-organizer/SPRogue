#include <ncurses.h>
#include <stdlib.h>
#include "floor.h"
#include "entity.h"
#include "player.h"
#include "util.h"
#include "sidebar.h"

#define ASSIGN_TILE_MACRO(type, row, col) rm->map[row][col] = get_tile_template(type); rm->map[row][col].r = row; rm->map[row][col].c = col
#define ASSIGN_DOOR_MACRO(type, row, col) ASSIGN_TILE_MACRO(type, row, col); rm->map[row][col].door_id = cvector_size(rm->doors); tdoor.prev = rm->map[row] + col; tdoor.next = NULL; tdoor.room_id = -1; cvector_push_back(rm->doors, tdoor)
#define ASSIGN_STAT_MACRO(flg, row, col) rm->map[row][col].status |= flg

bool is_door_ok(door d) {
    return d.prev && !d.next;
}

room * get_tmp_room() {
    room * rm;
    int r, c, tv;
    bool flg;
    door tdoor;

    rm = malloc(sizeof(room));
    memset(rm, 0, sizeof(rm));

    rm->doors = NULL;

    rm->roff = 0;
    rm->coff = 0;
    
    rm->name = "Test Room";
    rm->desc = "Room to see all present tiles";

    rm->r = TILE_NUM;
    rm->c = 2;

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

    for(r = 0; r < TILE_NUM; ++r) {
        ASSIGN_TILE_MACRO(r, r, 0);
        ASSIGN_TILE_MACRO(TT_WOOD_FLOOR, r, 1);
    }


    rm->entities = NULL;

    return rm;
}

room * get_butcher_room() {
    room * rm;
    int r, c;
    door tdoor;

    rm = malloc(sizeof(room));
    memset(rm, 0, sizeof(rm));

    rm->doors = NULL;

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

    ASSIGN_DOOR_MACRO(TT_WOOD_DOOR_VER, 4, 0);

    rm->map[3][rm->c-3].status |= TS_BLOOD;

    rm->entities = NULL;

    return rm;
}

void dfs_start_room(room *rm, int r, int c) {
    int i, rr, cc;
    
    if(rm->map[r][c].type != TT_DARK) return;
    
    ASSIGN_TILE_MACRO(TT_CAVE_FLOOR, r, c);

    for(i = 0; i < MOVE_TYPES; ++i) {
        rr = r + MOVE_DIRS[i][0];
        cc = c + MOVE_DIRS[i][1];
        if(rr >= 0 && rr < rm->r && cc >= 0 && cc < rm->c && rm->map[rr][cc].type != TT_CAVE_FLOOR) {
            dfs_start_room(rm, rr, cc);
        }
    }
}

room * get_start_room() {
    room * rm;
    int r, c, tv;
    bool flg;
    door tdoor;

    rm = malloc(sizeof(room));
    memset(rm, 0, sizeof(rm));

    rm->doors = NULL;

    rm->roff = 5;
    rm->coff = 25;
    
    rm->name = "Sacrificial Room";
    rm->desc = "A place of your awakening. A chaotic mess of runes and sigils are drawn haphazardly across the floor. You think it's enscribed with carrot blood.";

    rm->r = 18 + rm->roff;
    rm->c = 13 + rm->coff;
    
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
    
    ASSIGN_TILE_MACRO(TT_CAVE_WALL, rm->roff+0, 11+rm->coff);
    ASSIGN_TILE_MACRO(TT_CAVE_WALL, rm->roff+1, 5+rm->coff); ASSIGN_TILE_MACRO(TT_CAVE_WALL, rm->roff+1, 6+rm->coff); ASSIGN_TILE_MACRO(TT_CAVE_WALL, rm->roff+1, 7+rm->coff); ASSIGN_TILE_MACRO(TT_CAVE_WALL, rm->roff+1, 9+rm->coff); ASSIGN_TILE_MACRO(TT_CAVE_WALL, rm->roff+1, 10+rm->coff);
    ASSIGN_TILE_MACRO(TT_CAVE_WALL, rm->roff+2, 4+rm->coff); ASSIGN_TILE_MACRO(TT_CAVE_WALL, rm->roff+2, 8+rm->coff);
    ASSIGN_TILE_MACRO(TT_CAVE_WALL, rm->roff+3, 4+rm->coff);
    ASSIGN_TILE_MACRO(TT_CAVE_WALL, rm->roff+4, 3+rm->coff);
    ASSIGN_TILE_MACRO(TT_CAVE_WALL, rm->roff+5, 3+rm->coff);
    ASSIGN_TILE_MACRO(TT_CAVE_WALL, rm->roff+6, 4+rm->coff);
    ASSIGN_TILE_MACRO(TT_CAVE_WALL, rm->roff+7, 1+rm->coff); ASSIGN_TILE_MACRO(TT_CAVE_WALL, rm->roff+7, 2+rm->coff); ASSIGN_TILE_MACRO(TT_CAVE_WALL, rm->roff+7, 3+rm->coff); 
    ASSIGN_TILE_MACRO(TT_CAVE_WALL, rm->roff+8, 1+rm->coff);
    ASSIGN_TILE_MACRO(TT_CAVE_WALL, rm->roff+9, 1+rm->coff);
    ASSIGN_TILE_MACRO(TT_CAVE_WALL, rm->roff+10, 2+rm->coff);
    ASSIGN_TILE_MACRO(TT_CAVE_WALL, rm->roff+11, 2+rm->coff);
    ASSIGN_TILE_MACRO(TT_CAVE_WALL, rm->roff+12, 1+rm->coff);
    ASSIGN_TILE_MACRO(TT_CAVE_WALL, rm->roff+13, 0+rm->coff);
    ASSIGN_TILE_MACRO(TT_CAVE_WALL, rm->roff+14, 1+rm->coff);
    ASSIGN_TILE_MACRO(TT_CAVE_WALL, rm->roff+15, 1+rm->coff); ASSIGN_TILE_MACRO(TT_CAVE_WALL, rm->roff+15, 6+rm->coff);
    ASSIGN_TILE_MACRO(TT_CAVE_WALL, rm->roff+16, 2+rm->coff); ASSIGN_TILE_MACRO(TT_CAVE_WALL, rm->roff+16, 5+rm->coff); ASSIGN_TILE_MACRO(TT_CAVE_WALL, rm->roff+16, 7+rm->coff); ASSIGN_TILE_MACRO(TT_CAVE_WALL, rm->roff+16, 10+rm->coff);
    ASSIGN_TILE_MACRO(TT_CAVE_WALL, rm->roff+17, 3+rm->coff); ASSIGN_TILE_MACRO(TT_CAVE_WALL, rm->roff+17, 4+rm->coff); ASSIGN_TILE_MACRO(TT_CAVE_WALL, rm->roff+17, 7+rm->coff); ASSIGN_TILE_MACRO(TT_CAVE_WALL, rm->roff+17, 8+rm->coff); ASSIGN_TILE_MACRO(TT_CAVE_WALL, rm->roff+17, 9+rm->coff); ASSIGN_TILE_MACRO(TT_CAVE_WALL, rm->roff+17, 11+rm->coff);
    
    ASSIGN_TILE_MACRO(TT_CAVE_WALL, rm->roff+0, 12+rm->coff); ASSIGN_TILE_MACRO(TT_CAVE_WALL, rm->roff+17, 12+rm->coff); 
    
    c = 12;
    for(r = 1; r < 17; ++r) {
        ASSIGN_TILE_MACRO(TT_WOOD_WALL_VER, r+rm->roff, c+rm->coff);
    }

    ASSIGN_DOOR_MACRO(TT_WOOD_DOOR_VER, 9+rm->roff, 12+rm->coff);

    dfs_start_room(rm, 9+rm->roff, 7+rm->coff);

    ASSIGN_STAT_MACRO(TS_BLOOD, 8+rm->roff, 7+rm->coff);
    ASSIGN_STAT_MACRO(TS_BLOOD, 10+rm->roff, 7+rm->coff);
    ASSIGN_STAT_MACRO(TS_BLOOD, 9+rm->roff, 6+rm->coff);
    ASSIGN_STAT_MACRO(TS_BLOOD, 9+rm->roff, 8+rm->coff);
    ASSIGN_STAT_MACRO(TS_BLOOD, 9+rm->roff, 9+rm->coff);
    ASSIGN_STAT_MACRO(TS_BLOOD, 10+rm->roff, 5+rm->coff);

    ASSIGN_STAT_MACRO(TS_BLOOD, 7+rm->roff, 7+rm->coff);
    ASSIGN_STAT_MACRO(TS_BLOOD, 6+rm->roff, 6+rm->coff);
    ASSIGN_STAT_MACRO(TS_BLOOD, 5+rm->roff, 7+rm->coff);
    ASSIGN_STAT_MACRO(TS_BLOOD, 4+rm->roff, 6+rm->coff);
    ASSIGN_STAT_MACRO(TS_BLOOD, 4+rm->roff, 8+rm->coff);
    ASSIGN_STAT_MACRO(TS_BLOOD, 3+rm->roff, 7+rm->coff);

    ASSIGN_STAT_MACRO(TS_BOMB, 4+rm->roff, 7+rm->coff);

    ASSIGN_STAT_MACRO(TS_BLOOD, 11+rm->roff, 7+rm->coff);
    ASSIGN_STAT_MACRO(TS_BLOOD, 12+rm->roff, 7+rm->coff);
    ASSIGN_STAT_MACRO(TS_BLOOD, 13+rm->roff, 7+rm->coff);

    ASSIGN_STAT_MACRO(TS_BLOOD, 13+rm->roff, 8+rm->coff);
    ASSIGN_STAT_MACRO(TS_BLOOD, 14+rm->roff, 7+rm->coff);
    ASSIGN_STAT_MACRO(TS_BLOOD, 14+rm->roff, 9+rm->coff);
    ASSIGN_STAT_MACRO(TS_BLOOD, 15+rm->roff, 8+rm->coff);

    ASSIGN_STAT_MACRO(TS_PEER, 14+rm->roff, 8+rm->coff);

    ASSIGN_STAT_MACRO(TS_BLOOD, 12+rm->roff, 6+rm->coff);
    ASSIGN_STAT_MACRO(TS_BLOOD, 13+rm->roff, 5+rm->coff);
    ASSIGN_STAT_MACRO(TS_BLOOD, 13+rm->roff, 4+rm->coff);
    ASSIGN_STAT_MACRO(TS_HEALTH, 14+rm->roff, 6+rm->coff);

    rm->entities = NULL;

    //push_entity_into_room(rm, create_entity(ET_BEAR), 14+rm->roff, 6+rm->coff, -1);
    //push_entity_into_room(rm, create_entity(ET_RABBIT), 14+rm->roff, 7+rm->coff, -1);
    //push_entity_into_room(rm, create_entity(ET_RUNNER), 14+rm->roff, 8+rm->coff, -1);
    //push_entity_into_room(rm, create_entity(ET_CAPYBARA), rm->roff+15, rm->coff+7, -1);
    //push_entity_into_room(rm, create_entity(ET_HORSE), rm->roff+15, rm->coff+8, -1);
    
    return rm;
}

room * get_storage_room() {
    room * rm;
    int r, c, tv;
    bool flg;
    door tdoor;

    rm = malloc(sizeof(room));
    memset(rm, 0, sizeof(rm));

    rm->doors = NULL;

    rm->roff = 6;
    rm->coff = 20;
    
    rm->name = "Storage Room";
    rm->desc = "A place where rabbits store their goods, the smell of dust and rat piss permeates the air. Numerous shelves and boxes are coated with thick layer of dust.";

    rm->r = 17 + rm->roff;
    rm->c = 20 + rm->coff;
    
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

    for(r = rm->roff; r < rm->r; ++r) {
        for(c = rm->coff; c < rm->c; ++c) {
            ASSIGN_TILE_MACRO(TT_WOOD_FLOOR, r, c);
        }
    }
    for(c = rm->coff; c < rm->c; ++c) {
        ASSIGN_TILE_MACRO(TT_WOOD_WALL_HOR, rm->roff, c);
        ASSIGN_TILE_MACRO(TT_WOOD_WALL_HOR, rm->r-1, c);
    }
    for(r = rm->roff; r < rm->r; ++r) {
        ASSIGN_TILE_MACRO(TT_WOOD_WALL_VER, r, rm->coff);
        ASSIGN_TILE_MACRO(TT_WOOD_WALL_VER, r, rm->c-1);
    }
    ASSIGN_TILE_MACRO(TT_WOOD_WALL_NW, rm->roff, rm->coff);
    ASSIGN_TILE_MACRO(TT_WOOD_WALL_NE, rm->roff, rm->c-1);
    ASSIGN_TILE_MACRO(TT_WOOD_WALL_SW, rm->r-1, rm->coff);
    ASSIGN_TILE_MACRO(TT_WOOD_WALL_SE, rm->r-1, rm->c-1);
    
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+1, rm->coff+2);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+1, rm->coff+3);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+1, rm->coff+4);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+1, rm->coff+5);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+1, rm->coff+6);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+1, rm->coff+7);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+2, rm->coff+2);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+2, rm->coff+3);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+2, rm->coff+4);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+2, rm->coff+5);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+2, rm->coff+6);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+2, rm->coff+7);

    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+1, rm->coff+9);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+1, rm->coff+10);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+1, rm->coff+11);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+1, rm->coff+12);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+1, rm->coff+13);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+1, rm->coff+14);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+1, rm->coff+15);
    //ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+2, rm->coff+9);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+2, rm->coff+10);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+2, rm->coff+11);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+2, rm->coff+12);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+2, rm->coff+13);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+2, rm->coff+14);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+2, rm->coff+15);

    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+5, rm->coff+1);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+5, rm->coff+2);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+5, rm->coff+3);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+5, rm->coff+4);
    //ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+5, rm->coff+5);
    //ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+5, rm->coff+6);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+5, rm->coff+7);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+6, rm->coff+1);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+6, rm->coff+2);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+6, rm->coff+3);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+6, rm->coff+4);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+6, rm->coff+5);
    //ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+6, rm->coff+6);
    //ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+6, rm->coff+7);
    
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+5, rm->coff+9);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+5, rm->coff+10);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+5, rm->coff+11);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+5, rm->coff+12);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+5, rm->coff+13);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+5, rm->coff+14);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+5, rm->coff+15);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+6, rm->coff+9);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+6, rm->coff+10);
    //ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+6, rm->coff+11);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+6, rm->coff+12);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+6, rm->coff+13);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+6, rm->coff+14);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+6, rm->coff+15);

    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+9, rm->coff+1);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+9, rm->coff+2);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+9, rm->coff+3);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+9, rm->coff+4);
    //ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+9, rm->coff+5);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+9, rm->coff+6);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+9, rm->coff+7);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+10, rm->coff+1);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+10, rm->coff+2);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+10, rm->coff+3);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+10, rm->coff+4);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+10, rm->coff+5);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+10, rm->coff+6);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+10, rm->coff+7);
    
    //ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+9, rm->coff+9);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+9, rm->coff+10);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+9, rm->coff+11);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+9, rm->coff+12);
    //ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+9, rm->coff+13);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+9, rm->coff+14);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+9, rm->coff+15);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+10, rm->coff+9);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+10, rm->coff+10);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+10, rm->coff+11);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+10, rm->coff+12);
    //ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+10, rm->coff+13);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+10, rm->coff+14);
    ASSIGN_TILE_MACRO(TT_SHELF, rm->roff+10, rm->coff+15);

    ASSIGN_TILE_MACRO(TT_BOX, rm->r-4, rm->coff+1);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-4, rm->coff+2);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-4, rm->coff+3);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-4, rm->coff+4);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-4, rm->coff+5);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-4, rm->coff+6);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-4, rm->coff+7);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-4, rm->coff+8);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-4, rm->coff+9);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-4, rm->coff+10);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-4, rm->coff+11);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-4, rm->coff+12);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-4, rm->coff+13);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-4, rm->coff+14);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-4, rm->coff+15);

    ASSIGN_TILE_MACRO(TT_BOX, rm->r-3, rm->coff+1);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-3, rm->coff+2);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-3, rm->coff+3);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-3, rm->coff+4);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-3, rm->coff+5);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-3, rm->coff+6);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-3, rm->coff+7);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-3, rm->coff+8);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-3, rm->coff+9);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-3, rm->coff+10);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-3, rm->coff+11);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-3, rm->coff+12);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-3, rm->coff+13);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-3, rm->coff+14);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-3, rm->coff+15);

    ASSIGN_TILE_MACRO(TT_BOX, rm->r-2, rm->coff+1);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-2, rm->coff+2);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-2, rm->coff+3);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-2, rm->coff+4);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-2, rm->coff+5);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-2, rm->coff+6);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-2, rm->coff+7);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-2, rm->coff+8);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-2, rm->coff+9);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-2, rm->coff+10);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-2, rm->coff+11);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-2, rm->coff+12);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-2, rm->coff+13);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-2, rm->coff+14);
    ASSIGN_TILE_MACRO(TT_BOX, rm->r-2, rm->coff+15);

    for(r = rm->roff+1; r < rm->r-1; ++r) {
        ASSIGN_TILE_MACRO(TT_SHELF, r, rm->c-2);
    }

    ASSIGN_DOOR_MACRO(TT_WOOD_DOOR_HOR, rm->roff, rm->coff+1);
    ASSIGN_DOOR_MACRO(TT_WOOD_DOOR_HOR, rm->r-1, rm->c-3);

    ASSIGN_STAT_MACRO(TS_BOMB, rm->roff+1, rm->coff+8);
    ASSIGN_STAT_MACRO(TS_PEER, rm->roff+6, rm->coff+11);
    ASSIGN_STAT_MACRO(TS_HEALTH, rm->roff+9, rm->coff+5);

    ASSIGN_STAT_MACRO(TS_BLOOD, rm->roff+6, rm->coff+11);
    ASSIGN_STAT_MACRO(TS_BLOOD, rm->roff+7, rm->coff+10);
    ASSIGN_STAT_MACRO(TS_BLOOD, rm->roff+7, rm->coff+11);
    ASSIGN_STAT_MACRO(TS_BLOOD, rm->roff+7, rm->coff+12);

    ASSIGN_STAT_MACRO(TS_BLOOD, rm->roff+1, rm->coff+8);
    ASSIGN_STAT_MACRO(TS_BLOOD, rm->roff+2, rm->coff+8);

    rm->entities = NULL;

    push_entity_into_room(rm, create_entity(ET_RABBIT), rm->roff+12, rm->coff+12, -1);
    push_entity_into_room(rm, create_entity(ET_RABBIT), rm->roff+12, rm->coff+13, -1);
    push_entity_into_room(rm, create_entity(ET_RABBIT), rm->roff+11, rm->coff+14, -1);
    push_entity_into_room(rm, create_entity(ET_RABBIT), rm->roff+12, rm->coff+3, -1);
    push_entity_into_room(rm, create_entity(ET_RABBIT), rm->roff+6, rm->coff+11, -1);
    push_entity_into_room(rm, create_entity(ET_RABBIT), rm->roff+6, rm->coff+6, -1);
    push_entity_into_room(rm, create_entity(ET_RABBIT), rm->roff+1, rm->coff+8, -1);

    return rm;
}

room * get_corridor_room() {
    room * rm;
    int r, c, tv;
    bool flg;
    door tdoor;

    rm = malloc(sizeof(room));
    memset(rm, 0, sizeof(rm));

    rm->doors = NULL;
    
    rm->name = "Corridor";
    rm->desc = "A luxurious passageway. Its walls are dotted with drawings of famed rabbit heroes, and its floors are covered with blood-red velvet.";

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

    ASSIGN_DOOR_MACRO(TT_WOOD_DOOR_VER, rm->r/2, 0);
    ASSIGN_DOOR_MACRO(TT_WOOD_DOOR_VER, rm->r/2, rm->c-1);

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
            rm->map[e->r][e->c].entity_id = -1;
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

void push_entity_into_room(room *rm, entity *e, int row, int col, int ind) {
    int i;

    if(rm == NULL) rm = get_cur_room();
    
    if(row < 0 || row >= rm->r || col < 0 || col >= rm->c || rm->map[row][col].entity_id != -1) {
        raise("push_entity_into_room");
    }
    rm->map[row][col].entity_id = cvector_size(rm->entities);
    rm->dirty[row][col] = true;

    if(ind == -1) cvector_push_back(rm->entities, e);
    else cvector_insert(rm->entities, ind, e);

    e->tile = rm->map[row] + col;
    e->r = row;
    e->c = col;

    for(i = 0; i < cvector_size(rm->entities); ++i) {
        rm->entities[i]->tile->entity_id = i;
    }
}

//player를 현재 room에다 배치하는 함수. map의 row행 col열에 배치되며, room의 entities[idx]에 player의 entity가 들어가게 된다. 
//room이 변경된 후 반드시 호출해주어야 하는 함수.
void push_player_into_room(int row, int col){
    push_entity_into_room(NULL, get_player(), row, col, 0);
}