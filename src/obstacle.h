#include "primitives.h"

#ifndef OBSTACLE_H
#define OBSTACLE_H


void OBS_push(obstacle_t **head, int x1, int y1, int x2, int y2);
void OBS_free(obstacle_t *obstacles);
obstacle_t* TILE_calculate_ray_obstacles(int *tiles, int hero_x, int hero_y);
obstacle_t* OBS_get(obstacle_t *obstacles, int index);
int OBS_len(obstacle_t *obstacles);
obstacle_t* OBS_init(int x1, int y1, int x2, int y2);

#endif
