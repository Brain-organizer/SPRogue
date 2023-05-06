/*
이곳에 메인 게임 루프가 보관 될 예정.
아직은 아무것도 없습니다.
*/

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

typedef enum tile_type {
    DOOR_VER = -11,
    DOOR_HOR = -10,

    WALL_VER = -7,
    WALL_HOR = -6,

    SPACE = -3,
    DARK = -2,

    NULLTILE = -1,
} tile_type;

#define ROOM_WIDTH 13
#define ROOM_HEIGHT 7

int room_map[ROOM_HEIGHT][ROOM_WIDTH];

tile_type conv_char_to_tile_type(char c) {
    switch(c) {
        case ' ': return DARK;
        case '.': return SPACE;
        case '|': return WALL_VER;
        case '-': return WALL_HOR;
        case ':': return DOOR_VER;
        case '_': return DOOR_HOR;
        default: return NULLTILE;
    }
}

char conv_tile_type_to_char(tile_type t) {
    switch(t) {
        case DARK: return ' ';
        case SPACE: return '.';
        case WALL_VER: return '|';
        case WALL_HOR: return '-';
        case DOOR_VER: return ':';
        case DOOR_HOR: return '_';
        case NULLTILE: return 0;
    }
}

int read_map_file(char *path) {
    FILE *fp;
    int row, col;
    size_t strlen, t;
    char *line;

    memset(room_map, -1, sizeof(room_map));

    fp = fopen(path, "r");

    if((fp = fopen(path, "r")) == NULL) {
        fputs("fopen() error", stderr);
        return -1;
    }

    line = NULL;
    strlen = 0;

    for(row = 0; row < ROOM_HEIGHT; ++row) {
        if((strlen = getline(&line, &strlen, fp)) == -1) {
            fputs("getline() error", stderr);
            fclose(fp);
            return -1;
        }
        else if(strlen-1 != ROOM_WIDTH) {
            free(line);
            fclose(fp);
            fprintf(stderr, "getline() error. For file %s, length of row %d is %d, which is not %d\n", path, row, strlen-1, ROOM_WIDTH);
            return -1;
        }

        for(col = 0; col < ROOM_WIDTH; ++col) {
            room_map[row][col] = conv_char_to_tile_type(line[col]);
            
            if(room_map[row][col] == NULLTILE) {
                free(line);
                fclose(fp);
                fputs("conv_char_to_tile_type() error", stderr);
                return -1;
            }
        }

        free(line);
        line = NULL;
        strlen = 0;
    }

    fclose(fp);

    return 0;
}

int draw_map() {
    int row, col;
    char c;
    
    for(row = 0; row < ROOM_HEIGHT; ++row) {
        for(col = 0; col < ROOM_WIDTH; ++col) {
            c = conv_tile_type_to_char(room_map[row][col]);
            if(c) mvaddch(row, col, conv_tile_type_to_char(room_map[row][col]));
            else {
                fputs("conv_tile_type_to_char() error", stderr);
                return -1;
            }
        }
    }

    return 0;
}

int main() {
    int x, y, x_prev, y_prev, i;
    char msg[] = "HELLO ROGUE";
    bool chck[30] = {false};

    initscr();
    resize_term(ROOM_HEIGHT, ROOM_WIDTH);
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    read_map_file("resource/room/tmp.txt");
    draw_map();
    refresh();

    sleep(10);

    endwin();

    return 0;
}