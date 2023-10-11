#include <stdbool.h>

#ifndef TILE_H
#define TILE_H

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
