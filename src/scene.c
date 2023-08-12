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
    for (int i=0; i<MAX_LAYERS_ON_SCENE; i++) {
        scene->layers[i].n_objs     = 0;
    }
}

scene_t* SCENE_new(
    int n_layers
) {
    scene_t *scene   = NULL;
    scene            = (scene_t*)malloc(sizeof(scene_t));
    scene->n_layers  = 0;

    SCENE_clear(scene);

    for(int l=0; l<n_layers; l++) {
         scene->n_layers++;
    }

    return scene;
}

void SCENE_attach_shader(
    int layer, int shader_id
) {
    scene->layers[layer].on        = true;
    scene->layers[layer].shader_id = shader_id;
}

void SCENE_set_uniforms(
    scene_t    *scene,
    int         layer,
    float      *uniforms
) {
    int j = scene->layers[layer].n_objs;

    if (uniforms) {
        for (int i=0; i<MAX_SHADER_UNIFORMS_ARGS; i++) {
            scene->layers[layer].objs[j].uniforms[i] = uniforms[i];
        }
    }
}

void SCENE_append_vertices(
    scene_t    *scene,
    int         layer,
    int         len,
    GLfloat    *vertices
) {
    int j=scene->layers[layer].n_objs;
    int cur_len = scene->layers[layer].objs[j].len;

    scene->layers[layer].objs[j].len += len;

    if (vertices) {
        for (int i=0; i<len; i++) {
            scene->layers[layer].objs[j].vertices[cur_len+i] = vertices[i];
        }
    }
}

void SCENE_add(
    scene_t    *scene,
    int         layer,
    int         texture,
    int         len,
    GLfloat    *vertices,
    float      *uniforms
) {
    int i;
    int j=scene->layers[layer].n_objs;

    if (j>=MAX_SHADER_ON_SHADER_LAYER) { return; }

    scene->layers[layer].objs[j].len        = len;
    scene->layers[layer].objs[j].texture    = texture;

    if (vertices) {
        for (i=0; i<len; i++) {
            scene->layers[layer].objs[j].vertices[i] = vertices[i];
        }
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

void SCENE_use_program(
    int layer
) {
    glUseProgram(shader_library[scene->layers[layer].shader_id]->program);
}

void SCENE_draw(
    scene_t* scene 
) {
    for (int layer=0; layer<scene->n_layers; layer++) {
        if (!(scene->layers[layer].on)) {
            continue;
        }
        SCENE_use_program(layer);

        for (int i=0; i<scene->layers[layer].n_objs; i++) {
            RENDER_shader_texture(
                scene->layers[layer].shader_id,
                scene->layers[layer].objs[i].texture,
                scene->layers[layer].objs[i].vertices,
                scene->layers[layer].objs[i].len,
                scene->layers[layer].objs[i].uniforms
            );
        }
    }
}
