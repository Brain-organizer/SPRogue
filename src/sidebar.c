#include <ncurses.h>
#include <stdio.h>

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

cvector_vector_type(char *) lines;
int start;
WINDOW *sidewin;


void init_sidebar() {
    sidewin = newwin(HEIGHT, WIDTH, 0, START);
    lines = NULL;
    start = 0;
}

void draw_sidebar() {
    int i;
    for(i = 0; i < cvector_size(lines); ++i)
        mvwaddstr(sidewin, i, 0, lines[i]);
    //box(sidewin, 0, 0);
    wrefresh(sidewin);
}

void set_sidebar() {
    cvector_push_back(lines, "Hello sidebar!");
    draw_sidebar();
}

void scroll_down_sidebar();
void scroll_up_sidebar();