#include <GL/glew.h>
#include <GL/gl.h>

#include "data/library.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "gl_util.h"
#include "render.h"
#include "scene.h"

scene_t *scene = NULL;

void SCENE_clear(
    scene_t* scene
) {
    scene->n_layers=0;

    for (int i=0; i<MAX_LAYERS_ON_SCENE; i++) {
        scene->layers[i].on         = false;
        scene->layers[i].n_objs     = 0;
        scene->layers[i].program_id = 0;
    }
}

void SCENE_add_layer(
    scene_t *scene
) {
    scene->layers[scene->n_layers++].on   = true;
}

scene_t* SCENE_new(
    int n_layers
) {
    scene_t *scene   = NULL;
    scene            = (scene_t*)malloc(sizeof(scene_t));

    SCENE_clear(scene);

    for(int l=0; l<n_layers; l++) {
         SCENE_add_layer(scene);
    }

    return scene;
}

void SCENE_add_shader(
    scene_t    *scene,
    int         layer,
    int         texture,
    int         shader_id,
    int         len,
    GLfloat    *vertices,
    float      *uniforms
) {
    int i;
    int j=scene->layers[layer].n_objs;

    if (j>=MAX_SHADER_ON_SHADER_LAYER) { return; }

    scene->layers[layer].objs[j].program_id = shader_id;
    scene->layers[layer].objs[j].len        = len;
    scene->layers[layer].objs[j].texture    = texture;

    for (i=0; i<len; i++) {
        scene->layers[layer].objs[j].vertices[i] = vertices[i];
    }
    if (uniforms) {
        for (i=0; i<MAX_SHADER_UNIFORMS_ARGS; i++) {
            scene->layers[layer].objs[j].uniforms[i] = uniforms[i];
        }
    }

    scene->layers[layer].n_objs++;
}

void SCENE_free(
    scene_t* scene
) {
    free(scene);      
    scene = NULL;
}

void SCENE_draw(
    scene_t* scene 
) {
    for (int layer=0; layer<scene->n_layers; layer++) {
        glUseProgram(scene->layers[layer].program_id);
        
        for (int i=0; i<scene->layers[layer].n_objs; i++) {
            RENDER_shader(
                scene->layers[layer].objs[i].program_id,
                scene->layers[layer].objs[i].texture,
                scene->layers[layer].objs[i].vertices,
                scene->layers[layer].objs[i].len,
                scene->layers[layer].objs[i].uniforms
            );
        }
    }
}
