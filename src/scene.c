#include "data/library.h"

#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "components.h"
#include "gfx.h"
#include "global.h"
#include "mat.h"
#include "render.h"
#include "scene.h"

#define RECT_VERTICES_ROWS       6
#define RECT_VERTICES_COLS       2
#define COORD_PER_POLYGON_VERTEX 2

array_t *SCENE_id(
    int id
) {
    return MAT_scalar_new((float)id);
}

void SCENE_clear_layer(
    int layer
) {
    for (int j=0; j<MAX_DRAWBLE_OBJECTS_ON_LAYER; j++) {
        scene->layers[layer].objs[j].texture       = -1;
        scene->layers[layer].objs[j].count         =  0;
        scene->layers[layer].objs[j].n_vertices    =  0;

        for (int u=0; u<MAX_SHADER_UNIFORMS; u++) {
            MAT_free(scene->layers[layer].objs[j].uniforms[u]);
        }
        
        MAT_free(scene->layers[layer].objs[j].vertices);

    }
    scene->layers[layer].n_objs      = -1;
}

void SCENE_clear(
) {
    for (int i=0; i<scene->n_layers; i++) {
        int objs = scene->layers[i].n_objs+1;

        for (int j=0; j<objs; j++) {
            int shader = scene->layers[i].objs[j].shader_id;
            int n_uniforms = RENDER_uniform_n(shader);

            scene->layers[i].objs[j].texture       = -1;
            scene->layers[i].objs[j].n_vertices    =  0;
            scene->layers[i].objs[j].count         =  0;
            
            for (int u=0; u<n_uniforms ; u++) {
                MAT_free(scene->layers[i].objs[j].uniforms[u]);
            }

            MAT_free(scene->layers[i].objs[j].vertices);
            scene->layers[i].objs[j].shader_id = -1;
        }
        scene->layers[i].n_objs            = -1;
    }
    scene->cur_layer                 = -1;
    
    for (int b=0; b<scene->n_buffers; b++) {
        RENDER_clear_buffer(scene->buffers[b]->id);
    }
}

void SCENE_clean_buffer(
    int buffer
) { 
    RENDER_clear_buffer(scene->buffers[buffer]->id);
}

void SCENE_add_layer(
    int   layer,
    char *name
) {
    if (layer>MAX_LAYERS_ON_SCENE || layer<0) { 
        printf("Requested layer exceeds has inproper index number! \n");
        return;
    }

    for (int j=0; j<MAX_DRAWBLE_OBJECTS_ON_LAYER; j++) {
        scene->layers[layer].objs[j].vertices      = NULL;

        for (int u=0; u<MAX_SHADER_UNIFORMS; u++) {
            scene->layers[layer].objs[j].uniforms[u] = NULL;
        }
    }
    scene->layers[layer].n_objs = -1;
    scene->layers[layer].on     = true;
    scene->layers[layer].name   = name;
    scene->n_layers++;
}

void SCENE_add_defalt_buffer(
) {
    // create default buffer
    scene->buffers[scene->n_buffers] = (framebuffer_t*)malloc(sizeof(framebuffer_t));

    scene->buffers[scene->n_buffers]->x0          = 0;
    scene->buffers[scene->n_buffers]->y0          = 0;
    scene->buffers[scene->n_buffers]->w           = FRAMEBUFFER_WIDTH;
    scene->buffers[scene->n_buffers]->h           = FRAMEBUFFER_HEIGHT;
    scene->buffers[scene->n_buffers]->texture     = 0;
    scene->buffers[scene->n_buffers]->id          = DEFAULT_FRAMEBUFFER;

    scene->n_buffers++;
}

void SCENE_add_buffer(
    int    id,
    int    w,
    int    h
) {
    scene->buffers[id] = GFX_create_framebuffer(id, w, h);
    scene->n_buffers++;
}

void SCENE_init(
) {
    scene             = NULL;
    scene             = (scene_t*)malloc(sizeof(scene_t));
    scene->n_layers   = 0;
    scene->n_buffers  = 0;
    scene->cur_layer  = -1;
    scene->cur_buffer = 0;

    for(int b=0; b<MAX_BUFFERS_ON_SCENE; b++) {
        scene->buffers[b] = NULL;
    }
}

void SCENE_activate_buffer(
    int buffer
) {
    scene->cur_buffer = buffer;
}

void SCENE_activate_layer(
    int         layer
) {
    scene->cur_layer = layer;
}

render_object_t* SCENE_cur_obj(
) {
    int i = scene->layers[scene->cur_layer].n_objs;

    if (i < 0) {
        return NULL;
    }

    return &(scene->layers[scene->cur_layer].objs[i]);
}

