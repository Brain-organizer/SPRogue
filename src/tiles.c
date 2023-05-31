#include "color.h"
#include "tiles.h"
#include "floor.h"

tile tile_template[TILE_NUM];
void (*draw_tile_func[TILE_NUM])(tile *);

void set_tile_template(tile_type tp) {
    tile_template[tp].entity_id = -1;
    tile_template[tp].door_id = -1;
    tile_template[tp].status = 0;
    tile_template[tp].flags = 0;
    tile_template[tp].fg = tile_template[tp].bg = -1;
    tile_template[tp].blink = 0;
    tile_template[tp].name = "";
    tile_template[tp].desc = "";
}

void set_tile_template_TT_NULL_func() {
    tile_template[TT_NULL].type = TT_NULL;
}
void draw_tile_func_TT_NULL(tile *tile) {
    return;
}

void set_tile_template_TT_GRASS_FLOOR_func() {
    tile_template[TT_GRASS_FLOOR].type = TT_GRASS_FLOOR;
    tile_template[TT_GRASS_FLOOR].flags = TF_PASSABLE;
    tile_template[TT_GRASS_FLOOR].name = "Grass Floor";
    tile_template[TT_GRASS_FLOOR].desc = "Soft green grass is growing atop a flat gathering of dirt.";
}
void draw_tile_func_TT_GRASS_FLOOR(tile *tile) {
    if(tile->fg > 0) unget_color_id(tile->fg);
    if(tile->bg > 0) unget_color_id(tile->bg);

    tile->bg = get_color_id(184,147,92);

    if(tile->status & TS_BOMB) {
        tile->fg = get_color_id(80, 80, 200);
        tile->col = PAIR_COLOR(tile->fg, tile->bg);

        SET_COLOR(tile->col);
        mvaddstr(tile->r, tile->c, "o");
    }
    else if(tile->status & TS_PEER) {
        tile->fg = get_color_id(80, 80, 200);
        tile->col = PAIR_COLOR(tile->fg, tile->bg);

        SET_COLOR(tile->col);
        mvaddstr(tile->r, tile->c, "y");
    }
    else if(tile->status & TS_HEALTH) {
        tile->fg = get_color_id(140, 0, 0);
        tile->col = PAIR_COLOR(tile->fg, tile->bg);

        SET_COLOR(tile->col);
        mvaddstr(tile->r, tile->c, "&");
    }
    else if(tile->status & TS_BLOOD) {
        tile->fg = get_color_id(140, 0, 0);
        tile->col = PAIR_COLOR(tile->fg, tile->bg);

        SET_COLOR(tile->col);
        mvaddstr(tile->r, tile->c, ":");
    }
    else {
        tile->fg = get_color_id(0,110,51);
        tile->col = PAIR_COLOR(tile->fg, tile->bg);
        
        SET_COLOR(tile->col);
        mvaddstr(tile->r, tile->c, ":");
    }

    UNSET_COLOR(tile->col);
}

void set_tile_template_TT_CAVE_FLOOR_func() {
    tile_template[TT_CAVE_FLOOR].type = TT_CAVE_FLOOR;
    tile_template[TT_CAVE_FLOOR].flags = TF_PASSABLE;
    tile_template[TT_CAVE_FLOOR].name = "Cave Floor";
    tile_template[TT_CAVE_FLOOR].desc = "Moisture has drawn in moss and an assortment of crawling critters.";
}
void draw_tile_func_TT_CAVE_FLOOR(tile *tile) {
    if(tile->fg > 0) unget_color_id(tile->fg);
    if(tile->bg > 0) unget_color_id(tile->bg);

    tile->bg = get_color_id(200, 200, 200);

    if(tile->status & TS_BOMB) {
        tile->fg = get_color_id(80, 80, 200);
        tile->col = PAIR_COLOR(tile->fg, tile->bg);

        SET_COLOR(tile->col);
        mvaddstr(tile->r, tile->c, "o");
    }
    else if(tile->status & TS_PEER) {
        tile->fg = get_color_id(80, 80, 200);
        tile->col = PAIR_COLOR(tile->fg, tile->bg);

        SET_COLOR(tile->col);
        mvaddstr(tile->r, tile->c, "y");
    }
    else if(tile->status & TS_HEALTH) {
        tile->fg = get_color_id(140, 0, 0);
        tile->col = PAIR_COLOR(tile->fg, tile->bg);

        SET_COLOR(tile->col);
        mvaddstr(tile->r, tile->c, "&");
    }
    else if(tile->status & TS_BLOOD) {
        tile->fg = get_color_id(140, 0, 0);
        tile->col = PAIR_COLOR(tile->fg, tile->bg);

        SET_COLOR(tile->col);
        mvaddstr(tile->r, tile->c, "%");
    }
    else {
        tile->fg = 0;
        tile->col = PAIR_COLOR(tile->fg, tile->bg);
        
        SET_COLOR(tile->col);
        mvaddstr(tile->r, tile->c, " ");
    }
    
    UNSET_COLOR(tile->col);
}

