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

room * get_passageway() {
     room * rm;
    int r, c, tv;
    bool flg;
    door tdoor;

    rm = malloc(sizeof(room));
    memset(rm, 0, sizeof(rm));

    rm->doors = NULL;

    rm->roff = 13;
    rm->coff = 20;
    
    rm->name = "Passageway";
    rm->desc = "You are in a passageway, a large corridor that connects the many rooms of this barn. Most of the doors, however, are locked shut.";
   
    rm->r = 5 + rm->roff;
    rm->c = 36 + rm->coff;
    
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

    ASSIGN_DOOR_MACRO(TT_WOOD_DOOR_HOR, rm->roff, rm->coff+7);
    ASSIGN_DOOR_MACRO(TT_WOOD_DOOR_HOR, rm->r-1, rm->c-8);

    rm->entities = NULL;

    push_entity_into_room(rm, create_entity(ET_HORSE), rm->r-2, rm->c-8, -1);

    return rm;
}

room * get_butcher_room() {
    room * rm;
    int r, c, tv;
    bool flg;
    door tdoor;

    rm = malloc(sizeof(room));
    memset(rm, 0, sizeof(rm));

    rm->doors = NULL;

    rm->roff = 7;
    rm->coff = 31;
    
    rm->name = "Butcher's Room";
    rm->desc = "This is the butcher's room, a place where the carrots are turned into sustenance for the rabbit lords. You can smell the blood of the innocents from its red-stained floors.";
   
    rm->r = 17 + rm->roff;
    rm->c = 17 + rm->coff;
    
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

    for(r = rm->roff+3; r < rm->roff+9; ++r) {
        for(c = rm->coff+3; c < rm->coff+6; ++c) {
            ASSIGN_TILE_MACRO(TT_WOOD_TABLE, r, c);
        }
    }

    for(r = rm->roff+3; r < rm->roff+9; ++r) {
        for(c = rm->coff+11; c < rm->coff+14; ++c) {
            ASSIGN_TILE_MACRO(TT_WOOD_TABLE, r, c);
        }
    }

    for(c = rm->coff+1; c < rm->c-1; ++c) {
        ASSIGN_TILE_MACRO(TT_WOOD_WALL_HOR, rm->roff+11, c);
    }
    ASSIGN_TILE_MACRO(TT_WOOD_FLOOR, rm->roff+11, rm->coff+8);
    for(r = rm->roff+12; r < rm->r-1; ++r) {
        for(c = rm->coff+1; c < rm->c-1; ++c) {
            ASSIGN_TILE_MACRO(TT_WOOD_FLOOR, r, c);
        }
    }

    ASSIGN_DOOR_MACRO(TT_WOOD_DOOR_HOR, rm->roff+0, rm->coff+8);
    ASSIGN_DOOR_MACRO(TT_WOOD_DOOR_VER, rm->roff+10, rm->coff+16);

    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+1, rm->coff+1);
    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+1, rm->coff+2);
    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+1, rm->coff+3);
    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+1, rm->coff+4);
    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+1, rm->coff+5);
    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+1, rm->coff+6);

    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+1, rm->coff+10);
    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+1, rm->coff+11);
    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+1, rm->coff+12);
    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+1, rm->coff+13);
    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+1, rm->coff+14);
    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+1, rm->coff+15);

    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+13, rm->coff+1);
    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+13, rm->coff+2);
    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+13, rm->coff+3);
    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+13, rm->coff+4);
    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+13, rm->coff+5);
    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+13, rm->coff+6);
    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+13, rm->coff+7);

    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+13, rm->coff+9);
    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+13, rm->coff+10);
    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+13, rm->coff+11);
    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+13, rm->coff+12);
    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+13, rm->coff+13);
    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+13, rm->coff+14);
    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+13, rm->coff+15);

    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+15, rm->coff+1);
    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+15, rm->coff+2);
    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+15, rm->coff+3);
    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+15, rm->coff+4);
    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+15, rm->coff+5);
    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+15, rm->coff+6);
    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+15, rm->coff+7);

    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+15, rm->coff+9);
    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+15, rm->coff+10);
    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+15, rm->coff+11);
    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+15, rm->coff+12);
    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+15, rm->coff+13);
    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+15, rm->coff+14);
    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+15, rm->coff+15);

    ASSIGN_STAT_MACRO(TS_PEER, rm->roff+13, rm->coff+11);
    ASSIGN_STAT_MACRO(TS_PEER, rm->roff+15, rm->coff+6);
    ASSIGN_STAT_MACRO(TS_BOMB, rm->roff+15, rm->coff+14);
    ASSIGN_STAT_MACRO(TS_HEALTH, rm->roff+13, rm->coff+1);

    rm->entities = NULL;

    push_entity_into_room(rm, create_entity(ET_CAPYBARA), 7+rm->roff, 8+rm->coff, -1);
    push_entity_into_room(rm, create_entity(ET_RABBIT), 8+rm->roff, 6+rm->coff, -1);
    push_entity_into_room(rm, create_entity(ET_RABBIT), 7+rm->roff, 10+rm->coff, -1);
    push_entity_into_room(rm, create_entity(ET_RABBIT), 5+rm->roff, 7+rm->coff, -1);
    push_entity_into_room(rm, create_entity(ET_RABBIT), 2+rm->roff, 13+rm->coff, -1);\

    push_entity_into_room(rm, create_entity(ET_RABBIT), 14+rm->roff, 6+rm->coff, -1);
    push_entity_into_room(rm, create_entity(ET_RABBIT), 14+rm->roff, 12+rm->coff, -1);

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
    rm->coff = 32;
    
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

    rm->roff = 7;
    rm->coff = 29;
    
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

    rm->roff = 12;
    rm->coff = 15;
    
    rm->name = "Corridor";
    rm->desc = "A luxurious passageway. Its walls are lined with marble statues of rabbit heroes, and its floors are covered with blood-red velvet.";

    rm->r = 7 + rm->roff;
    rm->c = 50 + rm->coff;
    
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

    ASSIGN_DOOR_MACRO(TT_WOOD_DOOR_VER, rm->roff+3, rm->coff);
    ASSIGN_DOOR_MACRO(TT_WOOD_DOOR_VER, rm->roff+3, rm->c-1);

    for(r = rm->roff+2; r < rm->roff+5; ++r) {
        for(c = rm->coff+1; c < rm->c-1; ++c) {
            ASSIGN_TILE_MACRO(TT_RED_CARPET, r, c);
        }
    }
    for(c = rm->coff+4; c < rm->c-3; c += 4) {
        ASSIGN_TILE_MACRO(TT_STATUE, rm->roff+1, c);
        ASSIGN_TILE_MACRO(TT_STATUE, rm->r-2, c);
    }

    rm->entities = NULL;

    return rm;
}