void SCENE_add_uniform(
    const char      *name,
    array_t         *arr
) {
    render_object_t* object = SCENE_cur_obj();
    if (!object) {
        printf("wrong object requested: \n"); 
    }

    int shader       = object->shader_id;
    if (shader<0) {
        printf("wrong shader requested: %d for object %s \n", shader, object->name); 
    }

    int i            = RENDER_get_uniform_index(shader, name);
    if (i<0) {
        printf("wrong uniform requested: %s \n", name); 
    }

    int size         = RENDER_get_uniform_size(shader, i);
    if (size<MAT_size(arr)) {
        printf("wrong size of uniform requested! \n"); 
    }

    object->uniforms[i] = arr;
}

render_object_t *SCENE_new_object(
) {
    scene->layers[scene->cur_layer].n_objs++;
    return SCENE_cur_obj();
}

void SCENE_free(
) {
    SCENE_clear();

    for (int i=0; i<MAX_LAYERS_ON_SCENE; i++) {
        
        // TODO: invent a way to safely destroy framebuffers
        // if (scene->layers[i].framebuffer != NULL) {
        //     GFX_destroy_framebuffer(scene->layers[i].framebuffer->id);
        //     scene->layers[i].framebuffer = NULL;
        // }
    }

    // free(scene);      
    // scene = NULL;
}

bool SCENE_layer_is_on(
    int      layer
) {
    return scene->layers[layer].on;
}

bool SCENE_layer_is_off(
    int      layer
) {
    return !(SCENE_layer_is_on(layer));
}

int SCENE_get_buffer_tex(
) {
    return scene->buffers[scene->cur_buffer]->texture;
}

int SCENE_get_buffer_width(
) {
    return scene->buffers[scene->cur_buffer]->w;
}

int SCENE_get_buffer_height(
) {
    return scene->buffers[scene->cur_buffer]->h;
}

array_t *SCENE_polygon_coord_to_matrix(
    float *coords,
    int    n_vertices
) {
    int len      = n_vertices * COORD_PER_POLYGON_VERTEX;
    int rows     = (int)len/COORD_PER_POLYGON_VERTEX;
    int cols     = (int)COORD_PER_POLYGON_VERTEX;

    array_t *arr = MAT_new(rows, cols);

    for (int i=0; i<len; i++) {
        arr->values[i]=(float)coords[i];
    }
    
    free(coords);

    return arr;
}

array_t *SCENE_coord_to_matrix(
    float x1, float y1,
    float x2, float y2
) {
    array_t *arr  = MAT_new(RECT_VERTICES_ROWS, RECT_VERTICES_COLS);
    
    arr->values[0 ] = x1; arr->values[1 ] = y2;
    arr->values[2 ] = x2; arr->values[3 ] = y2;
    arr->values[4 ] = x2; arr->values[5 ] = y1;
    arr->values[6 ] = x1; arr->values[7 ] = y2;
    arr->values[8 ] = x2; arr->values[9 ] = y1;
    arr->values[10] = x1; arr->values[11] = y1;

    return arr;
}

array_t *SCENE_set_scale(
) {
    int w = SCENE_get_buffer_width();
    int h = SCENE_get_buffer_height();

    return MAT_vec2_new(w, h);
}
array_t *SCENE_set_emit_pt(
    int x0,
    int y0
) {

    int w = SCENE_get_buffer_width();
    int h = SCENE_get_buffer_height();

    int emit_x = x0 + w / 2 - camera_x;
    int emit_y = y0 + h / 2 - camera_y;

    return MAT_vec2_new(emit_x, emit_y);
}

array_t* SCENE_set_color(
    int r, int g, int b, int a
) {
    return MAT_vec4_new((float)r/COLOR_COEF, (float)g/COLOR_COEF, (float)b/COLOR_COEF, (float)a/COLOR_COEF);
}

void SCENE_put_polygon_to_scene(
    array_t *vertices,
    int      x0,
    int      y0,
    int      r,
    int      g,
    int      b,
    int      a
) {
    render_object_t* object = SCENE_new_object();

    if (!object) {
        return;
    }

    object->shader_id  = SHADER_LIGHT;
    object->texture    = NO_TEXTURE;
    object->mode       = GL_POLYGON;
    
    // TODO: should this be done on higher function?
    object->vertices       = vertices;
    object->n_vertices     = MAT_n(vertices);
    object->count          = (int) object->n_vertices / vertices->cols;
    object->name           = "light polygon";

    array_t *scale_arr     = SCENE_set_scale();
    array_t *camera_arr    = MAT_vec2_new(camera_x, camera_y);
    array_t *color_arr     = SCENE_set_color(r, g, b, a);
    array_t *emit_arr      = SCENE_set_emit_pt(x0, y0);
    array_t *power_arr     = MAT_scalar_new(2.5);

    SCENE_add_uniform("aScale", scale_arr); 
    SCENE_add_uniform("aCamera", camera_arr); 
    SCENE_add_uniform("aColor", color_arr);
    SCENE_add_uniform("emit", emit_arr);
    SCENE_add_uniform("power", power_arr);
}

