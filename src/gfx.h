#include <SDL2/SDL.h>
#include <GL/gl.h>

#ifndef GFX_H
#define GFX_H

extern SDL_Window *window;

GLuint GFX_generate_texture_ID();

int GFX_init_graphics();

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

// TODO: to be deleted
typedef int GFX_STATUS;
enum GFX_STATUS_TYPE {
    GFX_CORRECT = 1,
    GFX_WINDOW_CANNOT_BE_CREATED,
    GFX_VIDEO_CANNOT_BE_INITIALIZED,
    GFX_OPEN_GL_CONTEXT_CANNOT_BE_INITIALIED,
    GFX_GLEW_CANNOT_BE_INITIALIED,
    GFX_UNABLE_TO_SET_VSYNC,
    GFX_SDL_IMAGE_COULD_NOT_BE_INITIALIZED
};

#endif
