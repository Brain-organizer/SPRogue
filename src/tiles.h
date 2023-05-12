#ifndef TILE_HEADER
#define TILE_HEADER

#include <stdint.h>
#include <stdlib.h>

#define TILE_NUM 8

typedef enum tile_type {
    TT_NULL,
    TT_GRASS_FLOOR,
    TT_CAVE_FLOOR,
    TT_DARK,
    TT_CAVE_WALL_VER,
    TT_CAVE_WALL_HOR,
    TT_WOOD_WALL_VER,
    TT_WOOD_WALL_HOR,
} tile_type;

typedef enum tile_status {
    TS_FIRE = 1 << 0,
} tile_status;

typedef enum tile_flag {
    TF_PASSABLE = 1 << 0,
    TF_BURNABLE = 1 << 1,
} tile_flag;

typedef struct tile {
    int r, c;
    int col;
    tile_type type;
    uint64_t status;
    uint64_t flags;
    int entity_id;
    int player_id;
    //status stat[10];
    //pile *pile;
} tile;

void init_tiles();

void draw_tile(tile *tile);

tile get_tile_template(tile_type type);

void free_tile(tile *tile);

#endif