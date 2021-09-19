#include "primitives.h"

#ifndef OBSTACLE_H
#define OBSTACLE_H

obstacle_t* OBS_init(int x1, int y1, int x2, int y2);
obstacle_t* OBS_get_obstacles_from_polygon(vertex_t* poly);
obstacle_t* OBS_find_candidates(obstacle_t** head, int y);
void OBS_push(obstacle_t** head, int x1, int y1, int x2, int y2);
void OBS_free(obstacle_t* obstacles);
void OBS_delete(obstacle_t** head, int y);
void OBS_merge(obstacle_t** head, obstacle_t* candidates);

#endif
