#include <stdbool.h>

#ifndef RENDER_H
#define RENDER_H

void RENDER_texture(
    int        gl_texture_id,
    float      render_x1, float      render_y1,
    float      render_x2, float      render_y2,
    float      clip_x1,   float      clip_y1,
    float      clip_x2,   float      clip_y2,
    bool       flip
);

void RENDER_shader(
    int      program_id,
    GLfloat *vertices,
    int      n_vertices,
    int      size,
    float   *v_uniform,
    float   *f_uniform,
    float   *g_uniform
);

#endif
