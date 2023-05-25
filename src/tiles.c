#include "color.h"
#include "tiles.h"
#include "floor.h"

tile tile_template[TILE_NUM];
void (*draw_tile_func[TILE_NUM])(tile *);

void set_tile_template(tile_type tp) {
    tile_template[tp].entity_id = -1;
    tile_template[tp].player_id = -1;
    tile_template[tp].door_id = -1;
    tile_template[tp].status = 0;
    tile_template[tp].flags = 0;
}

void set_tile_template_TT_NULL_func() {
    tile_template[TT_NULL].type = TT_NULL;
}
void draw_tile_func_TT_NULL(tile *tile) {
    return;
}

void set_tile_template_TT_GRASS_FLOOR_func() {
    tile_template[TT_GRASS_FLOOR].type = TT_GRASS_FLOOR;
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
    tile_template[TT_CAVE_FLOOR].flags = TF_PASSABLE;
}
void draw_tile_func_TT_CAVE_FLOOR(tile *tile) {
    if(tile->status & TS_BLOOD) {
        tile->col = colornum(4, 7, true, false);
        setcolor(tile->col);
        mvaddstr(tile->r, tile->c, "%");
    }
    else {
        tile->col = colornum(7, 0, false, false);
        setcolor(tile->col);
        mvaddstr(tile->r, tile->c, "▓");
    }

    unsetcolor(tile->col);
}

void set_tile_template_TT_DARK_func() {
    tile_template[TT_DARK].type = TT_DARK;
}
void draw_tile_func_TT_DARK(tile *tile) {
    tile->col = colornum(0, 0, false, false);

    setcolor(tile->col);

    mvaddstr(tile->r, tile->c, " ");

    unsetcolor(tile->col);
}

void set_tile_template_TT_CAVE_WALL_func() {
    tile_template[TT_CAVE_WALL].type = TT_CAVE_WALL;
}
void draw_tile_func_TT_CAVE_WALL(tile *tile) {
    tile->col = colornum(0, 7, false, false);

    setcolor(tile->col);

    mvaddstr(tile->r, tile->c, "▓");

    unsetcolor(tile->col);
}

