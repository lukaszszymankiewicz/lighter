#include "data/library.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "gfx.h"
#include "mat.h"
#include "global.h"
#include "render.h"
#include "scene.h"

scene_t *scene = NULL;

void SCENE_clear(
) {
    for (int i=0; i<scene->n_layers; i++) {
        for (int j=0; j<MAX_DRAWBLE_OBJECTS_ON_LAYER; j++) {
            scene->layers[i].objs[j].texture       = -1;
            scene->layers[i].objs[j].len           =  0;
            scene->layers[i].objs[j].count         =  0;
            scene->layers[i].objs[j].uniform_count =  0;
            
            // clean uniforms
            for (int u=0; u<MAX_SHADER_UNIFORMS; u++) {
                if (scene->layers[i].objs[j].uniforms[u] != NULL) {
                    free(scene->layers[i].objs[j].uniforms[u]);
                    scene->layers[i].objs[j].uniforms[u] = NULL;
                }
            }
            
            // clean vertices
            if (scene->layers[i].objs[j].vertices != NULL) {
                // free(scene->layers[i].objs[j].vertices);
            }
            scene->layers[i].objs[j].vertices   = NULL;

        }
        scene->layers[i].n_objs      = -1;
    }
    scene->cur_layer                 = -1;
    
    for (int l=0; l<scene->n_layers; l++) {
        RENDER_clear_buffer(scene->layers[l].framebuffer->id);
    }
}

void SCENE_add_layer(
    int layer,
    int shader_id,
    int mode
) {
    if (layer>MAX_LAYERS_ON_SCENE || layer<0) { 
        printf("Requested layer exceeds has inproper index number! \n");
        return;
    }

    for (int j=0; j<MAX_DRAWBLE_OBJECTS_ON_LAYER; j++) {
        scene->layers[layer].objs[j].vertices      = NULL;
        scene->layers[layer].objs[j].uniform_count = 0;

        for (int u=0; u<MAX_SHADER_UNIFORMS; u++) {
            scene->layers[layer].objs[j].uniforms[u] = NULL;
        }
    }

    scene->layers[layer].n_objs          = -1;
    scene->layers[layer].on              = true;

    scene->layers[layer].framebuffer          = NULL;
    scene->layers[layer].framebuffer          = (framebuffer_t*)malloc(sizeof(framebuffer_t));
    scene->layers[layer].framebuffer->id      = DEFAULT_FRAMEBUFFER;
    scene->layers[layer].framebuffer->texture = NO_TEXTURE;
    scene->layers[layer].framebuffer->x0      = 0;
    scene->layers[layer].framebuffer->y0      = 0;
    scene->layers[layer].framebuffer->w       = framebuffer_w;
    scene->layers[layer].framebuffer->h       = framebuffer_h;

    scene->layers[layer].shader_id = shader_id;
    scene->layers[layer].mode      = mode;

    scene->n_layers++;
}

void SCENE_add_buffer_layer(
    int layer,
    int shader_id,
    int mode
) {
    // create basic layer
    SCENE_add_layer(layer, shader_id, mode);
    
    // delete previous framebuffer data
    free(scene->layers[layer].framebuffer);

    // create new framebuffer for current layer
    scene->layers[layer].framebuffer    = NULL;
    scene->layers[layer].framebuffer    = GFX_create_framebuffer();

    int new_id = scene->layers[layer].framebuffer->id;

    // update previous layers
    for (int l=0; l<layer; l++) {
        scene->layers[l].framebuffer->id = new_id;
        scene->layers[l].framebuffer->w  = SCREEN_WIDTH;
        scene->layers[l].framebuffer->h  = SCREEN_HEIGHT;
    }
    scene->layers[layer].framebuffer->id = DEFAULT_FRAMEBUFFER;
}

void SCENE_new(
) {
    scene            = (scene_t*)malloc(sizeof(scene_t));
    scene->n_layers  = 0;
    scene->cur_layer = -1;
}

void SCENE_activate_layer(
    int         layer
) {
    scene->cur_layer = layer;
}

int SCENE_cur_obj(
) {
    return scene->layers[scene->cur_layer].n_objs;
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

// TODO: here we can also introduce array
void SCENE_add_vertices(
    int         len,
    float      *vertices,
    int         val_per_vertex
) {
    int j=scene->layers[scene->cur_layer].n_objs;

    if (vertices) {
        for (int l=0; l<len; l++) {
            scene->layers[scene->cur_layer].objs[j].vertices = vertices;
        }
    }

    scene->layers[scene->cur_layer].objs[j].len     = len;
    scene->layers[scene->cur_layer].objs[j].count   = val_per_vertex;
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

    free(scene);      
    scene = NULL;
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

int SCENE_get_layer_buffer_tex(
    int layer
) {
    return scene->layers[layer].framebuffer->texture;
}

void SCENE_draw(
) {
    for (int layer=0; layer<scene->n_layers; layer++) {
        if (SCENE_layer_is_off(layer)) {
            continue;
        }
        printf("\nLAYER: %d ~~~~~~~~~~~~~~~~\n", layer);
        printf("objects to render: %d\n", scene->layers[layer].n_objs+1);

        for (int i=0; i<scene->layers[layer].n_objs+1; i++) {
            printf("OBJECT %d \n", i);
            printf("vertices to render: %d\n", scene->layers[layer].objs[i].len);
            RENDER_shader(
                scene->layers[layer].shader_id,
                scene->layers[layer].objs[i].texture,
                scene->layers[layer].objs[i].vertices,
                scene->layers[layer].objs[i].len,
                scene->layers[layer].objs[i].uniforms,
                scene->layers[layer].objs[i].count,
                scene->layers[layer].mode,
                scene->layers[layer].framebuffer->id,
                scene->layers[layer].framebuffer->w,
                scene->layers[layer].framebuffer->h
            );
        }
    }
}
