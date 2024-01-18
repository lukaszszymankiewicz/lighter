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

array_t SCENE_id(
    int id
) {
    return MAT_scalar_new((float)id);
}

int SCENE_cur_obj(
) {
    return scene->layers[scene->cur_layer].n_objs;
}

array_t* SCENE_get_current_object_vertices(
) {
    int cur_object = SCENE_cur_obj();

    if (cur_object == -1) {
        return NULL;
    }
    return &(scene->layers[scene->cur_layer].objs[cur_object].vertices);
}

void SCENE_clear_layer(
    int layer
) {
    printf("clearing for single layer %d \n", layer);
    for (int j=0; j<MAX_DRAWBLE_OBJECTS_ON_LAYER; j++) {
        scene->layers[layer].objs[j].texture       = -1;
        scene->layers[layer].objs[j].count         =  0;

        // clean uniforms
        for (int u=0; u<MAX_SHADER_UNIFORMS; u++) {
            if (scene->layers[layer].objs[j].uniforms[u] != NULL) {
                free(scene->layers[layer].objs[j].uniforms[u]);
                scene->layers[layer].objs[j].uniforms[u] = NULL;
            }
        }
        
        // if (scene->layers[layer].objs[j].vertices.values) {
        //     MAT_free(scene->layers[layer].objs[j].vertices);
        // }

        scene->layers[layer].objs[j].len           =  0;
        scene->layers[layer].objs[j].uniform_count =  -1;
    }
    scene->layers[layer].n_objs      = -1;
}

void SCENE_clear(
) {
    printf("SCENE CLEARING\n");
    printf("for %d layers \n", scene->n_layers);

    for (int i=0; i<scene->n_layers; i++) {

        int objs = scene->layers[i].n_objs+1;
        printf("layer: %d ", i);
        printf("clearing for %d objects \n", objs);

        for (int j=0; j<objs; j++) {
            scene->layers[i].objs[j].texture       = -1;
            scene->layers[i].objs[j].len           =  0;
            scene->layers[i].objs[j].count         =  0;
            
            // clean uniforms
            int uns = scene->layers[i].objs[j].uniform_count;
            printf("for object %d uniforms to clean: %d \n", j, uns);

            for (int u=0; u<uns; u++) {
                printf("    clearing for %d uniform \n", u);
                if (scene->layers[i].objs[j].uniforms[u] != NULL) {
                    free(scene->layers[i].objs[j].uniforms[u]);
                    scene->layers[i].objs[j].uniforms[u] = NULL;
                }
                printf("    cleared!\n");
            }
            
            scene->layers[i].objs[j].uniform_count = -1;

            // if (scene->layers[i].objs[j].vertices.values != NULL) {
            //     MAT_free(scene->layers[i].objs[j].vertices);
            // }

        }
        scene->layers[i].n_objs      = -1;
    }
    scene->cur_layer                 = -1;
    
    printf("EVETYTING CLEARED \n");
    printf("buffers n: %d", scene->n_buffers);
    for (int b=0; b<scene->n_buffers; b++) {
        RENDER_clear_buffer(scene->buffers[b]->id);
    }
    printf("BUFFER CLEARED \n");
}

void SCENE_clean_buffer(
    int buffer
) { 
    RENDER_clear_buffer(scene->buffers[buffer]->id);
}

