#include "primitives.h"
#include "obstacle.h"

#ifndef TILE_H
#define TILE_H

typedef struct tile {
  int          x;       // x offset on tileset
  int          y;       // y offset on tileset
  int          width;   // tile width (default is 32)
  int          height;  // tile height (default is 32)
  int          index;   // tileset index
} tile_t;

tile_t* TILE_new(int tileset_index, int x_offset, int y_offset);

// obstacle_t* TILE_calculate_ray_obstacles(tiles_list_t * tiles);

#endif