room * get_ambush_room() {
    room * rm;
    int r, c, tv;
    bool flg;
    door tdoor;

    rm = malloc(sizeof(room));
    memset(rm, 0, sizeof(rm));

    rm->doors = NULL;

    rm->roff = 3;
    rm->coff = 35;
    
    rm->name = "Ambush";
    rm->desc = "The rabbits are striking back! They have set up an ambush, and you must react! The horses seem extra dull today, however. Maybe you should act before they realize the situation.";

    rm->check = true;

    rm->r = 25 + rm->roff;
    rm->c = 9 + rm->coff;
    
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
    
    ASSIGN_DOOR_MACRO(TT_WOOD_DOOR_VER, rm->roff+12, rm->coff+0);
    ASSIGN_DOOR_MACRO(TT_WOOD_DOOR_VER, rm->roff+2, rm->coff+8);
    ASSIGN_DOOR_MACRO(TT_WOOD_DOOR_HOR, rm->roff+24, rm->coff+4);

    rm->entities = NULL;

    push_entity_into_room(rm, create_entity(ET_HORSE), rm->roff+22, rm->coff+2, -1);
    push_entity_into_room(rm, create_entity(ET_HORSE), rm->roff+22, rm->coff+4, -1);
    push_entity_into_room(rm, create_entity(ET_HORSE), rm->roff+22, rm->coff+6, -1);
    push_entity_into_room(rm, create_entity(ET_HORSE), rm->roff+23, rm->coff+3, -1);
    push_entity_into_room(rm, create_entity(ET_HORSE), rm->roff+23, rm->coff+5, -1);

    push_entity_into_room(rm, create_entity(ET_CAPYBARA), rm->roff+2, rm->coff+4, -1);
    //push_entity_into_room(rm, create_entity(ET_CAPYBARA), rm->roff+2, rm->coff+5, -1);
    push_entity_into_room(rm, create_entity(ET_RABBIT), rm->roff+3, rm->coff+2, -1);
    //push_entity_into_room(rm, create_entity(ET_RABBIT), rm->roff+3, rm->coff+3, -1);
    push_entity_into_room(rm, create_entity(ET_RABBIT), rm->roff+3, rm->coff+4, -1);
    //push_entity_into_room(rm, create_entity(ET_RABBIT), rm->roff+3, rm->coff+5, -1);
    push_entity_into_room(rm, create_entity(ET_RABBIT), rm->roff+3, rm->coff+6, -1);

    return rm;
}