void SCENE_add_layer(
    int layer
) {
    if (layer>MAX_LAYERS_ON_SCENE || layer<0) { 
        printf("Requested layer exceeds has inproper index number! \n");
        return;
    }

    for (int j=0; j<MAX_DRAWBLE_OBJECTS_ON_LAYER; j++) {
        // TODO: clean it!
        // scene->layers[layer].objs[j].vertices      = NULL;

        for (int u=0; u<MAX_SHADER_UNIFORMS; u++) {
            scene->layers[layer].objs[j].uniforms[u] = NULL;
        }

        scene->layers[layer].objs[j].uniform_count = -1;
    }
    scene->layers[layer].n_objs      = -1;
    scene->layers[layer].on = true;
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


int SCENE_cur_uniform(
) {
    int obj         = SCENE_cur_obj();
    int cur_uniform = scene->layers[scene->cur_layer].objs[obj].uniform_count;

    return cur_uniform;
}

void SCENE_add_uniform(
    array_t arr
) {
    int obj         = SCENE_cur_obj();
    int cur_uniform = SCENE_cur_uniform();
    
    scene->layers[scene->cur_layer].objs[obj].uniforms[cur_uniform] = arr.values;
    scene->layers[scene->cur_layer].objs[obj].uniform_count++;
    printf("\nuniform count increased for layer: %d and object: %d, now: %d\n", scene->cur_layer, obj, cur_uniform);
}

void SCENE_add_new_drawable_object(
) {
    scene->layers[scene->cur_layer].n_objs++;
}

void SCENE_set_texture(
    array_t arr
) {
    int obj = SCENE_cur_obj();

    scene->layers[scene->cur_layer].objs[obj].texture = (int)(arr.values[0]);
    SCENE_add_uniform(arr);
}

void SCENE_set_mode(
    GLuint     mode
) {
    int j=scene->layers[scene->cur_layer].n_objs;
    scene->layers[scene->cur_layer].objs[j].mode    = mode;
}

void SCENE_set_shader(
    int     shader_id
) {
    int j=scene->layers[scene->cur_layer].n_objs;
    scene->layers[scene->cur_layer].objs[j].shader_id    = shader_id;
}

void SCENE_add_vertices(
    array_t     arr
) {
    int j = scene->layers[scene->cur_layer].n_objs;
    int len = arr.rows * arr.cols;

    scene->layers[scene->cur_layer].objs[j].vertices = arr;

    // TODO: len and count should be deleted!
    scene->layers[scene->cur_layer].objs[j].len    = len;
    scene->layers[scene->cur_layer].objs[j].count  = arr.cols;
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

int SCENE_get_current_object_texture(
) {
    int cur_object = SCENE_cur_obj();
    if (cur_object == -1) {
        return -1;
    }
    return scene->layers[scene->cur_layer].objs[cur_object].texture;
}


bool SCENE_current_object_vertices_empty(
) {
    int cur_object = SCENE_cur_obj();
    if (cur_object == -1) {
        return true;
    }
    return scene->layers[scene->cur_layer].objs[cur_object].vertices.rows == 0;
}

array_t SCENE_polygon_coord_to_matrix(
    float *coords,
    int    len
) {
    array_t arr = MAT_new((int)len/COORD_PER_POLYGON_VERTEX, (int)COORD_PER_POLYGON_VERTEX);

    for (int i=0; i<len; i++) {
        arr.values[i]=(float)coords[i];
    }
    
    free(coords);

    return arr;
}

array_t coord_to_matrix(
    float x1, float y1,
    float x2, float y2
) {
    array_t arr = MAT_new(RECT_VERTICES_ROWS, RECT_VERTICES_COLS);
    
    arr.values[0 ] = x1; arr.values[1 ] = y2;
    arr.values[2 ] = x2; arr.values[3 ] = y2;
    arr.values[4 ] = x2; arr.values[5 ] = y1;
    arr.values[6 ] = x1; arr.values[7 ] = y2;
    arr.values[8 ] = x2; arr.values[9 ] = y1;
    arr.values[10] = x1; arr.values[11] = y1;

    return arr;
}

array_t SCENE_set_scale(
) {
    int w = SCENE_get_buffer_width();
    int h = SCENE_get_buffer_height();

    return MAT_vec2_new(w, h);
}

void SCENE_put_polygon_to_scene(
    float *vertices,
    int   n_vertices,
    int   x0,
    int   y0,
    int   r,
    int   g,
    int   b,
    int   a
) {
    int len     = n_vertices * COORD_PER_POLYGON_VERTEX;
    int buffer  = SCENE_get_buffer_tex();

    float r_gl = (float) r / COLOR_COEF;
    float g_gl = (float) g / COLOR_COEF;
    float b_gl = (float) b / COLOR_COEF;
    float a_gl = (float) a / COLOR_COEF;

    array_t pos_arr     = SCENE_polygon_coord_to_matrix(vertices, len);

    array_t scale_arr   = SCENE_set_scale();
    array_t color_arr   = MAT_vec4_new(r_gl, g_gl, b_gl, a_gl);
    array_t camera_arr  = MAT_vec2_new(camera_x, camera_y);
    array_t power_arr   = MAT_scalar_new(2.5);
    array_t emit_arr    = MAT_vec2_new((float)x0, (float)y0);
    
    // vertex
    SCENE_add_uniform(scale_arr);
    SCENE_add_uniform(camera_arr);

    // fragment
    int obj         = SCENE_cur_obj();

    SCENE_add_uniform(scale_arr);
    printf("scale_arr: \n");
    MAT_debug(scale_arr);

    SCENE_add_uniform(camera_arr);
    printf("camera_arr: \n");
    MAT_debug(camera_arr);

    SCENE_add_uniform(color_arr);
    printf("color_arr: \n");
    MAT_debug(color_arr);

    SCENE_add_uniform(emit_arr);
    printf("emit_arr: \n");
    MAT_debug(emit_arr);

    SCENE_add_uniform(scale_arr);
    printf("scale_arr: \n");
    MAT_debug(scale_arr);

    SCENE_add_uniform(camera_arr);
    printf("camera_arr: \n");
    MAT_debug(camera_arr);

    SCENE_add_uniform(power_arr);
    printf("power_arr: \n");
    MAT_debug(power_arr);

    SCENE_add_vertices(pos_arr);

    SCENE_set_mode(GL_POLYGON);
    SCENE_set_shader(SHADER_LIGHT);
}

void SCENE_put_texture_to_scene(
    int   draw_x, int   draw_y,
    int   draw_w, int   draw_h,
    int   clip_x, int   clip_y,
    int   clip_w, int   clip_h,
    int    tex_w, int    tex_h,
    bool  flip_w, bool  flip_h,
    int  texture
) {
    int corr_w = (int)(flip_w) * clip_w;
    int corr_h = (int)(flip_h) * clip_h;
    
    array_t pos_arr = coord_to_matrix(
        (float)(draw_x +      0),
        (float)(draw_y +      0),
        (float)(draw_x + draw_w),
        (float)(draw_y + draw_h)
    );
    array_t tex_arr = coord_to_matrix(
        (float)(clip_x +      0 + corr_w) / (float)tex_w,
        (float)(clip_y + clip_h - corr_h) / (float)tex_h,
        (float)(clip_x + clip_w - corr_w) / (float)tex_w,
        (float)(clip_y +      0 + corr_h) / (float)tex_h
    );
    MAT_join(&pos_arr, &tex_arr);
    // pos_arr was alocated here ^

    // still same texture is drawn - appending vertices
    if (SCENE_get_current_object_texture() == texture) {
        array_t* old_vertices = SCENE_get_current_object_vertices();
        // pos_arr is then freed here for the first time
        MAT_append(old_vertices, &pos_arr);

        return;
    } 

    SCENE_add_new_drawable_object();

    array_t scale_arr    = SCENE_set_scale();
    printf("texture %d \n", texture);
    array_t texture_arr  = SCENE_id(texture);
    array_t camera_arr   = MAT_vec2_new(camera_x, camera_y);

    SCENE_add_uniform(scale_arr);
    SCENE_add_uniform(camera_arr);
    SCENE_set_texture(texture_arr);
    SCENE_add_vertices(pos_arr);
    SCENE_set_mode(GL_TRIANGLES);
    SCENE_set_shader(SHADER_TEXTURE);
}

int SCENE_vertices_n(
    int layer,
    int n
) {
    shader_params_t obj = scene->layers[layer].objs[n];
    return obj.vertices.rows * obj.vertices.cols;
}

int SCENE_vertices_count(
    int layer,
    int n
) {
    shader_params_t obj = scene->layers[layer].objs[n];
    return obj.vertices.cols;
}

int SCENE_get_screen_multiplication_coef(
    float w, float h
) {
    int pix_scale_w = (int)FRAMEBUFFER_WIDTH / (int)w;
    int pix_scale_h = (int)FRAMEBUFFER_WIDTH / (int)h;

    return MIN(pix_scale_w, pix_scale_h);
}

// scale cur_buffer to fit on DEFAULT_FRAMEBUFFER (physical screen size) and put such texture on
// crrent layer
// TODO: use custom function and not SCENE_put_texture_to_scene
void SCENE_draw_scaled_buffer(
) {
    int buffer = scene->cur_buffer;

    // force camera to be zero
    camera_x = 0; camera_y = 0;
    
    int texture = SCENE_get_buffer_tex();

    int w       = SCENE_get_buffer_width();
    int h       = SCENE_get_buffer_height();
    int m       = SCENE_get_screen_multiplication_coef(w, h);
    int x0      = (FRAMEBUFFER_WIDTH-SCREEN_WIDTH*m)/2;
    int y0      = (FRAMEBUFFER_HEIGHT-SCREEN_HEIGHT*m)/2;

    SCENE_activate_buffer(DEFAULT_FRAMEBUFFER);

    SCENE_put_texture_to_scene(
        x0, y0,
        SCREEN_WIDTH*m, SCREEN_HEIGHT*m,
        0, 0,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        false, true,
        texture
    );

    SCENE_activate_buffer(buffer);
}

void SCENE_render_current_layer(
) {
    int layer  = scene->cur_layer;
    int target = scene->cur_buffer;

    printf("\nLAYER: %d | n_objects: %d ", layer, scene->layers[layer].n_objs+1);
    if (SCENE_layer_is_off(layer)) {
        printf("NOT DRAWING ANYTHING\n");
        printf("objects to render: 0\n");
        return;
    }

    for (int i=0; i<scene->layers[layer].n_objs+1; i++) {
        printf("\n    OBJECT: %d | ", i);
        printf("vertices: %d | ", SCENE_vertices_n(layer, i));

        RENDER_set_viewport(
            scene->buffers[target]->id,
            scene->buffers[target]->w,
            scene->buffers[target]->h
        );

        RENDER_shader(
            scene->layers[layer].objs[i].shader_id,
            scene->layers[layer].objs[i].texture,
            scene->layers[layer].objs[i].vertices.values,
            SCENE_vertices_n(layer, i),
            scene->layers[layer].objs[i].uniforms,
            SCENE_vertices_count(layer, i),
            scene->layers[layer].objs[i].mode
        );
    }
}