void set_tile_template_TT_DARK_func() {
    tile_template[TT_DARK].type = TT_DARK;
    tile_template[TT_DARK].name = "Darkness";
    tile_template[TT_DARK].desc = "It is dark. There is nothing here.";
}
void draw_tile_func_TT_DARK(tile *tile) {
    if(tile->fg > 0) unget_color_id(tile->fg);
    if(tile->bg > 0) unget_color_id(tile->bg);

    tile->fg = tile->bg = 0;
    tile->col = PAIR_COLOR(tile->fg, tile->bg);
    
    SET_COLOR(tile->col);
    mvaddstr(tile->r, tile->c, " ");
    UNSET_COLOR(tile->col);
}

void set_tile_template_TT_CAVE_WALL_func() {
    tile_template[TT_CAVE_WALL].type = TT_CAVE_WALL;
    tile_template[TT_CAVE_WALL].name = "Cave Wall";
    tile_template[TT_CAVE_WALL].desc = "Moisture has drawn in moss and an assortment of crawling critters.";
}
void draw_tile_func_TT_CAVE_WALL(tile *tile) {
    if(tile->fg > 0) unget_color_id(tile->fg);
    if(tile->bg > 0) unget_color_id(tile->bg);

    tile->fg = 0;
    tile->bg = get_color_id(120, 120, 120);
    tile->col = PAIR_COLOR(tile->fg, tile->bg);

    SET_COLOR(tile->col);
    mvaddstr(tile->r, tile->c, " ");
    UNSET_COLOR(tile->col);
}

void set_tile_template_TT_WOOD_WALL_VER_func() {
    tile_template[TT_WOOD_WALL_VER].type = TT_WOOD_WALL_VER;
    tile_template[TT_WOOD_WALL_VER].flags = TF_BURNABLE;
    tile_template[TT_WOOD_WALL_VER].name = "Wooden Wall";
    tile_template[TT_WOOD_WALL_VER].desc = "Rotting timber is beginning to crack under its own weight, revealing dark crevices within.";
}
void draw_tile_func_TT_WOOD_WALL_VER(tile *tile) {
    if(tile->fg > 0) unget_color_id(tile->fg);
    if(tile->bg > 0) unget_color_id(tile->bg);

    tile->fg = 0;
    tile->bg = get_color_id(133,94,66);
    tile->col = PAIR_COLOR(tile->fg, tile->bg);

    SET_COLOR(tile->col);
    mvaddstr(tile->r, tile->c, "║");
    UNSET_COLOR(tile->col);
}

void set_tile_template_TT_WOOD_WALL_HOR_func() {
    tile_template[TT_WOOD_WALL_HOR].type = TT_WOOD_WALL_HOR;
    tile_template[TT_WOOD_WALL_HOR].flags = TF_BURNABLE;
    tile_template[TT_WOOD_WALL_HOR].name = "Wooden Wall";
    tile_template[TT_WOOD_WALL_HOR].desc = "Rotting timber is beginning to crack under its own weight, revealing dark crevices within.";
}
void draw_tile_func_TT_WOOD_WALL_HOR(tile *tile) {
    if(tile->fg > 0) unget_color_id(tile->fg);
    if(tile->bg > 0) unget_color_id(tile->bg);

    tile->fg = 0;
    tile->bg = get_color_id(133,94,66);
    tile->col = PAIR_COLOR(tile->fg, tile->bg);

    SET_COLOR(tile->col);
    mvaddstr(tile->r, tile->c, "═");
    UNSET_COLOR(tile->col);
}

