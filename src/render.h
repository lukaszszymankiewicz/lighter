#include "gfx.h"
#include "mat.h"

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
    array_t *uniforms[MAX_SHADER_UNIFORMS],
    int      count,
    int      mode
);

int RENDER_uniform_n(int shader);
int RENDER_get_uniform_index(int shader, const char* uniform);
int RENDER_get_uniform_size(int shader, int uniform);
void RENDER_clear_buffer(int buffer);

#endif
