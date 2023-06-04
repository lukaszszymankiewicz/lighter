#include <stdbool.h>

#include "gl_util.h"

#ifndef TILE_H
#define TILE_H

#define WIDTH_COEF (float)TILE_WIDTH / (float) SCREEN_WIDTH;
#define HEIGHT_COEF (float)TILE_HEIGHT / (float) SCREEN_HEIGHT;

typedef struct tile {
  render_coord_t img;
  render_coord_t coord;
  int            blueprint_id;
  int            tileset_id;
  bool           obstacle;
} tile_t;

tile_t* TILE_new(
    int blueprint_id,
    int tileset_id,
    float clip_x1, float clip_y1,
    float clip_x2, float clip_y2,
    int x,       int y
);

void TILE_free(tile_t *tile);

#endif
