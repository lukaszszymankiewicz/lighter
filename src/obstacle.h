#include "primitives.h"

#ifndef OBSTACLE_H
#define OBSTACLE_H

obstacle_t* OBS_init(int x1, int y1, int x2, int y2);
void OBS_push(obstacle_t** head, int x1, int y1, int x2, int y2);
void OBS_free(obstacle_t* obstacles);

#endif
