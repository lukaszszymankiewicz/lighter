#include <stdbool.h>

#ifndef GL_UTIL_H
#define GL_UTIL_H

extern float global_x_scale;
extern float global_y_scale;

typedef struct render_coord {
    float        x1;       // glVertex2f for x1 (or glTexCoord2f)
    float        y1;       // glVertex2f for y1 (or glTexCoord2f)
    float        x2;       // glVertex2f for x2 (or glTexCoord2f)
    float        y2;       // glVertex2f for y2 (or glTexCoord2f)
} render_coord_t;

#endif
