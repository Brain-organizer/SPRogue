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

#include "floor.h"
#include "color.h"

void init();
bool quit();

int main() {

    init();

    init_floor();
    draw();

    while(true) {
        
        draw();
    }

    free_floor();
    
    endwin();

    return 0;
}

void init() {
    setlocale(LC_ALL, "");

    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    
    start_color();
    init_colorpairs();
    init_tiles();
    init_entities();
}