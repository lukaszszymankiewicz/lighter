#include <GL/glew.h>

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
    
    glBegin(GL_QUADS);
        glTexCoord2f(clip_x2, clip_y1); glVertex2f(render_x2, render_y1); // up right (1,1)
        glTexCoord2f(clip_x1, clip_y1); glVertex2f(render_x1, render_y1); // up left (-1, 1)
        glTexCoord2f(clip_x1, clip_y2); glVertex2f(render_x1, render_y2); // down left (-1, -1)
        glTexCoord2f(clip_x2, clip_y2); glVertex2f(render_x2, render_y2); // down right (1, -1)
    glEnd();
};

void RENDER_shader(
    GLfloat *vertices,
    int program_id,
    int n_vertices,
    int size
) { 

    // TODO: this propably needs to be refactored, as glGetIntegerv is slow (and glUseProgram!)!
    GLint current_program_id;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current_program_id);
    
    if (current_program_id != program_id) {
        glUseProgram(program_id);
    }

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * n_vertices, vertices, GL_STATIC_DRAW);
    GLint posAttrib = glGetAttribLocation(program_id, "position");
    glEnableVertexAttribArray(posAttrib);
    // glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(posAttrib, size, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, n_vertices);
};

