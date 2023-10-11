#include <stdbool.h>

#include "gl_util.h"

#ifndef TILE_H
#define TILE_H

#define WIDTH_COEF (float)TILE_WIDTH / (float) SCREEN_WIDTH
#define HEIGHT_COEF (float)TILE_HEIGHT / (float) SCREEN_HEIGHT

typedef struct tile {
  int            x;
  int            y;
  int            row;
  int            col;
  bool           obstacle;
  int            tileset_id;
} tile_t;

tile_t* TILE_new(int x, int y, int blueprint_id);

void TILE_free(tile_t *tile);

#endif
