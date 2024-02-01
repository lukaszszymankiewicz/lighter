#include <GL/gl.h>
#include <SDL2/SDL.h>

#include <stdbool.h>

#ifndef GFX_H
#define GFX_H

#define MAX_SHADER_UNIFORMS 8
#define DEFAULT_TEXTURE_UNIT 0

extern int   camera_x;
extern int   camera_y;

extern float FRAMEBUFFER_WIDTH;
extern float FRAMEBUFFER_HEIGHT;

extern SDL_Window *window;

typedef struct uniform {
    int   loc;
    int   type;
    int   size;
    char *name;
} uniform_t;

typedef struct shader_program {
    int           program;
    int           vertex_shader_id;
    int           fragment_shader_id;
    int           geometry_shader_id;
    int           n_uniforms;
    uniform_t    *uniforms[MAX_SHADER_UNIFORMS];
    unsigned int  vao;
    unsigned int  vbo;
} shader_program_t;

typedef struct framebuffer {
    unsigned int id;
    unsigned int texture;
    unsigned int x0;
    unsigned int y0;
    unsigned int w;
    unsigned int h;
} framebuffer_t;

typedef struct texture {
    SDL_Surface *surface;
    int          id;
} texture_t;

bool GFX_init_png();
bool GFX_init_gl_params();
bool GFX_init_glew();
bool GFX_init_window();
bool GFX_init_sdl_with_gl();
bool GFX_create_gl_context();
bool GFX_set_viewport();

void GFX_free();
void GFX_update();

void GFX_destroy_framebuffer(GLuint id);
void GFX_free_texture(texture_t* texture);

int GFX_type_size(GLenum type);

texture_t*     GFX_read_texture(const char *filepath);
framebuffer_t* GFX_create_framebuffer(int id, int w, int h);

shader_program_t* GFX_create_gl_program(
    const char* vertex_shader_path,
    const char* fragment_shader_path,
    const char* geometry_shader_path
);

#endif
