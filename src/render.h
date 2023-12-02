#include "gfx.h"

#ifndef RENDER_H
#define RENDER_H

void RENDER_set_viewport(
    int            buffer,
    int            buffer_w,
    int            buffer_h
);

void RENDER_shader(
    int      shader,
    int      texture,
    float   *vertices,
    int      n_vertices,
    float   *uniforms[MAX_SHADER_UNIFORMS],
    int      count,
    int      mode
);

void RENDER_clear_buffer(int buffer);
void RENDER_set_attachment(int attachment);

#endif
