#include <GL/gl.h>
#include <SDL2/SDL.h>

#include <stdbool.h>

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

int GFX_compile_shader(GLenum type, const char* path);
int GFX_link_shaders_to_program(GLuint vertex_id, GLuint fragment_id);

#endif
