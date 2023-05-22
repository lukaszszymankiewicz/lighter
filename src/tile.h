#include "texture.h"
#include "img.h"

#ifndef TILE_H
#define TILE_H

#define WIDTH_COEF (float)TILE_WIDTH / (float) SCREEN_WIDTH;
#define HEIGHT_COEF (float)TILE_HEIGHT / (float) SCREEN_HEIGHT;

typedef struct tile {
  render_img_t   img;
  render_coord_t coord;
  int            tileset_id;
  bol            obstacle;
} tile_t;

tile_t* TILE_new(
    texture_t *texture,
    int clip_x1, int clip_y1,
    int clip_x2, int clip_y2,
    int x,       int y
);

#endif
