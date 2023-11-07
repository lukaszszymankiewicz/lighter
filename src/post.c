#include "gfx.h"
#include "global.h"
#include "post.h"
#include "scene.h"


int POST_get_screen_multiplication_coef(
    float w, float h
) {
    int scale_w = (int)w / (int)SCREEN_WIDTH;
    int scale_h = (int)h / (int)SCREEN_HEIGHT;

    return MIN(scale_w, scale_h);
}

// TODO: rename it to scalre or smth?
void POST_draw(
) {
    // force camera to be zero
    camera_x = 0;
    camera_y = 0;

    int texture = SCENE_get_buffer_tex();
    int w       = SCENE_get_buffer_width();
    int h       = SCENE_get_buffer_height();
    int m       = POST_get_screen_multiplication_coef(w, h);
    int x0      = (w-SCREEN_WIDTH*m)/2;
    int y0      = (h-SCREEN_HEIGHT*m)/2;

    SCENE_draw_texture(
        x0, y0,
        SCREEN_WIDTH*m, SCREEN_HEIGHT*m,
        0, 0,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        false, true,
        texture
    );
}
