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
#include <signal.h>

#include "floor.h"
#include "color.h"
#include "sidebar.h"
#include "client.h"
#include "message.h"

void init();
void end_game(int signum);
void redirect_2(char *);
bool quit();

int main() {
    signal(SIGINT, end_game);
    signal(SIGTERM, end_game);
    signal(SIGQUIT, end_game);

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

    redirect_2("logs/err");
    
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

void end_game(int signum){
    close_connect();
    mvaddstr(0,1,"게임을 종료합니다.");
    refresh();
    sleep(3);
    endwin();
    exit(0);
}

void redirect_2(char *file){
    int errfd;

    errfd = open(file,O_WRONLY|O_CREAT|O_TRUNC,664);
    dup2(errfd, 2);
    close(errfd);
}