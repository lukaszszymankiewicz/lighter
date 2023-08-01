// TODO: check if relevent
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

// TODO: check if releveant
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_image.h>

#include "gfx.h"
#include "global.h"
#include "gl_util.h"

#define OPENGL_MAJOR_VERSION 3
#define OPENGL_MINOR_VERSION 3

#define SHADER_CHECK_MODE    "rb"
#define SHADER_READ_MODE     "r"

// TODO: make array from allowed types
#define ALLOWED_UNIFORM_TYPE_N   1
#define ALLOWED_UNIFORM_TYPE     (GLuint[ALLOWED_UNIFORM_TYPE_N]){ GL_FLOAT_VEC4 }
#define ALLOWED_UNIFORM_NAME_LEN 16

GLint  gVertexPos2DLocation = -1;
GLuint gVBO                 = 0;
GLuint gIBO                 = 0;

// TODO: check if relevent
bool gRenderQuad = true;

SDL_GLContext gl_context        = NULL;
SDL_Window   *window            = NULL;

int texture_id_counter = 0;
int max_buffer_len     = 512;

GLuint GFX_generate_texture_ID(
) {
    texture_id_counter++;
    return (GLuint) texture_id_counter;
}

int GFX_check_shader_compile_status(
    GLuint shader_id
) {
    GLint status;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &status);
    
    if (status == GL_FALSE) {
        char buffer[max_buffer_len];
        glGetShaderInfoLog(shader_id, max_buffer_len, NULL, buffer);
        printf("Shader compilation error: %s \n", buffer);
        return 0;
    }
    return 1;
}

int GFX_check_program_link_status(
    GLuint program 
) {
    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);

    if (linked == GL_FALSE) {
        char buffer[max_buffer_len];
        glGetProgramInfoLog(program, max_buffer_len, NULL, buffer);
        printf("Program link error: %s \n", buffer);
        return 0;
    }
    return 1;
}

char* GFX_read_shader_from_file(
   const char* path
) {
    FILE *fp;
    long  size = 0;
    char* shader;
    
    fp = fopen(path, SHADER_CHECK_MODE);
    if(fp == NULL) {
        printf("COULD NOT LOAD SHADER: %s!\n", path);
        return "";
    }

    fseek(fp, 0L, SEEK_END);
    size = ftell(fp) + 1;
    fclose(fp);

    fp = fopen(path, SHADER_READ_MODE);
    shader = memset(malloc(size), '\0', size);
    fread(shader, 1, size-1, fp);
    fclose(fp);

    return shader;
}

int GFX_compile_shader(
    const char* path
) {
    const char* src   = GFX_read_shader_from_file(path); 
    GLuint id         = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(id, 1, (const GLchar**)&src, NULL);
    glCompileShader(id);

    if (!GFX_check_shader_compile_status(id)) {
        return -1; 
    }
    return (int)id;
}

shader_program_t* GFX_create_gl_program(
    const char* vertex_shader_path,
    const char* fragment_shader_path,
    const char* geometry_shader_path
) {
    shader_program_t* shader_program = NULL;
    shader_program                   = (shader_program_t*)malloc(sizeof(shader_program_t));

    int vertex_shader_id = GFX_compile_shader(vertex_shader_path);
    if (vertex_shader_id == -1) { return NULL; }

    int fragment_shader_id = GFX_compile_shader(fragment_shader_path);
    if (fragment_shader_id == -1) { return NULL; }
    
    // geometry TBD
    // glAttachShader(program_id, geometry_shader_id);

    // link 
    GLuint program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);
    glLinkProgram(program_id);
    if (GFX_check_program_link_status(program_id) == 0) { return NULL; }
    
    // put everything in place
    shader_program->vertex_shader_id   = vertex_shader_id;
    shader_program->fragment_shader_id = fragment_shader_id;                   
    shader_program->geometry_shader_id = -1; // TDB
    shader_program->program            = program_id;

    // bind uniform arguments
    GLint i; GLint n_uniforms; GLint size; 
    GLenum type; 
    GLchar name[ALLOWED_UNIFORM_NAME_LEN];
    GLsizei length;

    // get uniforms
    glGetProgramiv(program_id, GL_ACTIVE_UNIFORMS, &n_uniforms);
    printf("Active Uniforms: %d\n", n_uniforms);
    
    shader_program->n_uniforms = n_uniforms;

    for (i=0; i<n_uniforms; i++) {
        glGetActiveUniform(program_id, (GLuint)i, ALLOWED_UNIFORM_NAME_LEN, &length, &size, &type, name);
        printf("Uniform #%d Type: %u Name: %s\n", i, type, name);

        for (int j=0; j<ALLOWED_UNIFORM_TYPE_N; j++) {
            if (type != ALLOWED_UNIFORM_TYPE[j]) {
                printf("invalid uniform type! \n");
                return NULL;
            }
            if (length > ALLOWED_UNIFORM_TYPE_N) {
                printf("uniform name is too long! \n");
                return NULL;
            }

            shader_program->uniform_ids[i]   = glGetUniformLocation(program_id, name);
            strcpy(shader_program->uniform_names[i], name);
        }
    }

    return shader_program;
}

