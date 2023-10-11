#include "gl_util.h"
#include "global.h"
#include "mat.h"

#include <stdio.h>

int camera_x              = 0;
int camera_y              = 0;
float framebuffer_w       = 1.0;
float framebuffer_h       = 1.0;
float pixel_perfect_scale = 1.0;

array_t GL_UTIL_id(
    int id
) {
    return MAT_scalar_new((float)id);
}

array_t GL_UTIL_scale(
) {
    return MAT_vec2_new((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
}

//TODO: to GFX!
void GL_UTIL_set_pixelperfect_scale(
) {
    int pix_scale_w = (int)framebuffer_w / (int)SCREEN_WIDTH;
    int pix_scale_h = (int)framebuffer_h / (int)SCREEN_HEIGHT;

    pixel_perfect_scale = MIN(pix_scale_w, pix_scale_h);
}