void set_tile_template_TT_WOOD_WALL_NE_func() {
    tile_template[TT_WOOD_WALL_NE].type = TT_WOOD_WALL_NE;
    tile_template[TT_WOOD_WALL_NE].flags = TF_BURNABLE;
    tile_template[TT_WOOD_WALL_NE].name = "Wooden Wall";
    tile_template[TT_WOOD_WALL_NE].desc = "Rotting timber is beginning to crack under its own weight, revealing dark crevices within.";
}
void draw_tile_func_TT_WOOD_WALL_NE(tile *tile) {
    if(tile->fg > 0) unget_color_id(tile->fg);
    if(tile->bg > 0) unget_color_id(tile->bg);

    tile->fg = 0;
    tile->bg = get_color_id(133,94,66);
    tile->col = PAIR_COLOR(tile->fg, tile->bg);

    SET_COLOR(tile->col);
    mvaddstr(tile->r, tile->c, "╗");
    UNSET_COLOR(tile->col);
}

void set_tile_template_TT_WOOD_WALL_SE_func() {
    tile_template[TT_WOOD_WALL_SE].type = TT_WOOD_WALL_SE;
    tile_template[TT_WOOD_WALL_SE].flags = TF_BURNABLE;
    tile_template[TT_WOOD_WALL_SE].name = "Wooden Wall";
    tile_template[TT_WOOD_WALL_SE].desc = "Rotting timber is beginning to crack under its own weight, revealing dark crevices within.";
}
void draw_tile_func_TT_WOOD_WALL_SE(tile *tile) {
    if(tile->fg > 0) unget_color_id(tile->fg);
    if(tile->bg > 0) unget_color_id(tile->bg);

    tile->fg = 0;
    tile->bg = get_color_id(133,94,66);
    tile->col = PAIR_COLOR(tile->fg, tile->bg);

    SET_COLOR(tile->col);
    mvaddstr(tile->r, tile->c, "╝");
    UNSET_COLOR(tile->col);
}

void set_tile_template_TT_WOOD_WALL_SW_func() {
    tile_template[TT_WOOD_WALL_SW].type = TT_WOOD_WALL_SW;
    tile_template[TT_WOOD_WALL_SW].flags = TF_BURNABLE;
    tile_template[TT_WOOD_WALL_SW].name = "Wooden Wall";
    tile_template[TT_WOOD_WALL_SW].desc = "Rotting timber is beginning to crack under its own weight, revealing dark crevices within.";
}
void draw_tile_func_TT_WOOD_WALL_SW(tile *tile) {
    if(tile->fg > 0) unget_color_id(tile->fg);
    if(tile->bg > 0) unget_color_id(tile->bg);

    tile->fg = 0;
    tile->bg = get_color_id(133,94,66);
    tile->col = PAIR_COLOR(tile->fg, tile->bg);

    SET_COLOR(tile->col);
    mvaddstr(tile->r, tile->c, "╚");
    UNSET_COLOR(tile->col);
}

void set_tile_template_TT_WOOD_WALL_NW_func() {
    tile_template[TT_WOOD_WALL_NW].type = TT_WOOD_WALL_NW;
    tile_template[TT_WOOD_WALL_NW].flags = TF_BURNABLE;
    tile_template[TT_WOOD_WALL_NW].name = "Wooden Wall";
    tile_template[TT_WOOD_WALL_NW].desc = "Rotting timber is beginning to crack under its own weight, revealing dark crevices within.";
}
void draw_tile_func_TT_WOOD_WALL_NW(tile *tile) {
    if(tile->fg > 0) unget_color_id(tile->fg);
    if(tile->bg > 0) unget_color_id(tile->bg);

    tile->fg = 0;
    tile->bg = get_color_id(133,94,66);
    tile->col = PAIR_COLOR(tile->fg, tile->bg);

    SET_COLOR(tile->col);
    mvaddstr(tile->r, tile->c, "╔");
    UNSET_COLOR(tile->col);
}

