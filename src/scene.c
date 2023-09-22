#include "data/library.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "gfx.h"
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
                free(scene->layers[i].objs[j].vertices);
            }
            scene->layers[i].objs[j].vertices   = NULL;

        }
        scene->layers[i].n_objs      = -1;
    }
    scene->cur_layer                 = -1;
}

void SCENE_add_layer(
    int      i
) {
    for (int j=0; j<MAX_DRAWBLE_OBJECTS_ON_LAYER; j++) {
        scene->layers[i].objs[j].vertices      = NULL;
        scene->layers[i].objs[j].uniform_count = 0;

        for (int u=0; u<MAX_SHADER_UNIFORMS; u++) {
            scene->layers[i].objs[j].uniforms[u] = NULL;
        }
    }

    scene->layers[i].n_objs          = -1;
    scene->layers[i].on              = true;
    scene->layers[i].framebuffer     = NULL;
    scene->layers[i].framebuffer     = (framebuffer_t*)malloc(sizeof(framebuffer_t));

    scene->layers[i].framebuffer->id      = 0;
    scene->layers[i].framebuffer->texture = 0;
}

void SCENE_add_buffer_layer(
    int      i
) {
    SCENE_add_layer(scene, i);
    // delete previous framebuffer data
    free(scene->layers[i].framebuffer);

    // create new framebuffer for current layer
    scene->layers[i].framebuffer = NULL;
    scene->layers[i].framebuffer = GFX_create_framebuffer();

    int new_id = scene->layers[i].framebuffer->id;

    // update previous layers
    for (int l=0; l<i; l++) {
        scene->layers[l].framebuffer->id = new_id;
    }
}

void SCENE_new(
) {
    scene            = (scene_t*)malloc(sizeof(scene_t));
    scene->n_layers  = 0;
    scene->cur_layer = -1;
}

void SCENE_attach_shader(
    int layer,
    int shader_id,
    int mode
) {
    scene->layers[layer].shader_id = shader_id;
    scene->layers[layer].mode      = mode;
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
    int obj = SCENE_cur_obj(scene);
    int cur_uniform =  scene->layers[scene->cur_layer].objs[obj].uniform_count;

    return cur_uniform;
}

void SCENE_add_uniform(
    float      *value
) {
    int obj         = SCENE_cur_obj(scene);
    int cur_uniform = SCENE_cur_uniform(scene);

    scene->layers[scene->cur_layer].objs[obj].uniforms[cur_uniform] = value;
    scene->layers[scene->cur_layer].objs[obj].uniform_count++;
}

void SCENE_add_new_drawable_object(
) {
    scene->layers[scene->cur_layer].n_objs++;
}

void SCENE_set_texture(
    float      *texture
) {
    int obj = SCENE_cur_obj(scene);

    scene->layers[scene->cur_layer].objs[obj].texture = (int)(texture[0]);
    SCENE_add_uniform(scene, texture);
}

void SCENE_add_vertices(
    int         len,
    float      *vertices,
    int         val_per_vertex
) {
    int j=scene->layers[scene->cur_layer].n_objs;

    if (vertices) {
        scene->layers[scene->cur_layer].objs[j].vertices = vertices;
    }

    scene->layers[scene->cur_layer].objs[j].len     = len;
    scene->layers[scene->cur_layer].objs[j].count   = count;
}

void SCENE_free(
) {
    for (int i=0; i<MAX_LAYERS_ON_SCENE; i++) {
        int n_objs = scene->layers[i].n_objs;

        for (int j=0; j<n_objs; j++) {
            free(scene->layers[i].objs[j].vertices);
            scene->layers[i].objs[j].vertices   = NULL;
        }

        if (scene->layers[i].framebuffer != NULL) {
            GFX_destroy_framebuffer(scene->layers[i].framebuffer->id);
            scene->layers[i].framebuffer = NULL;
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
    int      layer
) {
    return scene->layers[layer].on;
}

bool SCENE_layer_is_off(
    int      layer
) {
    return !(SCENE_layer_is_on(scene, layer));
}

void SCENE_use_buffer(
    int layer
) {
    int id = scene->layers[layer].framebuffer->id;
    GFX_bind_framebuffer(id);
}

void SCENE_draw(
) {
    for (int layer=0; layer<scene->n_layers; layer++) {
        if (SCENE_layer_is_off(layer)) {
            continue;
        }

        SCENE_use_buffer(layer);
        SCENE_use_program(layer);

        for (int i=0; i<scene->layers[layer].n_objs+1; i++) {
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

    // GLuint vaoCube, vaoQuad;
    // glGenVertexArrays(1, &vaoQuad);
    // glBindVertexArray(vaoQuad);

    // SCENE_use_program(0);

    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, texColorBuffer);

    // glDrawArrays(GL_TRIANGLES, 0, 6);
}
