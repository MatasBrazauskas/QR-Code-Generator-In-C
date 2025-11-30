#ifndef DRAW_H
#define DRAW_H

#include <stddef.h>
#include <stdlib.h>

#include "settings.h"

typedef struct {
    char** matrix;
    size_t sizeLength;
} Buffer;

Buffer* initBuffer(Settings* stg);

void PositionOrientationFinders();

#endif