void set_tile_template_TT_WOOD_FLOOR_func() {
    tile_template[TT_WOOD_FLOOR].type = TT_WOOD_FLOOR;
    tile_template[TT_WOOD_FLOOR].flags = TF_PASSABLE;
    tile_template[TT_WOOD_FLOOR].name = "Wooden Floor";
    tile_template[TT_WOOD_FLOOR].desc = "Rotting timber is beginning to crack under its own weight, revealing dark crevices within.";
}
void draw_tile_func_TT_WOOD_FLOOR(tile *tile) {
    if(tile->fg > 0) unget_color_id(tile->fg);
    if(tile->bg > 0) unget_color_id(tile->bg);

    tile->bg = get_color_id(186,140,99);

    if(tile->status & TS_BOMB) {
        tile->fg = get_color_id(80, 80, 200);
        tile->col = PAIR_COLOR(tile->fg, tile->bg);

        SET_COLOR(tile->col);
        mvaddstr(tile->r, tile->c, "o");
    }
    else if(tile->status & TS_PEER) {
        tile->fg = get_color_id(80, 80, 200);
        tile->col = PAIR_COLOR(tile->fg, tile->bg);

        SET_COLOR(tile->col);
        mvaddstr(tile->r, tile->c, "y");
    }
    else if(tile->status & TS_HEALTH) {
        tile->fg = get_color_id(140, 0, 0);
        tile->col = PAIR_COLOR(tile->fg, tile->bg);

        SET_COLOR(tile->col);
        mvaddstr(tile->r, tile->c, "&");
    }
    else if(tile->status & TS_BLOOD) {
        tile->fg = get_color_id(140, 0, 0);
        tile->col = PAIR_COLOR(tile->fg, tile->bg);

        SET_COLOR(tile->col);
        mvaddstr(tile->r, tile->c, "%");
    }
    else {
        tile->fg = 0;
        tile->col = PAIR_COLOR(tile->fg, tile->bg);
        
        SET_COLOR(tile->col);
        mvaddstr(tile->r, tile->c, " ");
    }

    UNSET_COLOR(tile->col);
}

void set_tile_template_TT_WOOD_TABLE_func() {
    tile_template[TT_WOOD_TABLE].type = TT_WOOD_TABLE;
    tile_template[TT_WOOD_TABLE].name = "Wooden Table";
    tile_template[TT_WOOD_TABLE].desc = "Made of poor craftsmanship, one must doubt how much longer it will hold its shape.";
}
void draw_tile_func_TT_WOOD_TABLE(tile *tile) {
    if(tile->fg > 0) unget_color_id(tile->fg);
    if(tile->bg > 0) unget_color_id(tile->bg);

    tile->fg = get_color_id(133,94,66);
    tile->bg = get_color_id(81,65,53);
    tile->col = PAIR_COLOR(tile->fg, tile->bg);

    SET_COLOR(tile->col);
    mvaddstr(tile->r, tile->c, "#");
    UNSET_COLOR(tile->col);
}

void set_tile_template_TT_WOOD_DOOR_VER_func() {
    tile_template[TT_WOOD_DOOR_VER].type = TT_WOOD_DOOR_VER;
    tile_template[TT_WOOD_DOOR_VER].flags |= TF_DOOR;
    tile_template[TT_WOOD_DOOR_VER].name = "Wooden Door";
    tile_template[TT_WOOD_DOOR_VER].desc = "Thick dark oak is held together by black cast-iron hinge, hiding what lies beyond its doors from your eyes. Or, perhaps, it's the other way around?";
}
void draw_tile_func_TT_WOOD_DOOR_VER(tile *tile) {
    if(tile->fg > 0) unget_color_id(tile->fg);
    if(tile->bg > 0) unget_color_id(tile->bg);

    tile->fg = 0;
    tile->bg = get_color_id(133,94,66);
    tile->col = PAIR_COLOR(tile->fg, tile->bg);

    SET_COLOR(tile->col);
    mvaddstr(tile->r, tile->c, "╏");
    UNSET_COLOR(tile->col);
}

void set_tile_template_TT_WOOD_DOOR_HOR_func() {
    tile_template[TT_WOOD_DOOR_HOR].type = TT_WOOD_DOOR_HOR;
    tile_template[TT_WOOD_DOOR_HOR].flags |= TF_DOOR;
    tile_template[TT_WOOD_DOOR_HOR].name = "Wooden Door";
    tile_template[TT_WOOD_DOOR_HOR].desc = "Thick dark oak is held together by black cast-iron hinge, hiding what lies beyond its doors from your eyes. Or, perhaps, it's the other way around?";
}
void draw_tile_func_TT_WOOD_DOOR_HOR(tile *tile) {
    if(tile->fg > 0) unget_color_id(tile->fg);
    if(tile->bg > 0) unget_color_id(tile->bg);

    tile->fg = 0;
    tile->bg = get_color_id(133,94,66);
    tile->col = PAIR_COLOR(tile->fg, tile->bg);

    SET_COLOR(tile->col);
    mvaddstr(tile->r, tile->c, "╍");
    UNSET_COLOR(tile->col);
}