void set_tile_template_TT_WOOD_WALL_VER_func() {
    tile_template[TT_WOOD_WALL_VER].type = TT_WOOD_WALL_VER;
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

void set_tile_template_TT_WOOD_WALL_NE_func() {
    tile_template[TT_WOOD_WALL_NE].type = TT_WOOD_WALL_NE;
    tile_template[TT_WOOD_WALL_NE].flags = TF_BURNABLE;
}
void draw_tile_func_TT_WOOD_WALL_NE(tile *tile) {
    if(tile->status & TS_FIRE) {
        tile->col = colornum(6, 4, true, true);
    }
    else {
        tile->col = colornum(0, 6, false, false);
    }

    setcolor(tile->col);

    mvaddstr(tile->r, tile->c, "╗");

    unsetcolor(tile->col);
}

void set_tile_template_TT_WOOD_WALL_SE_func() {
    tile_template[TT_WOOD_WALL_SE].type = TT_WOOD_WALL_SE;
    tile_template[TT_WOOD_WALL_SE].flags = TF_BURNABLE;
}
void draw_tile_func_TT_WOOD_WALL_SE(tile *tile) {
    if(tile->status & TS_FIRE) {
        tile->col = colornum(6, 4, true, true);
    }
    else {
        tile->col = colornum(0, 6, false, false);
    }

    setcolor(tile->col);

    mvaddstr(tile->r, tile->c, "╝");

    unsetcolor(tile->col);
}

void set_tile_template_TT_WOOD_WALL_SW_func() {
    tile_template[TT_WOOD_WALL_SW].type = TT_WOOD_WALL_SW;
    tile_template[TT_WOOD_WALL_SW].flags = TF_BURNABLE;
}
void draw_tile_func_TT_WOOD_WALL_SW(tile *tile) {
    if(tile->status & TS_FIRE) {
        tile->col = colornum(6, 4, true, true);
    }
    else {
        tile->col = colornum(0, 6, false, false);
    }

    setcolor(tile->col);

    mvaddstr(tile->r, tile->c, "╚");

    unsetcolor(tile->col);
}

void set_tile_template_TT_WOOD_WALL_NW_func() {
    tile_template[TT_WOOD_WALL_NW].type = TT_WOOD_WALL_NW;
    tile_template[TT_WOOD_WALL_NW].flags = TF_BURNABLE;
}
void draw_tile_func_TT_WOOD_WALL_NW(tile *tile) {
    if(tile->status & TS_FIRE) {
        tile->col = colornum(6, 4, true, true);
    }
    else {
        tile->col = colornum(0, 6, false, false);
    }

    setcolor(tile->col);

    mvaddstr(tile->r, tile->c, "╔");

    unsetcolor(tile->col);
}

void set_tile_template_TT_WOOD_FLOOR_func() {
    tile_template[TT_WOOD_FLOOR].type = TT_WOOD_FLOOR;
    tile_template[TT_WOOD_FLOOR].flags = TF_PASSABLE;
}
void draw_tile_func_TT_WOOD_FLOOR(tile *tile) {
    tile->col = colornum(4, 6, false, false);
    setcolor(tile->col);

    if(tile->status & TS_BLOOD) {
        mvaddstr(tile->r, tile->c, "▓");
    }
    else {
        mvaddstr(tile->r, tile->c, "░");
    }

    unsetcolor(tile->col);
}

void set_tile_template_TT_WOOD_TABLE_func() {
    tile_template[TT_WOOD_TABLE].type = TT_WOOD_TABLE;
}
void draw_tile_func_TT_WOOD_TABLE(tile *tile) {
    tile->col = colornum(6, 0, false, false);
    setcolor(tile->col);

    mvaddstr(tile->r, tile->c, "#");

    unsetcolor(tile->col);
}

void set_tile_template_TT_WOOD_DOOR_VER_func() {
    tile_template[TT_WOOD_DOOR_VER].type = TT_WOOD_DOOR_VER;
    tile_template[TT_WOOD_DOOR_VER].flags |= TF_DOOR;
}
void draw_tile_func_TT_WOOD_DOOR_VER(tile *tile) {
    tile->col = colornum(0, 6, false, false);
    setcolor(tile->col);

    mvaddstr(tile->r, tile->c, "╏");

    unsetcolor(tile->col);
}

void set_tile_template_TT_WOOD_DOOR_HOR_func() {
    tile_template[TT_WOOD_DOOR_HOR].type = TT_WOOD_DOOR_HOR;
    tile_template[TT_WOOD_DOOR_HOR].flags |= TF_DOOR;
}
void draw_tile_func_TT_WOOD_DOOR_HOR(tile *tile) {
    tile->col = colornum(0, 6, false, false);
    setcolor(tile->col);

    mvaddstr(tile->r, tile->c, "╍");

    unsetcolor(tile->col);
}

#define INIT_TILE_MACRO(NAME) set_tile_template(NAME); set_tile_template_ ## NAME ## _func(); draw_tile_func[NAME] = draw_tile_func_ ## NAME;

void init_tiles() {
    INIT_TILE_MACRO(TT_NULL)
    INIT_TILE_MACRO(TT_GRASS_FLOOR)
    INIT_TILE_MACRO(TT_CAVE_FLOOR)
    INIT_TILE_MACRO(TT_DARK)
    INIT_TILE_MACRO(TT_CAVE_WALL)
    INIT_TILE_MACRO(TT_WOOD_WALL_VER)
    INIT_TILE_MACRO(TT_WOOD_WALL_HOR)
    INIT_TILE_MACRO(TT_WOOD_WALL_NE)
    INIT_TILE_MACRO(TT_WOOD_WALL_SE)
    INIT_TILE_MACRO(TT_WOOD_WALL_SW)
    INIT_TILE_MACRO(TT_WOOD_WALL_NW)
    INIT_TILE_MACRO(TT_WOOD_FLOOR)
    INIT_TILE_MACRO(TT_WOOD_TABLE)
    INIT_TILE_MACRO(TT_WOOD_DOOR_VER)
    INIT_TILE_MACRO(TT_WOOD_DOOR_HOR)
}

void draw_tile(tile *tile) {
    draw_tile_func[tile->type](tile);
}

tile get_tile_template(tile_type type) {
    return tile_template[type];
}

void free_tile(tile *tile) {
    free(tile);
}

tile *get_tile_at(int r, int c) {
    room *rm = get_cur_room();
    if(!is_within_bound(r, c)) return NULL;
    return rm->map[r] + c;
}

int is_within_bound(int row, int col) {
    room *rm = get_cur_room();
    if(row < 0 || row >= rm->r || col < 0 || col >= rm->c) return 0;
    return 1;
}

//현재 방의 row행 col열이 이동 가능한 곳이면 1, 아니면 0 반환
int is_passable(int row, int col){
    room *rm = get_cur_room();
    if(!is_within_bound(row, col)) return 0;
    return is_passable_tile(rm->map[row] + col);
}
int is_passable_tile(tile *t) {
    return t->flags & TF_PASSABLE;
}

//현재 방의 row행 col열이 불이 붙을 수 있는 타일이면 1, 아니면 0 반환
int is_burnable(int row, int col){
    room *rm = get_cur_room();
    if(!is_within_bound(row, col)) return 0;
    return is_burnable_tile(rm->map[row] + col);
}
int is_burnable_tile(tile *t){
    return t->flags & TF_BURNABLE;
}

int is_door(int row, int col) {
    room *rm = get_cur_room();
    if(!is_within_bound(row, col)) return 0;
    return is_door_tile(rm->map[row] + col);
}
int is_door_tile(tile *t) {
    room *rm = get_cur_room();
    return (t->flags & TF_DOOR) && rm->doors[t->door_id].next;
}