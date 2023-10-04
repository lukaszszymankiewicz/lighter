#include <stdlib.h>

#include "global.h"
#include "post.h"
#include "scene.h"

#include "gl_util.h"
#include "mat.h"

#define VERTICES_PER_POST_TEX  24
#define POST_RENDER_COUNT      4

float* POST_vertices(
) {
    int    i;
    float *v = NULL;
    v        = (float*)malloc(sizeof(float) * VERTICES_PER_POST_TEX);
    i        = 0;
    
    render_coord_t render = GL_UTIL_rect(0.0, 0.0, (float)SCREEN_WIDTH, (float)SCREEN_WIDTH);
    render_coord_t clip   = (render_coord_t){ 0.0, 1.0, 1.0, 0.0 };

    // TODO: commonize it
    // Position                         Texcoords
    v[i++]=render.x1; v[i++]=render.y1; v[i++]=clip.x1; v[i++]=clip.y1; // Top-left
    v[i++]=render.x2; v[i++]=render.y1; v[i++]=clip.x2; v[i++]=clip.y1; // Top-right
    v[i++]=render.x2; v[i++]=render.y2; v[i++]=clip.x2; v[i++]=clip.y2; // Bottom-right
    v[i++]=render.x1; v[i++]=render.y1; v[i++]=clip.x1; v[i++]=clip.y1; // Top-left
    v[i++]=render.x2; v[i++]=render.y2; v[i++]=clip.x2; v[i++]=clip.y2; // Bottom-right
    v[i++]=render.x1; v[i++]=render.y2; v[i++]=clip.x1; v[i++]=clip.y2; // Bottom-left

    return v;
}

void POST_draw(
) {
    int    len      = VERTICES_PER_POST_TEX;

    float *vertices = NULL;
    vertices        = POST_vertices();
    
    int buffer_texture = SCENE_get_layer_buffer_tex(LAYER_BUFFER);

    SCENE_activate_layer(LAYER_BUFFER);
    SCENE_add_new_drawable_object();
    SCENE_add_uniform(MAT_vec2_new(0.0, 0.0));     // aCamera
    SCENE_set_texture(GL_UTIL_id(buffer_texture)); // texture
    SCENE_add_vertices(len, vertices, POST_RENDER_COUNT);
}
