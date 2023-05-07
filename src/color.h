#include <ncurses.h>
#include <stdbool.h>

short curs_color(int fg);

int is_bold(int col);

int is_blink(int col);

int colornum(int fg, int bg, bool bold, bool blink);

void init_colorpairs(void);

void setcolor(int col);

void unsetcolor(int col);