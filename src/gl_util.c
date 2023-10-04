#include "gl_util.h"
#include "global.h"
#include "mat.h"

#include <stdio.h>

float camera_x       = 0.0;
float camera_y       = 0.0;
float framebuffer_w  = 0.0;
float framebuffer_h  = 0.0;

float* GL_UTIL_id(
    int id
) {
    return MAT_scalar_new((float)id);
}

// no scale, single pixel on image == single pixel on screen resolution
float* GL_UTIL_single_pixel_scale(
) {
    return MAT_imat2_new(2.0 / framebuffer_w, 2.0 / framebuffer_h);
}

// scale applied - single pixel on image == single pixel on 320x240
float* GL_UTIL_multiple_pixel_scale(
) {
    return MAT_imat2_new(2.0 / (float)SCREEN_WIDTH, 2.0 / (float)SCREEN_HEIGHT);
}

float* GL_UTIL_camera(
) {
    return MAT_vec2_new(camera_x, camera_y);
}

float GL_UTIL_x(
    int x
) {
    return (((2.0 * (float)x) / (float)SCREEN_WIDTH) - 1.0) * X_AXIS_DIR;
}

float GL_UTIL_y(
    int y
) {
    return (((2.0 * (float)y) / (float)SCREEN_HEIGHT) - 1.0) * Y_AXIS_DIR;
}

render_coord_t GL_UTIL_rect(
    int x, int y,
    int w, int h
) {
    return (render_coord_t) {
        GL_UTIL_x(x    ), GL_UTIL_y(y    ),
        GL_UTIL_x(x + w), GL_UTIL_y(y + h)
    };
}

render_coord_t GL_UTIL_clip(
    int x1, int y1,
    int x2, int y2,
    int w,  int h
) {
    return (render_coord_t) {
        (float)x1 / (float)w,
        (float)y1 / (float)h,
        (float)x2 / (float)w,
        (float)y2 / (float)h
    };
}