void set_tile_template_TT_RED_CARPET_func() {
    tile_template[TT_RED_CARPET].type = TT_RED_CARPET;
    tile_template[TT_RED_CARPET].flags |= TF_PASSABLE;
    tile_template[TT_RED_CARPET].name = "Red Carpet";
    tile_template[TT_RED_CARPET].desc = "The carpet is made of red velvet. It glistens with life under the light. It feels decadent to touch.";
}
void draw_tile_func_TT_RED_CARPET(tile *tile) {
    if(tile->fg > 0) unget_color_id(tile->fg);
    if(tile->bg > 0) unget_color_id(tile->bg);

    tile->fg = 0;
    tile->bg = get_color_id(160, 0, 0);
    tile->col = PAIR_COLOR(tile->fg, tile->bg);

    SET_COLOR(tile->col);
    mvaddstr(tile->r, tile->c, " ");
    UNSET_COLOR(tile->col);
}

void set_tile_template_TT_GREEN_CARPET_func() {
    tile_template[TT_GREEN_CARPET].type = TT_GREEN_CARPET;
    tile_template[TT_GREEN_CARPET].flags |= TF_DOOR;
    tile_template[TT_GREEN_CARPET].name = "Green Carpet";
    tile_template[TT_GREEN_CARPET].desc = "The carpet is made of green-dyed velvet. It glistens with life under the light. It feels decadent to touch.";
}
void draw_tile_func_TT_GREEN_CARPET(tile *tile) {
    if(tile->fg > 0) unget_color_id(tile->fg);
    if(tile->bg > 0) unget_color_id(tile->bg);

    tile->fg = 0;
    tile->bg = get_color_id(0, 128, 0);
    tile->col = PAIR_COLOR(tile->fg, tile->bg);

    SET_COLOR(tile->col);
    mvaddstr(tile->r, tile->c, " ");
    UNSET_COLOR(tile->col);
}

void set_tile_template_TT_DIRT_FLOOR_func() {
    tile_template[TT_DIRT_FLOOR].type = TT_DIRT_FLOOR;
    tile_template[TT_DIRT_FLOOR].flags |= TF_PASSABLE;
    tile_template[TT_DIRT_FLOOR].name = "Dirt Floor";
    tile_template[TT_DIRT_FLOOR].desc = "A flat mound of dirt.";
}
void draw_tile_func_TT_DIRT_FLOOR(tile *tile) {
    if(tile->fg > 0) unget_color_id(tile->fg);
    if(tile->bg > 0) unget_color_id(tile->bg);

    tile->bg = get_color_id(184,147,92);

    if(tile->status & TS_BOMB) {
        tile->fg = get_color_id(80, 80, 200);
        tile->col = PAIR_COLOR(tile->fg, tile->bg);

        SET_COLOR(tile->col);
        mvaddstr(tile->r, tile->c, "o");
    }
    else if(tile->status & TS_PEER) {
        tile->fg = get_color_id(80, 80, 200);
        tile->col = PAIR_COLOR(tile->fg, tile->bg);

        SET_COLOR(tile->col);
        mvaddstr(tile->r, tile->c, "y");
    }
    else if(tile->status & TS_HEALTH) {
        tile->fg = get_color_id(140, 0, 0);
        tile->col = PAIR_COLOR(tile->fg, tile->bg);

        SET_COLOR(tile->col);
        mvaddstr(tile->r, tile->c, "&");
    }
    else if(tile->status & TS_BLOOD) {
        tile->fg = get_color_id(140, 0, 0);
        tile->col = PAIR_COLOR(tile->fg, tile->bg);

        SET_COLOR(tile->col);
        mvaddstr(tile->r, tile->c, "%");
    }
    else {
        tile->fg = 0;
        tile->col = PAIR_COLOR(tile->fg, tile->bg);
        
        SET_COLOR(tile->col);
        mvaddstr(tile->r, tile->c, ".");
    }

    UNSET_COLOR(tile->col);
}

void set_tile_template_TT_TREE_func() {
    tile_template[TT_TREE].type = TT_TREE;
    tile_template[TT_TREE].name = "Tree";
    tile_template[TT_TREE].desc = "A large oak tree is growing atop a flat mound of dirt";
}
void draw_tile_func_TT_TREE(tile *tile) {
    if(tile->fg > 0) unget_color_id(tile->fg);
    if(tile->bg > 0) unget_color_id(tile->bg);

    tile->bg = get_color_id(184,147,92);
    tile->fg = get_color_id(0,110,51);
    tile->col = PAIR_COLOR(tile->fg, tile->bg);

    SET_COLOR(tile->col);
    mvaddstr(tile->r, tile->c, "¥");
    UNSET_COLOR(tile->col);
}

