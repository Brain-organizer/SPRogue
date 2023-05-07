#define __GLIBC_USE
#define _GNU_SOURCE
#define __USE_GNU
#define __USE_EXTERN_INLINES

#include <stdio.h>
#include <ncurses.h>
#include <stdbool.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <locale.h>

#include "color.h"
#include "tiles.h"
#include "map.h"

int main() {
    int bg, fg;
    int col;
    room *rm;

    setlocale(LC_ALL, "");

    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    
    start_color();
    init_colorpairs();
    init_tiles();
    
    rm = get_tmp_room();
    draw_room(rm);

    refresh();
    
    getch();
    free_room(rm);
    endwin();

    return 0;
}