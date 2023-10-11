#include "gl_util.h"
#include "global.h"
#include "mat.h"

#include <stdio.h>

#define RECT_VERTICES_ROWS 6
#define RECT_VERTICES_COLS 2

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

array_t GL_UTIL_camera(
) {
    return MAT_vec2_new(camera_x, camera_y);
}

void GL_UTIL_set_pixelperfect_scale(
) {
    int pix_scale_w = (int)framebuffer_w / (int)SCREEN_WIDTH;
    int pix_scale_h = (int)framebuffer_h / (int)SCREEN_HEIGHT;

    pixel_perfect_scale = MIN(pix_scale_w, pix_scale_h);
}

array_t GL_UTIL_coord_to_matrix(
    render_coord_t coord
) {
    array_t arr = MAT_new(RECT_VERTICES_ROWS, RECT_VERTICES_COLS);

    arr.values[0 ]=coord.x1; arr.values[1 ]=coord.y2;
    arr.values[2 ]=coord.x2; arr.values[3 ]=coord.y2;
    arr.values[4 ]=coord.x2; arr.values[5 ]=coord.y1;
    arr.values[6 ]=coord.x1; arr.values[7 ]=coord.y2;
    arr.values[8 ]=coord.x2; arr.values[9 ]=coord.y1;
    arr.values[10]=coord.x1; arr.values[11]=coord.y1;

    return arr;
}