void set_tile_template_TT_FENCE_func() {
    tile_template[TT_FENCE].type = TT_FENCE;
    tile_template[TT_FENCE].name = "Wooden Fence";
    tile_template[TT_FENCE].desc = "A set of wooden stakes are held together by sturdy planks. It'd be difficult to pass through it.";
}
void draw_tile_func_TT_FENCE(tile *tile) {
    if(tile->fg > 0) unget_color_id(tile->fg);
    if(tile->bg > 0) unget_color_id(tile->bg);

    tile->bg = get_color_id(184,147,92);
    tile->fg = get_color_id(133*0.8,94*0.8,66*0.8);
    tile->col = PAIR_COLOR(tile->fg, tile->bg);

    SET_COLOR(tile->col);
    mvaddstr(tile->r, tile->c, "#");
    UNSET_COLOR(tile->col);
}

void set_tile_template_TT_BOULDER_func() {
    tile_template[TT_BOULDER].type = TT_BOULDER;
    tile_template[TT_BOULDER].name = "Large Boulder";
    tile_template[TT_BOULDER].desc = "A large granite slab has pierced up through the land.";
}
void draw_tile_func_TT_BOULDER(tile *tile) {
    if(tile->fg > 0) unget_color_id(tile->fg);
    if(tile->bg > 0) unget_color_id(tile->bg);

    tile->bg = get_color_id(100,100,100);
    tile->fg = 7;
    tile->col = PAIR_COLOR(tile->fg, tile->bg);

    SET_COLOR(tile->col);
    mvaddstr(tile->r, tile->c, "^");
    UNSET_COLOR(tile->col);
}

void set_tile_template_TT_SHELF_func() {
    tile_template[TT_SHELF].type = TT_SHELF;
    tile_template[TT_SHELF].name = "Wooden Shelf";
    tile_template[TT_SHELF].desc = "A set of large planks are piled together to form a crude shelf. A set of unidentifiable items are placed within it. You think something just moved.";
}
void draw_tile_func_TT_SHELF(tile *tile) {
    if(tile->fg > 0) unget_color_id(tile->fg);
    if(tile->bg > 0) unget_color_id(tile->bg);

    tile->bg = get_color_id(186,140,99);
    tile->fg = get_color_id(133*0.8,94*0.8,66*0.8);
    tile->col = PAIR_COLOR(tile->fg, tile->bg);

    SET_COLOR(tile->col);
    mvaddstr(tile->r, tile->c, "#");
    UNSET_COLOR(tile->col);
}

void set_tile_template_TT_BOX_func() {
    tile_template[TT_BOX].type = TT_BOX;
    tile_template[TT_BOX].name = "Pile of Boxes";
    tile_template[TT_BOX].desc = "A collection of wooden boxes that are roughly piled together. It'd be difficult to pass through them, let alone retrieve any items within.";
}
void draw_tile_func_TT_BOX(tile *tile) {
    if(tile->fg > 0) unget_color_id(tile->fg);
    if(tile->bg > 0) unget_color_id(tile->bg);

    tile->bg = get_color_id(186,140,99);
    tile->fg = get_color_id(133*0.8,94*0.8,66*0.8);
    tile->col = PAIR_COLOR(tile->fg, tile->bg);

    SET_COLOR(tile->col);
    mvaddstr(tile->r, tile->c, "[");
    UNSET_COLOR(tile->col);
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
    INIT_TILE_MACRO(TT_RED_CARPET)
    INIT_TILE_MACRO(TT_GREEN_CARPET)
    INIT_TILE_MACRO(TT_DIRT_FLOOR);
    INIT_TILE_MACRO(TT_TREE);
    INIT_TILE_MACRO(TT_FENCE);
    INIT_TILE_MACRO(TT_BOULDER);
    INIT_TILE_MACRO(TT_SHELF);
    INIT_TILE_MACRO(TT_BOX);
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

char *tile_status_to_str(tile_status t) {
    switch(t) {
        case TS_FIRE:
            return "on fire";
        case TS_BLOOD:
            return "covered with blood";
        case TS_BOMB:
            return "storing a piece of dead potato";
        case TS_PEER:
            return "storing an eggplant corpse";
        case TS_HEALTH:
            return "storing a vial of blood";
        default:
            return "null status";
    }
}

char *tile_flag_to_str(tile_flag t) {
    switch(t) {
        case TF_BURNABLE:
            return "catch fire";
        case TF_DOOR:
            return "lead to a different room";
        case TF_PASSABLE:
            return "let a carrot pass through";
        default:
            return "null flag";
    }
}