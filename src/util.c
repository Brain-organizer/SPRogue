#include <stdlib.h>
#include <stdio.h>
#include "util.h"

void raise(char *s) {
    perror(s);
    exit(1);
}