array_t *SCENE_texture_pos(
    int   draw_x, int   draw_y,
    int   draw_w, int   draw_h,
    int   clip_x, int   clip_y,
    int   clip_w, int   clip_h,
    int    tex_w, int    tex_h,
    bool  flip_w, bool  flip_h
) {
    int corr_w = (int)(flip_w) * clip_w;
    int corr_h = (int)(flip_h) * clip_h;
    
    array_t *pos_arr = SCENE_coord_to_matrix(
        (float)(draw_x +      0),
        (float)(draw_y +      0),
        (float)(draw_x + draw_w),
        (float)(draw_y + draw_h)
    );
    array_t *tex_arr = SCENE_coord_to_matrix(
        (float)(clip_x +      0 + corr_w) / (float)tex_w,
        (float)(clip_y + clip_h - corr_h) / (float)tex_h,
        (float)(clip_x + clip_w - corr_w) / (float)tex_w,
        (float)(clip_y +      0 + corr_h) / (float)tex_h
    );

    array_t *new = NULL;
    new          = MAT_join(pos_arr, tex_arr);

    return new;
}

// TODO: maybe type of texture here?
void SCENE_put_texture_to_scene(
    array_t *vertices,
    int      texture
) {
    render_object_t* object = SCENE_new_object();

    object->shader_id       = SHADER_TEXTURE;
    object->texture         = texture;
    object->mode            = GL_TRIANGLES;
    object->vertices        = vertices;
    object->n_vertices      = MAT_n(vertices);
    object->count           = (int) object->n_vertices / vertices->cols;
    object->name            = "texture";

    array_t *scale_arr      = SCENE_set_scale();
    array_t *texture_arr    = SCENE_id(texture);
    array_t *camera_arr     = MAT_vec2_new(camera_x, camera_y);

    SCENE_add_uniform("aScale", scale_arr); 
    SCENE_add_uniform("aCamera", camera_arr); 
    SCENE_add_uniform("ourTexture", texture_arr);
}

int SCENE_get_screen_multiplication_coef(
    float w, float h
) {
    int pix_scale_w = (int)FRAMEBUFFER_WIDTH / (int)w;
    int pix_scale_h = (int)FRAMEBUFFER_WIDTH / (int)h;

    return MIN(pix_scale_w, pix_scale_h);
}

// scale cur_buffer to fit on DEFAULT_FRAMEBUFFER (physical screen size) and put such texture on
// current layer
// TODO: use custom function and not SCENE_put_texture_to_scene
void SCENE_draw_scaled_buffer(
) {
    int buffer = scene->cur_buffer;

    // force camera to be zero
    camera_x = 0;
    camera_y = 0;
    
    int texture = SCENE_get_buffer_tex();

    int w       = SCENE_get_buffer_width();
    int h       = SCENE_get_buffer_height();
    int m       = SCENE_get_screen_multiplication_coef(w, h);
    int x0      = (FRAMEBUFFER_WIDTH-SCREEN_WIDTH*m)/2;
    int y0      = (FRAMEBUFFER_HEIGHT-SCREEN_HEIGHT*m)/2;

    SCENE_activate_buffer(DEFAULT_FRAMEBUFFER);

    array_t *vertices = SCENE_texture_pos(
        x0,             y0,
        SCREEN_WIDTH*m, SCREEN_HEIGHT*m,
        0,              0,
        SCREEN_WIDTH,   SCREEN_HEIGHT,
        SCREEN_WIDTH,   SCREEN_HEIGHT,
        false,          true
    );

    SCENE_put_texture_to_scene(vertices, texture);
    SCENE_activate_buffer(buffer);
}

void SCENE_render_current_layer(
) {
    int layer  = scene->cur_layer;
    int target = scene->cur_buffer;

    printf("\nLAYER: %s | n_objects: %d ", scene->layers[layer].name, scene->layers[layer].n_objs+1);
    if (SCENE_layer_is_off(layer)) {
        printf("objects to render: 0\n");
        return;
    }

    for (int i=0; i<scene->layers[layer].n_objs+1; i++) {
        printf("\n    OBJECT: %d | ", i);

        RENDER_set_viewport(
            scene->buffers[target]->id,
            scene->buffers[target]->w,
            scene->buffers[target]->h
        );

        RENDER_shader(
            scene->layers[layer].objs[i].shader_id,
            scene->layers[layer].objs[i].texture,
            scene->layers[layer].objs[i].vertices->values,
            scene->layers[layer].objs[i].n_vertices,
            scene->layers[layer].objs[i].uniforms,
            scene->layers[layer].objs[i].count,
            scene->layers[layer].objs[i].mode
        );
    }
}
