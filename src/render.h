#include <stdbool.h>

#ifndef RENDER_H
#define RENDER_H

void RENDER_shader(
    int      shader,
    int      texture,
    GLfloat *vertices,
    int      n_vertices,
    float   *uniforms,
    int      count,
    GLenum   mode
);

#endif
