#include <stdbool.h>

#include "mat.h"

#ifndef SCENE_H
#define SCENE_H

#define MAX_LAYERS_ON_SCENE           5
#define MAX_BUFFERS_ON_SCENE          3
#define MAX_DRAWBLE_OBJECTS_ON_LAYER  20

enum layers { LAYER_LIGHT, LAYER_TILE, LAYER_SPRITE, SCALED_IMAGE, LAYER_ALL };
enum buffers { DEFAULT_FRAMEBUFFER, FIRST_BUFFER };

typedef struct  render_object {
    int         shader_id;
    int         texture;
    int         mode;
    int         n_vertices;
    int         count;
    array_t    *vertices;    
    array_t    *uniforms[MAX_SHADER_UNIFORMS];
    char       *name;  // few bytes to ease debugging
} render_object_t;

typedef struct layer {
    char             *name;
    bool              on;
    int               n_objs;
    render_object_t   objs[MAX_DRAWBLE_OBJECTS_ON_LAYER];
} layer_t; 

typedef struct scene {
    int                n_layers;
    int                n_buffers;
    int                cur_layer;
    int                cur_buffer;
    layer_t            layers[MAX_LAYERS_ON_SCENE];
    framebuffer_t     *buffers[MAX_BUFFERS_ON_SCENE];    
} scene_t; 

void SCENE_init();
void SCENE_clear();
void SCENE_add_layer(int layer, char *name);
void SCENE_activate_buffer(int buffer);
void SCENE_activate_layer(int layer);
void SCENE_add_defalt_buffer();
void SCENE_add_buffer(int id, int w, int h);

void SCENE_draw_scaled_buffer();

void SCENE_clean_buffer(int buffer);
void SCENE_free();
void SCENE_render_current_layer();
void SCENE_clear_layer(int layer);

array_t *SCENE_texture_pos(
    int   draw_x, int   draw_y,
    int   draw_w, int   draw_h,
    int   clip_x, int   clip_y,
    int   clip_w, int   clip_h,
    int    tex_w, int    tex_h,
    bool  flip_w, bool  flip_h
); 

void SCENE_put_texture_to_scene(char *name, array_t *vertices, int texture);
array_t *SCENE_polygon_coord_to_matrix(float *coords, int n_vertices);

void SCENE_put_light_polygon_to_scene(
    array_t *vertices,
    float    diffuse,
    int      x0,
    int      y0,
    int      r,
    int      g,
    int      b,
    int      a
);

#endif
