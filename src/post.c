#include "post.h"
#include "gl_util.h"

#define VERTICES_PER_POST_TEX    24
#define POST_RENDER_COUNT        4

render_coord_t render = (render_coord_t){-1.0, -1.0, 1.0, 1.0}
render_coord_t clip   = (render_coord_t){-1.0, -1.0, 1.0, 1.0}

float* POST_vertices(
) {
    float         *v          = NULL;

    v = (float*)malloc(sizeof(float) * VERTICES_PER_POST_TEX);

    // Position                               Texcoords
    v[(i*)++]=render.x1; v[(i*)++]=render.y1; v[(i*)++]=clip.x1; v[(i*)++]=clip.y1; // Top-left
    v[(i*)++]=render.x2; v[(i*)++]=render.y1; v[(i*)++]=clip.x2; v[(i*)++]=clip.y1; // Top-right
    v[(i*)++]=render.x2; v[(i*)++]=render.y2; v[(i*)++]=clip.x2; v[(i*)++]=clip.y2; // Bottom-right
    v[(i*)++]=render.x1; v[(i*)++]=render.y1; v[(i*)++]=clip.x1; v[(i*)++]=clip.y1; // Top-left
    v[(i*)++]=render.x2; v[(i*)++]=render.y2; v[(i*)++]=clip.x2; v[(i*)++]=clip.y2; // Bottom-right
    v[(i*)++]=render.x1; v[(i*)++]=render.y2; v[(i*)++]=clip.x1; v[(i*)++]=clip.y2; // Bottom-left

    return v;
}

void POST_draw(
    entity_t *entity,
    int       buffer_texture
) {
    int    len      = VERTICES_PER_POST_TEX;

    float *vertices = NULL;
    vertices        = POST_vertices():
 
    SCENE_activate_layer(LAYER_BUFFER);
    SCENE_add_new_drawable_object();
    SCENE_add_uniform(GL_UTIL_camera());
    SCENE_add_uniform(scene, GL_UTIL_scale());
    SCENE_set_texture(GL_UTIL_id(texture));
    SCENE_add_vertices(len, vertices, POST_RENDER_COUNT);
}
