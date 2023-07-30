#include "./data/library.h"

#include <GL/glew.h>
#include <stdio.h>
#include <stdbool.h>

#include "render.h"

void RENDER_texture(
    int        gl_texture_id,
    float      render_x1,   // place on screen (OpenGL coords)
    float      render_y1,   // place on screen (OpenGL coords)
    float      render_x2,   // place on screen (OpenGL coords)
    float      render_y2,   // place on screen (OpenGL coords)
    float      clip_x1,      // position on clipture (OpenGL coords)
    float      clip_y1,      // position on clipture (OpenGL coords)
    float      clip_x2,      // position on clipture (OpenGL coords)
    float      clip_y2,      // position on clipture (OpenGL coords)
    bool       flip
) {
    glBindTexture(GL_TEXTURE_2D, gl_texture_id);

    if (flip) {
        float temp;
        temp    = clip_x1; 
        clip_x1 = clip_x2;
        clip_x2 = temp;
    }
    // TODO: this to shader as well? 
    glBegin(GL_QUADS);
        glTexCoord2f(clip_x2, clip_y1); glVertex2f(render_x2, render_y1); // up right (1,1)
        glTexCoord2f(clip_x1, clip_y1); glVertex2f(render_x1, render_y1); // up left (-1, 1)
        glTexCoord2f(clip_x1, clip_y2); glVertex2f(render_x1, render_y2); // down left (-1, -1)
        glTexCoord2f(clip_x2, clip_y2); glVertex2f(render_x2, render_y2); // down right (1, -1)
    glEnd();
};

void RENDER_shader(
    int      shader,
    GLfloat *vertices,
    int      n_vertices,
    int      size,
    float   *v_uniform,
    float   *f_uniform,
    float   *g_uniform
) { 
    // if (current_program_id != program_id) {
    // }

    // TODO: this cannot be called once per frame!
    // TODO: to separate GFX function?
    glUseProgram(shader_library[shader]->program);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * n_vertices, vertices, GL_STATIC_DRAW);
    
    // TODO: to some GFX function?
    int i=0;
    for (int u=0; u<shader_library[shader]->vertex_shader.n_uniforms; u++) { 
        int uniform_id = shader_library[shader]->vertex_shader.uniform_ids[u];
        // glUniform4f(uniform_id, v_uniform[i], v_uniform[i+1], v_uniform[i+2], v_uniform[i+3]);
        i+=MAX_SHADER_UNIFORM_ARGS;
    }

    i=0;
    for (int u=0; u<shader_library[shader]->fragment_shader.n_uniforms; u++) { 
        int uniform_id = shader_library[shader]->fragment_shader.uniform_ids[u];
        glUniform4f(uniform_id, f_uniform[i], f_uniform[i+1], f_uniform[i+2], f_uniform[i+3]);
        printf("uniform id :%d ", uniform_id);
        printf("uniforms: %f %f %f %f \n", f_uniform[i], f_uniform[i+1], f_uniform[i+2], f_uniform[i+3]);
        i+=MAX_SHADER_UNIFORM_ARGS;
    }

    i=0;
    for (int u=0; u<shader_library[shader]->geomentry_shader.n_uniforms; u++) { 
        int uniform_id = shader_library[shader]->geomentry_shader.uniform_ids[u];
        // glUniform4f(uniform_id, g_uniform[i], g_uniform[i+1], g_uniform[i+2], g_uniform[i+3]);
        i+=MAX_SHADER_UNIFORM_ARGS;
    }

    // TODO: this cannot be called once per frame!
    // TODO: shader struct should have `position` in it !
    // TODO: To separate GFX function!
    GLint posAttrib = glGetAttribLocation(shader_library[shader]->program, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_POLYGON, 0, (int)(n_vertices/size));
};

