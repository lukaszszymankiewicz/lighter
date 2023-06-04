#include <stdbool.h>

#include "texture.h"

#ifndef RENDER_H
#define RENDER_H

void RENDER_texture(
    texture_t *texture,
    float      render_x1, float      render_y1,
    float      render_x2, float      render_y2,
    float      clip_x1,   float      clip_y1,
    float      clip_x2,   float      clip_y2,
    bool       flip
);

#endif
