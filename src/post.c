#include <stdlib.h>

#include "global.h"
#include "post.h"
#include "scene.h"

#include "gl_util.h"
#include "mat.h"

#define VERTICES_PER_POST_TEX       24
#define POST_RENDER_COUNT           4

render_coord_t render = (render_coord_t) { 0.0, 0.0, 0.0, 0.0 };
render_coord_t clip   = (render_coord_t) { 0.0, 0.0, 0.0, 0.0 };

void POST_set(
) {
    GL_UTIL_set_pixelperfect_scale();
    render = (render_coord_t) { 0.0, 0.0, (float)SCREEN_WIDTH, (float)SCREEN_WIDTH };
    clip   = GL_UTIL_clip(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT);
}

array_t POST_vertices(
) {
    array_t pos_arr = GL_UTIL_coord_to_matrix(render);
    array_t tex_arr = GL_UTIL_coord_to_matrix(clip);

    MAT_join(pos_arr, tex_arr);

    return pos_arr;
}

void POST_draw(
) {
    int    len           = VERTICES_PER_POST_TEX;
    int texture          = SCENE_get_layer_buffer_tex(LAYER_BUFFER);

    array_t vertices_arr = POST_vertices();
    array_t camera_arr   = MAT_vec2_new(0.0, 0.0);
    array_t scale_arr    = GL_UTIL_scale();
    array_t texture_arr  = GL_UTIL_id(texture);

    SCENE_activate_layer(LAYER_BUFFER);
    SCENE_add_new_drawable_object();
    SCENE_add_uniform(camera_arr);
    SCENE_add_uniform(scale_arr);
    SCENE_set_texture(texture_arr);
    SCENE_add_vertices(len, vertices_arr.values, POST_RENDER_COUNT);
}
