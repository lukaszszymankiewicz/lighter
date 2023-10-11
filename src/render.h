#ifndef RENDER_H
#define RENDER_H

#define MAX_SHADER_UNIFORMS 3

void RENDER_shader(
    int      shader,
    int      texture,
    float   *vertices,
    int      n_vertices,
    float   *uniforms[MAX_SHADER_UNIFORMS],
    int      count,
    int      mode,
    int      buffer,
    int      buffer_w,
    int      buffer_h
);

void RENDER_clear_buffer(int buffer);

#endif
