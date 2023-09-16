#include "gl_util.h"
#include "global.h"

float global_x_scale        = 1.0;
float global_y_scale        = 1.0;

float GL_UTIL_x(int x) {
    // return (((float)x) / (float)SCREEN_WIDTH) * global_x_scale;
    return (((float)x) / (float)SCREEN_WIDTH);
}

float GL_UTIL_y(int y) {
    // return (((float)y) / (float)SCREEN_HEIGHT) * global_y_scale * -1;
    return (((float)y) / (float)SCREEN_HEIGHT);
}

render_coord_t GL_UTIL_rect(
    int x, int y,
    int w, int h
) {
    return (render_coord_t) {
        GL_UTIL_x(x),     GL_UTIL_y(y),
        GL_UTIL_x(x + w), GL_UTIL_y(y + h)
    };
}

render_coord_t GL_UTIL_clip(
    int x1, int y1,
    int x2, int y2,
    int w,  int h
) {
    return (render_coord_t) {
        (float)x1/(float)w,
        (float)y1/(float)h,
        (float)x2/(float)w,
        (float)y2/(float)h
    };
}
