/*
이곳에 메인 게임 루프가 보관 될 예정.
아직은 아무것도 없습니다.
*/

#include <stdio.h>
#include <ncurses.h>
#include <stdbool.h>

int main() {
    int x, y, x_prev, y_prev, i;
    char msg[] = "HELLO ROGUE";
    bool chck[30] = {false};

    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    x = 9;
    y = 10;

    mvaddch(y, x, '@');

    while(true) {
        x_prev = x, y_prev = y;

        switch(getch()) {
            case KEY_RIGHT:
                ++x;
                if(x >= 100) x = 100;
                break;
            case KEY_LEFT:
                --x;
                if(x < 0) x = 0;
                break;
            case KEY_UP:
                --y;
                if(y < 0) x = 0;
                break;
            case KEY_DOWN:
                ++y;
                if(y > 100) y = 100;
                break;
        }

        mvaddch(y_prev, x_prev, ' ');

        if(y == 10 && x >= 10 && x < 30) chck[x-10] = !chck[x-10];

        for(i = 0; i < sizeof(msg) - 1; ++i) {
            if(chck[i]) mvaddch(10, 10+i, msg[i]);
            else mvaddch(10, 10+i, ' ');
        }

        mvaddch(y, x, '@');
    }

    endwin();

    return 0;
}