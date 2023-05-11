#include "texture.h"

#ifndef IMG_H
#define IMG_H

// image which can be render by OpenGL (part of the `texture`)
typedef struct render_img {
  float        x1;       // glTexCoord2f for x1
  float        y1;       // glTexCoord2f for y1
  float        x2;       // glTexCoord2f for x2
  float        y2;       // glTexCoord2f for y2
} render_img_t;

// OpenGL coord for rendering
typedef struct render_coord {
  float        x1;       // glVertex2f for x1
  float        y1;       // glVertex2f for y1
  float        x2;       // glVertex2f for x2
  float        y2;       // glVertex2f for y2
} render_coord_t;

render_img_t   IMG_new(texture_t *texture, int x1, int y1, int x2, int y2);
render_coord_t IMG_COORD_new(float x1, float y1, float x2, float y2);

#endif
