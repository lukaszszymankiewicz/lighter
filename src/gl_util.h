#include "mat.h"

#ifndef GL_UTIL_H
#define GL_UTIL_H

#define MAX_SHADER_UNIFORMS 3
#define VERTICES_PER_RECT   6

extern int camera_x;
extern int camera_y;
extern float framebuffer_w;
extern float framebuffer_h;

// TODO: to GFX?
typedef struct shader_program {
    int           program;
    int           vertex_shader_id;
    int           fragment_shader_id;
    int           geometry_shader_id;
    int           n_uniforms;
    int           uniform_loc[MAX_SHADER_UNIFORMS];
    int           uniform_types[MAX_SHADER_UNIFORMS];
    char         *uniform_names[MAX_SHADER_UNIFORMS];
    unsigned int  vao;
    unsigned int  vbo;
} shader_program_t;

// TODO: to GFX?
typedef struct framebuffer {
    unsigned int id;
    unsigned int texture;
    unsigned int x0;
    unsigned int y0;
    unsigned int w;
    unsigned int h;
} framebuffer_t;

extern float pixel_perfect_scale;

array_t GL_UTIL_camera();
array_t GL_UTIL_id(int id);
array_t GL_UTIL_scale();

void GL_UTIL_set_pixelperfect_scale();

#endif
