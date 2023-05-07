#include "color.h"
#include "tiles.h"

tile tile_template[TILE_NUM];
void (*draw_tile_func[TILE_NUM])(tile *);

void set_tile_template_TT_NULL_func() {
    tile_template[TT_NULL].type = TT_NULL;
    tile_template[TT_NULL].status = 0;
    tile_template[TT_NULL].flags = 0;
}
void draw_tile_func_TT_NULL(tile *tile) {
    return;
}

void set_tile_template_TT_GRASS_FLOOR_func() {
    tile_template[TT_GRASS_FLOOR].type = TT_GRASS_FLOOR;
    tile_template[TT_GRASS_FLOOR].status = 0;
    tile_template[TT_GRASS_FLOOR].flags = TF_PASSABLE | TF_BURNABLE;
}
void draw_tile_func_TT_GRASS_FLOOR(tile *tile) {
    if(tile->status & TS_FIRE) {
        tile->col = colornum(4, 2, true, true);
        setcolor(tile->col);
        mvaddstr(tile->r, tile->c, "❃");
    }
    else {
        tile->col = colornum(6, 2, false, false);
        setcolor(tile->col);
        mvaddstr(tile->r, tile->c, ":");
    }

    unsetcolor(tile->col);
}

void set_tile_template_TT_CAVE_FLOOR_func() {
    tile_template[TT_CAVE_FLOOR].type = TT_CAVE_FLOOR;
    tile_template[TT_CAVE_FLOOR].status = 0;
    tile_template[TT_CAVE_FLOOR].flags = TF_PASSABLE;
}
void draw_tile_func_TT_CAVE_FLOOR(tile *tile) {
    tile->col = colornum(7, 0, false, false);

    setcolor(tile->col);

    mvaddstr(tile->r, tile->c, "▓");

    unsetcolor(tile->col);
}

void set_tile_template_TT_DARK_func() {
    tile_template[TT_DARK].type = TT_DARK;
    tile_template[TT_DARK].status = 0;
    tile_template[TT_DARK].flags = 0;
}
void draw_tile_func_TT_DARK(tile *tile) {
    tile->col = colornum(0, 0, false, false);

    setcolor(tile->col);

    mvaddstr(tile->r, tile->c, " ");

    unsetcolor(tile->col);
}

void set_tile_template_TT_CAVE_WALL_VER_func() {
    tile_template[TT_CAVE_WALL_VER].type = TT_CAVE_WALL_VER;
    tile_template[TT_CAVE_WALL_VER].status = 0;
    tile_template[TT_CAVE_WALL_VER].flags = 0;
}
void draw_tile_func_TT_CAVE_WALL_VER(tile *tile) {
    tile->col = colornum(0, 7, false, false);

    setcolor(tile->col);

    mvaddstr(tile->r, tile->c, "▓");

    unsetcolor(tile->col);
}

void set_tile_template_TT_CAVE_WALL_HOR_func() {
    tile_template[TT_CAVE_WALL_HOR].type = TT_CAVE_WALL_HOR;
    tile_template[TT_CAVE_WALL_HOR].status = 0;
    tile_template[TT_CAVE_WALL_HOR].flags = 0;
}
void draw_tile_func_TT_CAVE_WALL_HOR(tile *tile) {
    tile->col = colornum(0, 7, false, false);

    setcolor(tile->col);

    mvaddstr(tile->r, tile->c, "▓");

    unsetcolor(tile->col);
}

void set_tile_template_TT_WOOD_WALL_VER_func() {
    tile_template[TT_WOOD_WALL_VER].type = TT_WOOD_WALL_VER;
    tile_template[TT_WOOD_WALL_VER].status = 0;
    tile_template[TT_WOOD_WALL_VER].flags = TF_BURNABLE;
}
void draw_tile_func_TT_WOOD_WALL_VER(tile *tile) {
    if(tile->status & TS_FIRE) {
        tile->col = colornum(6, 4, true, true);
    }
    else {
        tile->col = colornum(0, 6, false, false);
    }

    setcolor(tile->col);

    mvaddstr(tile->r, tile->c, "║");

    unsetcolor(tile->col);
}

void set_tile_template_TT_WOOD_WALL_HOR_func() {
    tile_template[TT_WOOD_WALL_HOR].type = TT_WOOD_WALL_HOR;
    tile_template[TT_WOOD_WALL_HOR].status = 0;
    tile_template[TT_WOOD_WALL_HOR].flags = TF_BURNABLE;
}
void draw_tile_func_TT_WOOD_WALL_HOR(tile *tile) {
    if(tile->status & TS_FIRE) {
        tile->col = colornum(6, 4, true, true);
    }
    else {
        tile->col = colornum(0, 6, false, false);
    }

    setcolor(tile->col);

    mvaddstr(tile->r, tile->c, "═");

    unsetcolor(tile->col);
}

#define INIT_TILE_MACRO(NAME) set_tile_template_ ## NAME ## _func(); draw_tile_func[NAME] = draw_tile_func_ ## NAME;

void init_tiles() {
    INIT_TILE_MACRO(TT_NULL)
    INIT_TILE_MACRO(TT_GRASS_FLOOR)
    INIT_TILE_MACRO(TT_CAVE_FLOOR)
    INIT_TILE_MACRO(TT_DARK)
    INIT_TILE_MACRO(TT_CAVE_WALL_VER)
    INIT_TILE_MACRO(TT_CAVE_WALL_HOR)
    INIT_TILE_MACRO(TT_WOOD_WALL_VER)
    INIT_TILE_MACRO(TT_WOOD_WALL_HOR)
}

void draw_tile(tile *tile) {
    draw_tile_func[tile->type](tile);
}

tile get_tile_template(tile_type type) {
    return tile_template[type];
}