#include "gl_util.h"

#ifndef RENDER_H
#define RENDER_H

void RENDER_shader(
    int      shader,
    int      texture,
    GLfloat *vertices,
    int      n_vertices,
    float   *uniforms[MAX_SHADER_UNIFORMS],
    int      count,
    GLenum   mode,
    int      buffer
);

#endif
