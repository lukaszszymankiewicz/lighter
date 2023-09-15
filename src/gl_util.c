#include "gl_util.h"
#include "global.h"

float global_x_scale        = 1.0;
float global_y_scale        = 1.0;

float GL_UTIL_x(int x) {
    return (((float)x) / (float)SCREEN_WIDTH) * global_x_scale;
}

float GL_UTIL_y(int y) {
    return (((float)y) / (float)SCREEN_HEIGHT) * global_y_scale * -1;
}

render_coord_t GL_UTIL_rect(
    int x, int y,
    int w, int h
) {
    return (render_coord_t) {
        GL_UTIL_x(x), GL_UTIL_y(y),
        GL_UTIL_x(x + w), GL_UTIL_y(y + h)
    };
}
