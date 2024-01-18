#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <stdio.h>

#include "./data/library.h"

#include "global.h"
#include "render.h"

void RENDER_set_uniform(
    int    uniform_id,
    int    type,
    float *uniform
) {
    switch (type)
    {
        case GL_FLOAT:
            glUniform1f(uniform_id, uniform[0]);
            break;
        case GL_FLOAT_VEC2:
            glUniform2f((GLuint)uniform_id, (GLfloat)uniform[0], (GLfloat)uniform[1]);
            break;
        case GL_FLOAT_VEC3:
            glUniform3f(uniform_id, uniform[0], uniform[1], uniform[2]);
            break;
        case GL_FLOAT_VEC4:
            glUniform4f(uniform_id, uniform[0], uniform[1], uniform[2], uniform[3]);
            break;
        case GL_SAMPLER_2D:
            glUniform1i(uniform_id, 0);
            break;
        case GL_FLOAT_MAT2:
            glUniformMatrix2fv(uniform_id, 1, GL_FALSE, &uniform[0]);
            break;
        default:
            printf("unknown default uniform \n");
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
        printf("idx %u of %d\n", u, shader_library[shader]->n_uniforms);
        printf("uniform %s \n", name);
        printf("id %d \n", uniform_id);
        printf("type %d \n", type);
        RENDER_set_uniform(uniform_id, type, uniforms[u]);
    }
}

void RENDER_clear_buffer(
    int   buffer
) {
    // clear buffer accordin to clearning rule!
    glBindFramebuffer(GL_FRAMEBUFFER, buffer);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void RENDER_set_viewport(
    int            buffer,
    int            buffer_w,
    int            buffer_h
) { 
    printf("BUFFER: %d | size: %d %d %d %d | ", buffer, 0, 0, buffer_w, buffer_h);
    glBindFramebuffer(GL_FRAMEBUFFER, buffer);
    glViewport(0, 0, buffer_w, buffer_h);    
}

void RENDER_shader(
    int            shader,
    int            texture,
    float         *vertices,
    int            n_vertices,
    float         *uniforms[MAX_SHADER_UNIFORMS],
    int            count,
    int            mode
) { 
    printf("SHADER %d | ", shader_library[shader]->program);
    printf("TEXTURE %d", texture);
    glUseProgram(shader_library[shader]->program);

    glBindVertexArray(shader_library[shader]->vao);
    glBindBuffer(GL_ARRAY_BUFFER, shader_library[shader]->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*n_vertices, vertices, GL_STATIC_DRAW);
    RENDER_set_uniforms(shader, uniforms);
    
    // glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(mode, 0, (int)(n_vertices/count));

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
