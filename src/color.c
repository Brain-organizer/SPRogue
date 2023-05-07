/*
Reference: https://www.linuxjournal.com/content/about-ncurses-colors-0
*/

#include "color.h"

short curs_color(int fg)
{
    switch (7 & fg) {           /* RGB */
    case 0:                     /* 000 */
        return (COLOR_BLACK);
    case 1:                     /* 001 */
        return (COLOR_BLUE);
    case 2:                     /* 010 */
        return (COLOR_GREEN);
    case 3:                     /* 011 */
        return (COLOR_CYAN);
    case 4:                     /* 100 */
        return (COLOR_RED);
    case 5:                     /* 101 */
        return (COLOR_MAGENTA);
    case 6:                     /* 110 */
        return (COLOR_YELLOW);
    case 7:                     /* 111 */
        return (COLOR_WHITE);
    }
}

int is_bold(int col)
{
    /* return the intensity bit */
    return col & (1 << 3);
}

int is_blink(int col) {
    return col & (1 << 7);
}

int colornum(int fg, int bg, bool bold, bool blink)
{
    return (blink << 7) | ((bg & 7)<<4) | (bold << 3) | (fg & 7);
}

void init_colorpairs(void)
{
    int fg, bg;
    int colorpair;

    for (bg = 0; bg <= 7; bg++) {
        for (fg = 0; fg <= 7; fg++) {
            colorpair = colornum(fg, bg, true, true);
            if(colorpair != 0) init_pair(colorpair, curs_color(fg), curs_color(bg));

            colorpair = colornum(fg, bg, true, false);
            if(colorpair != 0) init_pair(colorpair, curs_color(fg), curs_color(bg));
            
            colorpair = colornum(fg, bg, false, true);
            if(colorpair != 0) init_pair(colorpair, curs_color(fg), curs_color(bg));

            colorpair = colornum(fg, bg, false, false);
            if(colorpair != 0) init_pair(colorpair, curs_color(fg), curs_color(bg));
        }
    }
}

void setcolor(int col)
{
    /* set the color pair (colornum) and bold/bright (A_BOLD) */

    attron(COLOR_PAIR(col));
    if (is_bold(col)) {
        attron(A_BOLD);
    }
    if(is_blink(col)) {
        attron(A_BLINK);
    }
}

void unsetcolor(int col)
{
    /* unset the color pair (colornum) and
       bold/bright (A_BOLD) */

    attroff(COLOR_PAIR(col));
    if (is_bold(col)) {
        attroff(A_BOLD);
    }
    if(is_blink(col)) {
        attroff(A_BLINK);
    }
}