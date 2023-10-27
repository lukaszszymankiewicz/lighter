#include <stdbool.h>
#include <stdlib.h>

#include "global.h"
#include "post.h"
#include "scene.h"

#include "gl_util.h"

void POST_set(
) {
    GL_UTIL_set_pixelperfect_scale();
}

void POST_draw(
) {
    int texture          = SCENE_get_layer_buffer_tex(LAYER_BUFFER);
    
    // camera needs to zero
    camera_x = 0; camera_y = 0;

    SCENE_draw_texture(
          (1138 - 960)/2, (768-720)/2,
                     960,         720,
                       0,           0,
                     320,         240,
                     320,         240,
                   false,         true,
        texture
    );
    // SCENE_draw_texture(
    //       0,   0,
    //     320, 240,
    //       0,   0,
    //     320, 240,
    //     320, 240,
    //     false, true,
    //     texture
    // );
}
