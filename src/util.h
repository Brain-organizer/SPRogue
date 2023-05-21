#ifndef UTIL_HEADER
#define UTIL_HEADER

#define MOVE_TYPES 4

static const int MOVE_DIRS[MOVE_TYPES][2] = {
    {1, 0},
    {-1, 0},
    {0, 1},
    {0, -1},
};

typedef enum dir_type {
    DOWN,
    UP,
    RIGHT,
    LEFT,
} dir_type;

void raise(char *s);

#endif