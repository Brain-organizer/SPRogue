#include <ncurses.h>
#include <stdio.h>
#include <string.h>

#include "entity.h"
#include "player.h"
#include "floor.h"
#include "rooms.h"
#include "color.h"
#include "sidebar.h"
#include "cvector.h"

// makefile 내 row 수와 연관 있음
#define HEIGHT 38
#define WIDTH 23
#define START 115 - WIDTH

WINDOW *sidewin;
cvector_vector_type(int) colorsv;

void init_sidebar() {
    sidewin = newwin(HEIGHT, WIDTH, 0, START);
    colorsv = NULL;
}

int add_sidebar_for_entity(entity *entity, int y, int i) {
    int x = 0, col;
    WSET_COLOR(sidewin, entity->col);
    mvwaddstr(sidewin, y, x, entity->icon);
    x += strlen(entity->icon);
    WUNSET_COLOR(sidewin, entity->col);
    mvwaddstr(sidewin, y, x, ": ");
    x += strlen(": ");

    if(entity->is_enemy) {
        col = get_color_id(220, 0, 0);
    }
    else {
        col = get_color_id(0, 220, 0);
    }
    cvector_push_back(colorsv, col);
    WSET_COLOR(sidewin, PAIR_COLOR(col, 0));

    mvwprintw(sidewin, y, x, "%s(%d)", entity->name, i);

    WUNSET_COLOR(sidewin, PAIR_COLOR(col, 0));

    ++y;
    x = 0;

    mvwprintw(sidewin, y, x, "Health: %d", entity->hp);


    ++y;
    x = 0;
    if(entity->type != ET_CARROT) {
        mvwprintw(sidewin, y, x, "Delay: %d", entity->delay);
        ++y;
        x = 0;
    }

    ++y;

    return y;
}

void draw_sidebar() {
    room * rm;
    int y, i;

    for(i = 0; i < cvector_size(colorsv); ++i) {
        if(colorsv[i] > 0) {
            unget_color_id(colorsv[i]);
        }
    }
    cvector_clear(colorsv);

    wclear(sidewin);

    rm = get_cur_room();
    y = 0;

    for(i = 0; i < cvector_size(rm->entities); ++i) {
        y = add_sidebar_for_entity(rm->entities[i], y, i);
    }

    wrefresh(sidewin);
}