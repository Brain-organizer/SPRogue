#ifndef UTIL_HEADER
#define UTIL_HEADER

typedef enum dir_type {
    DOWN,
    UP,
    RIGHT,
    LEFT,
    MOVE_TYPES,
} dir_type;

static const int MOVE_DIRS[MOVE_TYPES][2] = {
    {1, 0},
    {-1, 0},
    {0, 1},
    {0, -1},
};

void raise(char *s);

#endif