#include <GL/gl.h>
#include <SDL2/SDL.h>

#include <stdbool.h>

#include "gl_util.h"

#ifndef GFX_H
#define GFX_H

extern SDL_Window *window;

GLuint GFX_generate_texture_ID();

bool GFX_init_graphics();

bool GFX_init_png();
bool GFX_set_viewport();
bool GFX_init_gl_params();
bool GFX_init_vsync();
bool GFX_init_glew();
bool GFX_init_window();
bool GFX_init_sdl_with_gl();
bool GFX_init_video();
bool GFX_create_gl_context();

void GFX_free();
void GFX_clear_screen();
void GFX_update();

void GFX_set_interpolation_2d(int interpolation);
void GFX_bind_texture(GLuint texture_id);
void GFX_specify_texture(SDL_Surface *surface, int mode);

int GFX_check_shader_compile_status(GLuint shader_id);
int GFX_check_program_link_status(GLuint program);

shader_program_t* GFX_create_gl_program(
    const char* vertex_shader_path,
    const char* fragment_shader_path,
    const char* geometry_shader_path,
    int n_uniforms_shader,
    int n_uniforms_fragment,
    int n_uniforms_geometry,
    const char *shader_uniforms[MAX_SHADER_UNIFORMS],
    const char *fragment_uniforms[MAX_SHADER_UNIFORMS],
    const char *geometry_uniforms[MAX_SHADER_UNIFORMS]
);

#endif
