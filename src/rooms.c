#include <ncurses.h>
#include <stdlib.h>
#include "rooms.h"
#include "entity.h"

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
        rm->map[r][c] = get_tile_template(TT_CAVE_WALL_HOR);
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
        rm->map[r][c] = get_tile_template(TT_CAVE_WALL_VER);
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

    rm->entities = malloc(sizeof(entity *) * 2);
    rm->entities[0] = malloc(sizeof(entity));
    *(rm->entities[0]) = get_entity_template(ET_CARROT);
    rm->entities[0]->r = 3;
    rm->entities[0]->c = 4;
    rm->map[3][4].entity_id = 0;

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
    entity *ptr = rm->entities[0];

    for(r = 0; r < rm->r; ++r) {
        free_tile(rm->map[r]);
    }
    while(ptr) {
        if(ptr->type == ET_CARROT)
            continue;
        free_entity(ptr);
        ++ptr;
    }

    free(rm->map);
    free(rm);
}