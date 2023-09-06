#ifndef GL_UTIL_H
#define GL_UTIL_H

#define MAX_VERTEX_LEN                50
#define MAX_SHADER_ATTRIBS            3
#define MAX_SHADER_UNIFORMS_ARGS      4
#define MAX_SHADER_UNIFORMS           2
#define MAX_SHADER_UNIFORMS_ARGS_LEN  MAX_SHADER_UNIFORMS_ARGS * MAX_SHADER_UNIFORMS           

extern float global_x_scale;
extern float global_y_scale;

typedef struct render_coord {
    float        x1;       // glVertex2f for x1 (or glTexCoord2f)
    float        y1;       // glVertex2f for y1 (or glTexCoord2f)
    float        x2;       // glVertex2f for x2 (or glTexCoord2f)
    float        y2;       // glVertex2f for y2 (or glTexCoord2f)
} render_coord_t;

typedef struct render_vertex {
    float         coefs[MAX_VERTEX_LEN];
    int           len;
} render_vertex_t;

typedef struct shader_program {
    int    program;
    int    vertex_shader_id;
    int    fragment_shader_id;
    int    geometry_shader_id;
    int    n_uniforms;
    int    uniform_loc[MAX_SHADER_UNIFORMS];
    int    n_attribs;
    int    attrib_size; // calosc
    int    attrib_loc[MAX_SHADER_ATTRIBS];
    int    attrib_stride[MAX_SHADER_ATTRIBS];
    int    vao;
    int    vbo;
} shader_program_t;

render_coord_t GL_UTIL_global_to_gl_coord(
    int x, int y, int w, int h,
    float clip_x1, float clip_y1,
    float clip_x2, float clip_y2
);

render_coord_t GL_UTIL_gl_to_camera_gl(
    render_coord_t render,
    int camera_x, int camera_y
);

render_coord_t GL_UTIL_global_to_gl_coord_single(
    int        x, int        y,
    int camera_x, int camera_y
);

#endif
