#ifndef TILE_H
#define TILE_H

typedef struct tile {
  int          x;       // x 
  int          y;       // y 
  int          width;   // tile width (default is 32)
  int          height;  // tile height (default is 32)
} tile_t;

tile_t* TILE_new(int x, int y);

#endif
