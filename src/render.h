#include <stdbool.h>

#ifndef RENDER_H
#define RENDER_H

void RENDER_shader(
    int      shader,
    int      texture,
    GLfloat *vertices,
    int      n_vertices,
    float   *uniforms
);

void RENDER_shader_texture(
    int      shader,
    int      texture,
    GLfloat *vertices,
    int      n_vertices,
    float   *uniforms
);

#endif
