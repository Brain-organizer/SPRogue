#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "message.h"
#include "cvector.h"
#include "client.h"
#include "rooms.h"
#include "tiles.h"
#include "floor.h"

#define HEIGHT 8
#define WIDTH 91
#define START 39 - HEIGHT

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
void describe_tile(int r, int c) {
    room *rm = get_cur_room();
    tile *tl = rm->map[r] + c;
    char bf[WIDTH*HEIGHT+1];
    int i;
    bool flg;

    add_message("You are looking at the %s.", tl->name);
    add_message(tl->desc);

    strcpy(bf, "This tile is");
    flg = true;
    for(i = 0; i < TS_NUM; ++i) {
        if(tl->status & (1 << i)) {
            flg = false;
            strcat(bf, ", ");
            strcat(bf, tile_status_to_str(1<<i));
        }
    }
    if(flg) strcat(bf, ", unremarkable.");
    add_message(bf);

    strcpy(bf, "This tile can");
    flg = true;
    for(i = 0; i < TF_NUM; ++i) {
        if(tl->flags & (1 << i)) {
            flg = false;
            strcat(bf, ", ");
            strcat(bf, tile_flag_to_str(1<<i));
        }
    }
    if(flg) strcat(bf, ", do nothing.");
    add_message(bf);
}