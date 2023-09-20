#include "./data/library.h"

#include <GL/glew.h>
#include <GL/gl.h>

#include <stdbool.h>
#include <stdio.h>

#include "gfx.h"
#include "render.h"

void RENDER_set_uniform(
    int    uniform_id,
    int    type,
    float *uniform
) {
    switch (type)
    {
        case GL_FLOAT_VEC2:
            glUniform2f(uniform_id, uniform[0], uniform[1]);
            printf("values: %f %f\n", uniform[0], uniform[1]);
            break;
        case GL_FLOAT_VEC3:
            glUniform3f(uniform_id, uniform[0], uniform[1], uniform[2]);
            printf("values: %f %f %f\n", uniform[0], uniform[1], uniform[2]);
            break;
        case GL_FLOAT_VEC4:
            glUniform4f(uniform_id, uniform[0], uniform[1], uniform[2], uniform[3]);
            printf("values: %f %f %f %f\n", uniform[0], uniform[1], uniform[2], uniform[3]);
            break;
        case GL_SAMPLER_2D:
            glUniform1f(uniform_id, uniform[0]);
            printf("values: %f \n", uniform[0]);
            break;
        case GL_FLOAT_MAT2:
            glUniformMatrix2fv(uniform_id, 1, GL_FALSE, &uniform[0]);
            printf("values: %f %f %f %f\n", uniform[0], uniform[1], uniform[2], uniform[3]);
            break;
        default:
            break;
    }
}

void RENDER_set_uniforms(
    int      shader,
    float   *uniforms[]
) {
    for (int u=0; u<shader_library[shader]->n_uniforms; u++) { 
        int   uniform_id = shader_library[shader]->uniform_loc[u];
        int   type       = shader_library[shader]->uniform_types[u];
        char *name       = shader_library[shader]->uniform_names[u];

        printf("uniform (%s) with id (%d) ", name, uniform_id);
        RENDER_set_uniform(uniform_id, type, uniforms[u]);
    }
}

void RENDER_shader(
    int      shader,
    int      texture,
    GLfloat *vertices,
    int      n_vertices,
    float   *uniforms[MAX_SHADER_UNIFORMS],
    int      count,
    GLenum   mode
) { 
    printf("rendering program %d --------- \n", shader_library[shader]->program);
    glBindVertexArray(shader_library[shader]->vao);
    glBindBuffer(GL_ARRAY_BUFFER, shader_library[shader]->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*n_vertices, vertices, GL_STATIC_DRAW);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    RENDER_set_uniforms(shader, uniforms);
    glDrawArrays(mode, 0, (int)(n_vertices/count));

    // cleanup
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