room * get_boss_room() {
    room * rm;
    int r, c, tv;
    bool flg;
    door tdoor;

    rm = malloc(sizeof(room));
    memset(rm, 0, sizeof(rm));

    rm->check = true;

    rm->doors = NULL;

    rm->roff = 10;
    rm->coff = 20;
    
    rm->name = "Den of Bears";
    rm->desc = "A place of residence for the bears, it is from this seat of power that they command. The bears, however, seem preoccupied.";

    rm->r = 18 + rm->roff;
    rm->c = 40 + rm->coff;
    
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

    for(r = rm->roff+4; r < rm->r; ++r) {
        for(c = rm->coff+1; c < rm->c; ++c) {
            ASSIGN_TILE_MACRO(TT_WOOD_FLOOR, r, c);
        }
    }
    ASSIGN_TILE_MACRO(TT_WOOD_FLOOR, rm->roff+1, rm->coff+30);
    ASSIGN_TILE_MACRO(TT_WOOD_FLOOR, rm->roff+2, rm->coff+28);
    ASSIGN_TILE_MACRO(TT_WOOD_FLOOR, rm->roff+2, rm->coff+29);
    ASSIGN_TILE_MACRO(TT_WOOD_FLOOR, rm->roff+2, rm->coff+30);
    ASSIGN_TILE_MACRO(TT_WOOD_FLOOR, rm->roff+2, rm->coff+31);
    ASSIGN_TILE_MACRO(TT_WOOD_FLOOR, rm->roff+2, rm->coff+32);
    ASSIGN_TILE_MACRO(TT_WOOD_FLOOR, rm->roff+3, rm->coff+26);
    ASSIGN_TILE_MACRO(TT_WOOD_FLOOR, rm->roff+3, rm->coff+27);
    ASSIGN_TILE_MACRO(TT_WOOD_FLOOR, rm->roff+3, rm->coff+28);
    ASSIGN_TILE_MACRO(TT_WOOD_FLOOR, rm->roff+3, rm->coff+29);
    ASSIGN_TILE_MACRO(TT_WOOD_FLOOR, rm->roff+3, rm->coff+30);
    ASSIGN_TILE_MACRO(TT_WOOD_FLOOR, rm->roff+3, rm->coff+31);
    ASSIGN_TILE_MACRO(TT_WOOD_FLOOR, rm->roff+3, rm->coff+32);
    ASSIGN_TILE_MACRO(TT_WOOD_FLOOR, rm->roff+3, rm->coff+33);
    ASSIGN_TILE_MACRO(TT_WOOD_FLOOR, rm->roff+3, rm->coff+34);
    ASSIGN_TILE_MACRO(TT_WOOD_WALL_HOR, rm->roff+4, rm->coff+22);
    ASSIGN_TILE_MACRO(TT_WOOD_WALL_HOR, rm->roff+4, rm->coff+23);
    ASSIGN_TILE_MACRO(TT_WOOD_WALL_HOR, rm->roff+4, rm->coff+24);
    ASSIGN_TILE_MACRO(TT_WOOD_WALL_SE, rm->roff+4, rm->coff+25);
    ASSIGN_TILE_MACRO(TT_WOOD_WALL_VER, rm->roff+3, rm->coff+25);
    ASSIGN_TILE_MACRO(TT_WOOD_WALL_NW, rm->roff+2, rm->coff+25);
    ASSIGN_TILE_MACRO(TT_WOOD_WALL_HOR, rm->roff+2, rm->coff+26);
    ASSIGN_TILE_MACRO(TT_WOOD_WALL_SE, rm->roff+2, rm->coff+27);
    ASSIGN_TILE_MACRO(TT_WOOD_WALL_NW, rm->roff+1, rm->coff+27);
    ASSIGN_TILE_MACRO(TT_WOOD_WALL_HOR, rm->roff+1, rm->coff+28);
    ASSIGN_TILE_MACRO(TT_WOOD_WALL_SE, rm->roff+1, rm->coff+29);
    ASSIGN_TILE_MACRO(TT_WOOD_WALL_HOR, rm->roff+0, rm->coff+30);
    ASSIGN_TILE_MACRO(TT_WOOD_WALL_NW, rm->roff+0, rm->coff+29);
    ASSIGN_TILE_MACRO(TT_WOOD_WALL_NE, rm->roff+0, rm->coff+31);
    ASSIGN_TILE_MACRO(TT_WOOD_WALL_SW, rm->roff+1, rm->coff+31);
    ASSIGN_TILE_MACRO(TT_WOOD_WALL_HOR, rm->roff+1, rm->coff+32);
    ASSIGN_TILE_MACRO(TT_WOOD_WALL_NE, rm->roff+1, rm->coff+33);
    ASSIGN_TILE_MACRO(TT_WOOD_WALL_SW, rm->roff+2, rm->coff+33);
    ASSIGN_TILE_MACRO(TT_WOOD_WALL_HOR, rm->roff+2, rm->coff+34);
    ASSIGN_TILE_MACRO(TT_WOOD_WALL_NE, rm->roff+2, rm->coff+35);
    ASSIGN_TILE_MACRO(TT_WOOD_WALL_VER, rm->roff+3, rm->coff+35);
    ASSIGN_TILE_MACRO(TT_WOOD_WALL_SW, rm->roff+4, rm->coff+35);
    ASSIGN_TILE_MACRO(TT_WOOD_WALL_HOR, rm->roff+4, rm->coff+36);
    ASSIGN_TILE_MACRO(TT_WOOD_WALL_HOR, rm->roff+4, rm->coff+37);
    ASSIGN_TILE_MACRO(TT_WOOD_WALL_HOR, rm->roff+4, rm->coff+38);

    for(c = rm->coff+1; c < rm->c; ++c) {
        ASSIGN_TILE_MACRO(TT_WOOD_WALL_HOR, rm->r-1, c);
    }
    for(r = rm->roff+4; r < rm->r; ++r) {
        ASSIGN_TILE_MACRO(TT_WOOD_WALL_VER, r, rm->coff+1);
        ASSIGN_TILE_MACRO(TT_WOOD_WALL_VER, r, rm->c-1);
    }
    ASSIGN_TILE_MACRO(TT_WOOD_WALL_NE, rm->roff+4, rm->coff+39);
    
    ASSIGN_TILE_MACRO(TT_WOOD_WALL_SW, rm->r-1, rm->coff+1);
    ASSIGN_TILE_MACRO(TT_WOOD_WALL_SE, rm->r-1, rm->c-1);

    ASSIGN_DOOR_MACRO(TT_WOOD_DOOR_HOR, rm->r-1, rm->roff+30);

    r = rm->roff+4;
    for(c = rm->coff+1; c < rm->coff+22; ++c) {
        ASSIGN_TILE_MACRO(TT_WOOD_WALL_HOR, r, c);
    }
    ASSIGN_TILE_MACRO(TT_WOOD_WALL_NW, r, rm->coff+1);
    c = rm->coff+1;
    for(r = rm->roff+5; r < rm->roff+15; ++r) {
        ASSIGN_TILE_MACRO(TT_WOOD_WALL_VER, r, c);
    }
    
    for(r = rm->roff+5; r < rm->roff+15; ++r) {
        for(c = rm->coff+2; c < rm->coff+22; ++c) {
            ASSIGN_TILE_MACRO(TT_GRASS_FLOOR, r, c);
        }
    }

    r = rm->roff + 15;
    for(c = rm->coff+2; c < rm->coff+23; ++c) {
        ASSIGN_TILE_MACRO(TT_FENCE, r, c);
    }
    --c;
    ASSIGN_TILE_MACRO(TT_FENCE, rm->roff+5, c);
    ASSIGN_TILE_MACRO(TT_FENCE, rm->roff+6, c);
    ASSIGN_TILE_MACRO(TT_FENCE, rm->roff+7, c);
    ASSIGN_TILE_MACRO(TT_FENCE, rm->roff+12, c);
    ASSIGN_TILE_MACRO(TT_FENCE, rm->roff+13, c);
    ASSIGN_TILE_MACRO(TT_FENCE, rm->roff+14, c);

    ASSIGN_TILE_MACRO(TT_BOULDER, rm->roff+5, rm->coff+8);
    ASSIGN_TILE_MACRO(TT_BOULDER, rm->roff+6, rm->coff+8);
    ASSIGN_TILE_MACRO(TT_BOULDER, rm->roff+5, rm->coff+9);
    ASSIGN_TILE_MACRO(TT_BOULDER, rm->roff+6, rm->coff+9);

    ASSIGN_TILE_MACRO(TT_TREE, rm->roff+8, rm->coff+4);
    ASSIGN_TILE_MACRO(TT_TREE, rm->roff+11, rm->coff+7);
    ASSIGN_TILE_MACRO(TT_BOULDER, rm->roff+9, rm->coff+10);
    ASSIGN_TILE_MACRO(TT_BOULDER, rm->roff+9, rm->coff+11);

    ASSIGN_TILE_MACRO(TT_TREE, rm->roff+6, rm->coff+16);
    ASSIGN_TILE_MACRO(TT_BOULDER, rm->roff+6, rm->coff+19);

    ASSIGN_TILE_MACRO(TT_TREE, rm->roff+11, rm->coff+17);

    ASSIGN_TILE_MACRO(TT_BOULDER, rm->roff+9, rm->coff+19);

    ASSIGN_TILE_MACRO(TT_BOULDER, rm->roff+13, rm->coff+15);
    ASSIGN_TILE_MACRO(TT_BOULDER, rm->roff+13, rm->coff+16);
    ASSIGN_TILE_MACRO(TT_BOULDER, rm->roff+14, rm->coff+15);
    ASSIGN_TILE_MACRO(TT_BOULDER, rm->roff+14, rm->coff+16);

    ASSIGN_TILE_MACRO(TT_BOULDER, rm->roff+13, rm->coff+6);
    ASSIGN_TILE_MACRO(TT_BOULDER, rm->roff+13, rm->coff+5);

    ASSIGN_TILE_MACRO(TT_TREE, rm->roff+10, rm->coff+13);
    ASSIGN_TILE_MACRO(TT_TREE, rm->roff+5, rm->coff+7);
    ASSIGN_TILE_MACRO(TT_TREE, rm->roff+13, rm->coff+11);
    ASSIGN_TILE_MACRO(TT_TREE, rm->roff+11, rm->coff+17);
    ASSIGN_TILE_MACRO(TT_TREE, rm->roff+14, rm->coff+2);
    ASSIGN_TILE_MACRO(TT_TREE, rm->roff+7, rm->coff+12);
    ASSIGN_TILE_MACRO(TT_TREE, rm->roff+13, rm->coff+21);

    ASSIGN_TILE_MACRO(TT_BOULDER, rm->roff+9, rm->coff+2);
    ASSIGN_TILE_MACRO(TT_BOULDER, rm->roff+10, rm->coff+2);

    ASSIGN_STAT_MACRO(TS_BLOOD, rm->roff+1, rm->coff+30);
    ASSIGN_STAT_MACRO(TS_BLOOD, rm->roff+2, rm->coff+28);
    ASSIGN_STAT_MACRO(TS_BLOOD, rm->roff+2, rm->coff+29);
    ASSIGN_STAT_MACRO(TS_BLOOD, rm->roff+2, rm->coff+30);
    ASSIGN_STAT_MACRO(TS_BLOOD, rm->roff+2, rm->coff+31);
    ASSIGN_STAT_MACRO(TS_BLOOD, rm->roff+2, rm->coff+32);
    ASSIGN_STAT_MACRO(TS_BLOOD, rm->roff+3, rm->coff+28);
    ASSIGN_STAT_MACRO(TS_BLOOD, rm->roff+3, rm->coff+29);
    ASSIGN_STAT_MACRO(TS_BLOOD, rm->roff+3, rm->coff+33);
    ASSIGN_STAT_MACRO(TS_BLOOD, rm->roff+4, rm->coff+30);
    ASSIGN_STAT_MACRO(TS_BLOOD, rm->roff+4, rm->coff+27);
    ASSIGN_STAT_MACRO(TS_BLOOD, rm->roff+4, rm->coff+33);
    ASSIGN_STAT_MACRO(TS_BLOOD, rm->roff+3, rm->coff+31);
    ASSIGN_STAT_MACRO(TS_BLOOD, rm->roff+3, rm->coff+27);
    ASSIGN_STAT_MACRO(TS_BLOOD, rm->roff+3, rm->coff+34);
    ASSIGN_STAT_MACRO(TS_BLOOD, rm->roff+3, rm->coff+30);

    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+2, rm->coff+30);
    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+1, rm->coff+30);
    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+2, rm->coff+29);
    ASSIGN_STAT_MACRO(TS_CORPSE, rm->roff+3, rm->coff+31);

    rm->entities = NULL;

    push_entity_into_room(rm, create_entity(ET_BEAR), rm->roff+6, rm->coff+7, -1);
    push_entity_into_room(rm, create_entity(ET_BEAR), rm->roff+2, rm->coff+30, -1);
    push_entity_into_room(rm, create_entity(ET_CAPYBARA), rm->roff+6, rm->coff+29, -1);
    push_entity_into_room(rm, create_entity(ET_CAPYBARA), rm->roff+6, rm->coff+30, -1);
    push_entity_into_room(rm, create_entity(ET_CAPYBARA), rm->roff+6, rm->coff+31, -1);

    push_entity_into_room(rm, create_entity(ET_RABBIT), rm->roff+8, rm->coff+25, -1);
    push_entity_into_room(rm, create_entity(ET_RABBIT), rm->roff+8, rm->coff+27, -1);
    push_entity_into_room(rm, create_entity(ET_RABBIT), rm->roff+8, rm->coff+29, -1);
    push_entity_into_room(rm, create_entity(ET_RABBIT), rm->roff+8, rm->coff+31, -1);
    push_entity_into_room(rm, create_entity(ET_RABBIT), rm->roff+8, rm->coff+33, -1);
    push_entity_into_room(rm, create_entity(ET_RABBIT), rm->roff+8, rm->coff+35, -1);

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