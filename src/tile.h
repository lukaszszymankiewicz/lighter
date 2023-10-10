#include <stdbool.h>

#include "gl_util.h"

#ifndef TILE_H
#define TILE_H

#define WIDTH_COEF (float)TILE_WIDTH / (float) SCREEN_WIDTH;
#define HEIGHT_COEF (float)TILE_HEIGHT / (float) SCREEN_HEIGHT;

typedef struct tile {
  render_coord_t clip;
  render_coord_t render;
  int            x,
  int            y,
  int            row,
  int            col,
  int            blueprint_id;
  int            tileset_id;
  bool           obstacle;
} tile_t;

tile_t* TILE_new(
    int            blueprint_id,
    int            tileset_id,
    int            row,
    int            col,
    int            x,
    int            y
);

void TILE_free(tile_t *tile);

#endif
