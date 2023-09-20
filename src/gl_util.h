#ifndef GL_UTIL_H
#define GL_UTIL_H

#define MAX_VERTEX_LEN                50

// TODO: TBD
#define MAX_SHADER_UNIFORMS_ARGS      4
#define MAX_SHADER_UNIFORMS           3

// TODO: TBD?
#define MAX_SHADER_UNIFORMS_ARGS_LEN  MAX_SHADER_UNIFORMS_ARGS * MAX_SHADER_UNIFORMS           

extern float scale_x;
extern float scale_y;

extern float camera_x;
extern float camera_y;

typedef struct render_coord {
    float x1; float y1;
    float x2; float y2;
} render_coord_t;

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

float GL_UTIL_x(int x);
float GL_UTIL_y(int y);

float* GL_UTIL_camera();
float* GL_UTIL_scale();
float* GL_UTIL_id(int id);

render_coord_t GL_UTIL_rect(int x, int y, int w, int h);
render_coord_t GL_UTIL_clip(int x1, int y1, int x2, int y2, int w, int h);

#endif
