#include "gl_util.h"
#include "global.h"

float global_x_scale        = 1.0;
float global_y_scale        = 1.0;


render_coord_t GL_UTIL_global_to_gl_coord(
    int        x,       int        y,
    int        w,       int        h,
    float      clip_x1, float      clip_y1,
    float      clip_x2, float      clip_y2
) {
    float x1, y1, x2, y2;

    x1 = (float)x / (float)SCREEN_WIDTH;
    y1 = (float)y / (float)SCREEN_HEIGHT;

    y1 *= -1;

    x2 = (float)x1 + (((clip_x2 - clip_x1) * w) / SCREEN_WIDTH);
    y2 = (float)y1 - (((clip_y2 - clip_y1) * h) / SCREEN_HEIGHT);

    x1 *= global_x_scale;
    y1 *= global_y_scale;
    x2 *= global_x_scale;
    y2 *= global_y_scale;

    return (render_coord_t) {x1, y1, x2, y2};
}

render_coord_t GL_UTIL_gl_to_camera_gl(
    render_coord_t render,
    int camera_x, int camera_y
) {
    float camera_x_diff = (((float)camera_x) / (float)SCREEN_WIDTH) * global_x_scale;
    float camera_y_diff = (((float)camera_y) / (float)SCREEN_HEIGHT) * global_y_scale;

    render.x1 -= camera_x_diff;
    render.y1 += camera_y_diff;
    render.x2 -= camera_x_diff;
    render.y2 += camera_y_diff;

    return render;
}

render_coord_t GL_UTIL_global_to_gl_coord_single(
    int        x, int        y,
    int camera_x, int camera_y
) {
    // float x1, y1;

    // x1 = (float)x / (float)SCREEN_WIDTH;
    // y1 = (float)y / (float)SCREEN_HEIGHT;

    // y1 *= -1;

    // float camera_x_diff = (((float)camera_x) / (float)SCREEN_WIDTH) * global_x_scale;
    // float camera_y_diff = (((float)camera_y) / (float)SCREEN_HEIGHT) * global_y_scale;

    // x1 -= camera_x_diff;
    // y1 += camera_y_diff;

    // x1 *= global_x_scale;
    // y1 *= global_y_scale;

    // return (render_coord_t) {x1, y1, x1, y1};

    return GL_UTIL_global_to_gl_coord(
        x, y, 1, 1, 0, 0, camera_x, camera_y
    );
}
