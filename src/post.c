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

    int max_w = SCREEN_WIDTH*pixel_perfect_scale; 
    int max_h = SCREEN_HEIGHT*pixel_perfect_scale; 
    
    float scale_w = 0.80;

    float corr_w = (framebuffer_w - max_w) * scale_w;
    // float corr_h = (framebuffer_h - max_h) * 0.1;
    float corr_h = -10;

    render = (render_coord_t) {
        corr_w,
        corr_h,
        max_w + corr_w,
        max_h + corr_h
    };

    clip  = (render_coord_t) { 0, 1, 1, 0 };
}

array_t POST_vertices(
) {
    array_t pos_arr = GL_UTIL_coord_to_matrix(render);
    array_t tex_arr = GL_UTIL_coord_to_matrix(clip);

    MAT_join(&pos_arr, &tex_arr);

    return pos_arr;
}

void POST_draw(
) {
    int    len           = VERTICES_PER_POST_TEX;
    int texture          = SCENE_get_layer_buffer_tex(LAYER_BUFFER);

    array_t vertices_arr = POST_vertices();
    array_t camera_arr   = MAT_vec2_new(0.0, 0.0);
    array_t scale_arr    = MAT_vec2_new((float)1366, (float)768);
    array_t texture_arr  = GL_UTIL_id(texture);

    SCENE_activate_layer(LAYER_BUFFER);
    SCENE_add_new_drawable_object();
    SCENE_add_uniform(camera_arr);
    SCENE_add_uniform(scale_arr);
    SCENE_set_texture(texture_arr);
    SCENE_add_vertices(len, vertices_arr.values, POST_RENDER_COUNT);
}
