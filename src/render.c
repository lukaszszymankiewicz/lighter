#include "./data/library.h"

#include <GL/glew.h>

#include <stdio.h>

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
