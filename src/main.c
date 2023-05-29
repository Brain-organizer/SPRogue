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
#include <time.h>

#include "floor.h"
#include "color.h"
#include "sidebar.h"
#include "client.h"
#include "message.h"

void init();
bool quit();

int main() {
    init();

    init_floor();

    draw();
    draw_sidebar();

    while(true) {
        //플레이어 액션 함수 호출 : 함수 내부에서 딜레이를 확인하고 0이면 키 입력을 받아 알맞은 행동 진행
        if(update_player() == false) continue;
        
        //엔티티 액션 함수 호출 : 함수 내부에서 for문을 돌며 모든 엔티티에 대해 update_entity 수행. 
        update_all_entities();

        increase_tick();
        draw();
    }

    free_floor();
    free_message();
    
    endwin();

    return 0;
}

void init() {
    srand(time(NULL));
    
    setlocale(LC_ALL, "");

    init_connect();
    init_tick();
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    
    start_color();
    reset_colors();
    init_color_pairs();
    init_tiles();
    init_entities();
    init_player();

    init_sidebar();

    init_message();
}