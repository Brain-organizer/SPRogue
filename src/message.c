#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "message.h"
#include "cvector.h"
#include "client.h"

#define HEIGHT 7
#define WIDTH 100
#define START 42 - HEIGHT

WINDOW *msgwin;
cvector_vector_type(char *) msgs;
int start;
int last;

void init_message() {
    msgwin = newwin(HEIGHT, WIDTH, START, 0);
    msgs = NULL;
    start = 0;
    last = 0;
}
void add_message(const char *format, ...) {
    va_list arg;
    char *msg;
    char bf[WIDTH*HEIGHT+1];
    char *ptr;

    va_start(arg, format);
    vsprintf(bf, format, arg);

    ptr = bf;

    while(strlen(ptr) > 0) {
        msg = malloc(WIDTH + 1);
        strncpy(msg, ptr, WIDTH + 1);
        msg[WIDTH] = '\0';
        ptr += strlen(msg);
        cvector_push_back(msgs, msg);
    }

    start = cvector_size(msgs) - HEIGHT;
    if(start < 0) start = 0;
}
void draw_message() {
    int i;
    wclear(msgwin);
    for(i = start; i < cvector_size(msgs); ++i) {
        mvwaddstr(msgwin, i - start, 0, msgs[i]);
    }
    for(; last < cvector_size(msgs); ++last) {
        post_log(msgs[last]);
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