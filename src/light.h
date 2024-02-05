#include "segment.h"
#include "vertex.h"

#ifndef LIGHT_H
#define LIGHT_H

#define R                               2000
#define RAY_THRESHOLD                   2

vertex_t* LIG_calculate(
    int        x,
    int        y,
    float      width,
    float      angle,
    segment_t* obstacles
);

#endif
