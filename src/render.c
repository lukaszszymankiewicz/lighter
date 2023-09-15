#include "./data/library.h"

#include <GL/glew.h>

#include <stdio.h>
#include <stdlib.h>

#include "gfx.h"
#include "render.h"

// ther are only few uniform types valid for this engine
int RENDER_set_uniform(
    int    uniform_id,
    int    type,
    int    i,
    float *uniforms
) {
    printf("uniform id :%d ", uniform_id);

    int size       = GFX_type_size(type);

    if (size == 2) {
        glUniform2f(uniform_id, uniforms[i], uniforms[i+1]);
        printf("values: %f %f\n", uniforms[i], uniforms[i+1]);
    } else if (size == 3) {
        glUniform3f(uniform_id, uniforms[i], uniforms[i+1], uniforms[i+2]);
        printf("values: %f %f %f\n", uniforms[i], uniforms[i+1], uniforms[i+2]);
    } else if (size == 4) {
        glUniform4f(uniform_id, uniforms[i], uniforms[i+1], uniforms[i+2], uniforms[i+3]);
        printf("values: %f %f %f %f\n", uniforms[i], uniforms[i+1], uniforms[i+2], uniforms[i+3]);
    } else {
        glUniform1f(uniform_id, uniforms[i]);
        printf("values: %f \n", uniforms[i]);
    }

    return i + size;
}

void RENDER_set_uniforms(
    int      shader,
    float   *uniforms
) {
    int i=0;

    for (int u=0; u<shader_library[shader]->n_uniforms; u++) { 
        int uniform_id = shader_library[shader]->uniform_loc[u];
        int type       = shader_library[shader]->uniform_types[u];

        int shift = RENDER_set_uniform(uniform_id, type, i, uniforms);
        i = shift;
    }
}

void RENDER_shader(
    int      shader,
    int      texture,
    GLfloat *vertices,
    int      n_vertices,
    float   *uniforms,
    int      count,
    GLenum   mode
) { 
    glBindVertexArray(shader_library[shader]->vao);
    glBindBuffer(GL_ARRAY_BUFFER, shader_library[shader]->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * n_vertices, vertices, GL_STATIC_DRAW);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    RENDER_set_uniforms(shader, uniforms);
    glDrawArrays(mode, 0, (int)(n_vertices/count));

    // cleanup
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

// TODO: each shader should have its own small function to render it
// void RENDER_shader_light(
//     int      shader,
//     int      texture,
//     GLfloat *vertices,
//     int      n_vertices,
//     float   *uniforms
// ) { 
//     if (texture != NO_TEXTURE) {
//         glBindTexture(GL_TEXTURE_2D, texture);
//     }
//       
//     glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * n_vertices, vertices, GL_STATIC_DRAW);
//     
//     int i=0;
//     for (int u=0; u<shader_library[shader]->n_uniforms; u++) { 
//         int uniform_id = shader_library[shader]->uniform_loc[u];
//         glUniform4f(uniform_id, uniforms[i], uniforms[i+1], uniforms[i+2], uniforms[i+3]);
//         printf("uniform id :%d ", uniform_id);
//         printf("uniforms: %f %f %f %f \n", uniforms[i], uniforms[i+1], uniforms[i+2], uniforms[i+3]);
//         i+=MAX_SHADER_UNIFORMS_ARGS;
//     }
// 
//     int cur_shift = 0;
//     int size      = shader_library[shader]->attrib_size;
// 
//     for (int i=0; i<shader_library[shader]->n_attribs; i++) {
//         GLint attrib = shader_library[shader]->attrib_loc[i];
//         glEnableVertexAttribArray(attrib);
// 
//         glVertexAttribPointer(
//             attrib,
//             shader_library[shader]->attrib_stride[i],
//             GL_FLOAT,
//             GL_FALSE,
//             (size*sizeof(float)),
//             (void*)(cur_shift*sizeof(float))
//         );
// 
//         cur_shift+=shader_library[shader]->attrib_stride[i];
//     }
// 
//     glDrawArrays(GL_POLYGON, 0, (int)(n_vertices/size));
// };
// 
