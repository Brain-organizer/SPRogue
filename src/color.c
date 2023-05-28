#include "color.h"
#include "cvector.h"

#define RGB_BRIGHTNESS 3

color colors[256];

void reset_colors() {
    memset(colors, 0, sizeof(colors));
}

void init_color_pairs() {
    int f, b;
    for(f = 0; f < 256; ++f) {
        for(b = 0; b < 256; ++b) {
            if(f != 0 || b != 0)
                init_pair((f << 8) + b, f, b);
        }
    }
}

int get_color_id(int r, int g, int b) {
    int i;
    for(i = 1; i < 256; ++i) {
        if(colors[i].cnt > 0 && colors[i].r == r && colors[i].g == g && colors[i].b == b) {
            ++colors[i].cnt;
            return i;
        }
    }
    for(i = 1; i < 256; ++i) {
        if(colors[i].cnt == 0) {
            colors[i].r = r;
            colors[i].g = g;
            colors[i].b = b;
            colors[i].cnt = 1;
            init_color(i, r * RGB_BRIGHTNESS, g * RGB_BRIGHTNESS, b * RGB_BRIGHTNESS);
            return i;
        }
    }

    fputs("get_color_id()\n", stderr);
    exit(1);
}

void unget_color_id(int i) {
    --colors[i].cnt;
    if(colors[i].cnt < 0) {
        fputs("unget_color_id()\n", stderr);
        exit(1);
    }
}