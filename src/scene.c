#include "data/library.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "gfx.h"
#include "render.h"
#include "scene.h"

scene_t *scene = NULL;

void SCENE_clear(
    scene_t* scene
) {
    for (int i=0; i<MAX_LAYERS_ON_SCENE; i++) {
        int n_objs = scene->layers[i].n_objs;

        for (int j=0; j<n_objs; j++) {
            scene->layers[i].objs[j].len        =  0;
            scene->layers[i].objs[j].texture    = -1;
            scene->layers[i].objs[j].count      =  0;
            memset(scene->layers[i].objs[j].uniforms, 0.0, sizeof(float)*MAX_SHADER_UNIFORMS_ARGS_LEN);
            
            if (scene->layers[i].objs[j].vertices != NULL) {
                free(scene->layers[i].objs[j].vertices);
            }
            scene->layers[i].objs[j].vertices   = NULL;
        }
        scene->layers[i].n_objs      = 0;
    }
}

void SCENE_add_layer(
    scene_t* scene,
    int      i
) {
    for (int j=0; j<MAX_DRAWBLE_OBJECTS_ON_LAYER; j++) {
        scene->layers[i].objs[j].vertices   = NULL;
    }
    scene->layers[i].on          = true;
    scene->layers[i].n_objs      = 0;
}

void SCENE_add_buffer(
    scene_t* scene
) {
    // scene->buffer = GFX_create_framebuffer();
}

scene_t* SCENE_new(
    int n_layers
) {
    scene_t *scene   = NULL;
    scene            = (scene_t*)malloc(sizeof(scene_t));
    scene->n_layers  = 0;
    scene->buffer    = 0;

    for(int l=0; l<n_layers; l++) {
        SCENE_add_layer(scene, scene->n_layers++);
    }

    SCENE_clear(scene);

    return scene;
}

void SCENE_attach_shader(
    int layer, int shader_id, GLenum mode
) {
    scene->layers[layer].on        = true;
    scene->layers[layer].shader_id = shader_id;
    scene->layers[layer].mode      = mode;
}

void SCENE_add(
    scene_t    *scene,
    int         layer,
    int         texture,
    int         len,
    GLfloat    *vertices,
    float      *uniforms,
    int         count
) {
    int i;
    int j=scene->layers[layer].n_objs;

    if (j>=MAX_SHADER_ON_SHADER_LAYER) { return; }

    if (vertices) {
        scene->layers[layer].objs[j].vertices = vertices;
    }
    if (uniforms) {
        for (i=0; i<MAX_SHADER_UNIFORMS_ARGS_LEN; i++) {
            scene->layers[layer].objs[j].uniforms[i] = uniforms[i];
        }
    }

    scene->layers[layer].objs[j].len     = len;
    scene->layers[layer].objs[j].texture = texture;
    scene->layers[layer].objs[j].count   = count;

    scene->layers[layer].n_objs++;
}

void SCENE_free(
    scene_t* scene
) {
    GFX_destroy_framebuffer(scene->buffer);

    for (int i=0; i<MAX_LAYERS_ON_SCENE; i++) {
        int n_objs = scene->layers[i].n_objs;

        for (int j=0; j<n_objs; j++) {
            free(scene->layers[i].objs[j].vertices);
            scene->layers[i].objs[j].vertices   = NULL;
        }
    }
    free(scene);      
    scene = NULL;
}

void SCENE_use_program(
    int layer
) {
    int id = shader_library[scene->layers[layer].shader_id]->program;
    GFX_use_shader_program(id);
}

bool SCENE_layer_is_on(
    scene_t *scene,
    int      layer
) {
    return scene->layers[layer].on;
}

bool SCENE_layer_is_off(
    scene_t *scene,
    int      layer
) {
    return !(SCENE_layer_is_on(scene, layer));
}

void SCENE_draw(
    scene_t* scene 
) {
    // no buffer to render to!
    if (!(scene->buffer)) {
        // return;
    }
    
    GFX_bind_framebuffer(scene->buffer);

    for (int layer=0; layer<scene->n_layers; layer++) {
        if (SCENE_layer_is_off(scene, layer)) {
            continue;
        }
        SCENE_use_program(layer);

        for (int i=0; i<scene->layers[layer].n_objs; i++) {
            RENDER_shader(
                scene->layers[layer].shader_id,
                scene->layers[layer].objs[i].texture,
                scene->layers[layer].objs[i].vertices,
                scene->layers[layer].objs[i].len,
                scene->layers[layer].objs[i].uniforms,
                scene->layers[layer].objs[i].count,
                scene->layers[layer].mode
            );
        }
    }
}
