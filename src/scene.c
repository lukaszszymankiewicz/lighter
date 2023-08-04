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
    scene_t *scene,
    int      type
) {
    scene->layers[scene->n_layers].on   = true;
    scene->layers[scene->n_layers].type = type;
    scene->n_layers++;
}

scene_t* SCENE_new(
    int n_tile_layers,
    int n_shader_layers
) {
    scene_t *scene   = NULL;
    scene            = (scene_t*)malloc(sizeof(scene_t));

    SCENE_clear(scene);

    for(int l=0; l<n_tile_layers; l++) {
         SCENE_add_layer(scene, LAYER_TYPE_TEXTURE);
    }

    for(int l=0; l<n_shader_layers; l++) {
         SCENE_add_layer(scene, LAYER_TYPE_SHADER);
    }

    return scene;
}

void SCENE_add_texture(
    scene_t       *scene,
    int            layer,
    int            texture_id,
    render_coord_t render,
    render_coord_t clip,
    bool           flip_w,
    bool           flip_h
) {
    int idx = scene->layers[layer].n_objs;

    if (idx >= MAX_SPRITES_ON_SPRITES_LAYER) { return; }

    scene->layers[layer].objs[idx].texture.texture_id = texture_id;
    scene->layers[layer].objs[idx].texture.render     = render;
    scene->layers[layer].objs[idx].texture.clip       = clip;
    scene->layers[layer].objs[idx].texture.flip_w     = flip_w;
    scene->layers[layer].objs[idx].texture.flip_h     = flip_w;

    scene->layers[layer].n_objs++;
}

void SCENE_add_shader(
    scene_t    *scene,
    int         layer,
    int         program_id,
    int         len,
    int         size,
    GLfloat    *vertices,
    float      *uniforms
) {
    int i;
    int j=scene->layers[layer].n_objs;

    if (j>=MAX_SHADER_ON_SHADER_LAYER) { return; }

    scene->layers[layer].objs[j].shader.program_id = program_id;
    scene->layers[layer].objs[j].shader.len        = len;
    scene->layers[layer].objs[j].shader.size       = size;

    for (i=0; i<len; i++) {
        scene->layers[layer].objs[j].shader.vertices[i] = vertices[i];
    }
    if (uniforms) {
        for (i=0; i<MAX_SHADER_UNIFORMS_ARGS; i++) {
            scene->layers[layer].objs[j].shader.uniforms[i] = uniforms[i];
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
        
        if (scene->layers[layer].type == LAYER_TYPE_TEXTURE) {
            for (int i=0; i<scene->layers[layer].n_objs; i++) {
                RENDER_texture(
                    tilesets_library[scene->layers[layer].objs[i].texture.texture_id]->id,
                    scene->layers[layer].objs[i].texture.render.x1,
                    scene->layers[layer].objs[i].texture.render.y1,
                    scene->layers[layer].objs[i].texture.render.x2,
                    scene->layers[layer].objs[i].texture.render.y2,
                    scene->layers[layer].objs[i].texture.clip.x1,
                    scene->layers[layer].objs[i].texture.clip.y1,
                    scene->layers[layer].objs[i].texture.clip.x2,
                    scene->layers[layer].objs[i].texture.clip.y2,
                    scene->layers[layer].objs[i].texture.flip_w
                );
            }

        } else {
            for (int i=0; i<scene->layers[layer].n_objs; i++) {
                RENDER_shader(
                    scene->layers[layer].objs[i].shader.program_id,
                    scene->layers[layer].objs[i].shader.vertices,
                    scene->layers[layer].objs[i].shader.len,
                    scene->layers[layer].objs[i].shader.size,
                    scene->layers[layer].objs[i].shader.uniforms
                );
            }
        }
    }
}