void GFX_set_interpolation_2d(
    int interpolation
) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpolation);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolation);
}

void GFX_bind_texture(
    GLuint texture_id
) {
    // glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
}

// specify texture in OpenGL
void GFX_specify_texture(
    SDL_Surface *surface,
    int          mode
) {
    // surface can be NULL, check must be done beforehand
    if(surface && surface->format->BytesPerPixel == 4) {
        mode = GL_RGBA; 
    }
    // RGBA8888 
    // color key 0x80 0xFF 0xFF
    // There’s not much to do to use the alpha channel. Just make a PNG with transparency, load the
    // texture as GL_RGBA (I think glpng will do this for you automatically).
    // Then set your blend mode to GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA and enable blending

    // surface can be NULL, check must be done beforehand
    if (surface) {
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            mode,
            surface->w,
            surface->h,
            0,
            mode,
            GL_UNSIGNED_BYTE,
            surface->pixels
        );
    }
}

// sets global rendering scale which must be a positive integer. Scale tries to fit best tile_per_x 
// and tile_per_y. This is needed to achieve pixel-perfect rendering (which can be done only if each
// pixel in every drawing routine is multiplied by some scale).
void GFX_set_global_render_scale(
) {
    float tile_per_x = (float)MAX_SCREEN_TILE_PER_X;
    float tile_per_y = (float)MAX_SCREEN_TILE_PER_Y;        ;

    GLint m_viewport[4];
    glGetIntegerv(GL_VIEWPORT, m_viewport);
    
    float max_screen_w = (float)m_viewport[2];
    float max_screen_h = (float)m_viewport[3];

    int closestSclaleX = (int)((float)max_screen_w / ((float)TILE_WIDTH * tile_per_x));
    int closestSclaleY = (int)((float)max_screen_h / ((float)TILE_HEIGHT * tile_per_y));
    
    // best scaling is chosen (fit most of tile_per_x or tile_per_y)
    int scale = MAX(closestSclaleX, closestSclaleY);
    
    // global scale is calculated and set
    global_x_scale = (float)TILE_WIDTH / (max_screen_w / 2.0 / tile_per_x) * scale;
    global_y_scale = (float)TILE_HEIGHT / (max_screen_h / 2.0 / tile_per_y) * scale;
}

bool GFX_init_window() {
    window = SDL_CreateWindow(
        GAME_NAME,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        DEFAULT_SCREEN_WIDTH,
        DEFAULT_SCREEN_HEIGHT,
        SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL 
    );

    if (!window) {
        printf("Error: %s \n", SDL_GetError());
        return false;
    }

    SDL_ShowCursor(SDL_DISABLE); // disable cursor on window

    return true;
};

bool GFX_init_sdl_with_gl(
) {
    SDL_GL_SetAttribute(OPENGL_MAJOR_VERSION, OPENGL_MAJOR_VERSION);
    SDL_GL_SetAttribute(OPENGL_MINOR_VERSION, OPENGL_MINOR_VERSION);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    return true;
}

bool GFX_init_video(
) {
    return (SDL_Init(SDL_INIT_VIDEO) == 0);
};

bool GFX_create_gl_context() {
    gl_context = SDL_GL_CreateContext(window);
    return (gl_context != NULL);
}

bool GFX_init_glew() {
    return (glewInit() == GLEW_OK);
}

bool GFX_init_vsync(){
    return (SDL_GL_SetSwapInterval(1) == 0);
}

bool GFX_init_gl_params(
) {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // TODO: log error with
    // error = glGetError();

    GFX_set_global_render_scale();

    return true;
}

bool GFX_init_png(
) {
    return (bool)IMG_Init(IMG_INIT_PNG);
};

bool GFX_set_viewport(
) {
    GLint m_viewport[4];
    glGetIntegerv(GL_VIEWPORT, m_viewport);
    return true;
}

GLuint GFX_init_vao(
) {
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    return vao;
}

GLuint GFX_init_vbo(
) {
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    return vbo;
}

void GFX_clear_screen() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
};

void GFX_update() {
    SDL_GL_SwapWindow(window);
    SDL_UpdateWindowSurface(window);
};

bool GFX_init_graphics(
) {
    // TODO: OK, this propably needs to be placed somewhere else
    // GFX_init_vao();
    GFX_init_vbo();

    return true;
};

// TODO: to modules teardown
void GFX_free(
) {
    if (gl_context) {
        // TODO: some cleaning here?
        // glDeleteProgram(opengl_program_id);
        glUseProgram(0);
    }

    if (window) {
        SDL_DestroyWindow(window);
    }

    IMG_Quit();
};

