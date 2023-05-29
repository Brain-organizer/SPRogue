#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "message.h"
#include "cvector.h"

#define HEIGHT 5
#define WIDTH 100
#define START 40 - HEIGHT

WINDOW *msgwin;
cvector_vector_type(char *) msgs;
int start;

void init_message() {
    msgwin = newwin(HEIGHT, WIDTH, START, 0);
    msgs = NULL;
    start = 0;
}
char *add_message() {
    char *msg;
    msg = malloc(2 * WIDTH + 1);
    cvector_push_back(msgs, msg);

    start = cvector_size(msgs) - HEIGHT;
    if(start < 0) start = 0;

    return msg;
}
void draw_message() {
    int i;
    wclear(msgwin);
    for(i = start; i < cvector_size(msgs); ++i) {
        mvwaddstr(msgwin, i - start, 0, msgs[i]);
    }
    wrefresh(msgwin);
}
void free_message() {
    int i;
    for(i = 0; i < cvector_size(msgs); ++i) {
        free(msgs[i]);
    }
    cvector_free(msgs);
}