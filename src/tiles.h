#ifndef TILE_HEADER
#define TILE_HEADER

#include <stdint.h>
#include <stdlib.h>

typedef enum tile_type {
    TT_NULL=0,
    TT_GRASS_FLOOR,
    TT_CAVE_FLOOR,
    TT_DARK,
    TT_CAVE_WALL,
    TT_WOOD_WALL_VER,
    TT_WOOD_WALL_HOR,
    TT_WOOD_WALL_NE,
    TT_WOOD_WALL_SE,
    TT_WOOD_WALL_SW,
    TT_WOOD_WALL_NW,
    TT_WOOD_FLOOR,
    TT_WOOD_TABLE,
    TT_WOOD_DOOR_VER,
    TT_WOOD_DOOR_HOR,
    TT_RED_CARPET,
    TT_GREEN_CARPET,
    TILE_NUM,
} tile_type;

typedef enum tile_status {
    TS_FIRE = 1 << 0,
    TS_BLOOD = 1 << 1,
} tile_status;

typedef enum tile_flag {
    TF_PASSABLE = 1 << 0,
    TF_BURNABLE = 1 << 1,
    TF_DOOR = 1 << 2,
} tile_flag;

typedef struct tile {
    int r, c;
    int fg, bg, blink;
    int col;
    tile_type type;
    uint64_t status;
    uint64_t flags;
    int entity_id;
    int player_id;
    int door_id;
    char *name;
    //status stat[10];
    //pile *pile;
} tile;

void init_tiles();

void draw_tile(tile *tile);

tile get_tile_template(tile_type type);

void free_tile(tile *tile);

tile *get_tile_at(int r, int c);

int is_within_bound(int row, int col);

int is_passable(int row, int col);
int is_passable_tile(tile *t);

int is_burnable(int row, int col);
int is_burnable_tile(tile *t);

int is_door(int row, int col);
int is_door_tile(tile *t);

#endif