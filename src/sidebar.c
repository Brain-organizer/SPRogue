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
#define HEIGHT 40
#define WIDTH 20
#define START 120 - WIDTH

WINDOW *sidewin;

void init_sidebar() {
    sidewin = newwin(HEIGHT, WIDTH, 0, START);
}

int add_sidebar_for_entity(entity *entity, int y) {
    int x = 0;
    WSET_COLOR(sidewin, entity->col);
    mvwaddstr(sidewin, y, x, entity->icon);
    x += strlen(entity->icon);
    WUNSET_COLOR(sidewin, entity->col);
    mvwaddstr(sidewin, y, x, ": ");
    x += 2;
    mvwaddstr(sidewin, y, x, entity->name);
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

    wclear(sidewin);

    rm = get_cur_room();
    y = 0;

    for(i = 0; i < cvector_size(rm->entities); ++i) {
        y = add_sidebar_for_entity(rm->entities[i], y);
    }

    wrefresh(sidewin);
}