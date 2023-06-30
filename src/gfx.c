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
        printf("Shader compilation error: %s \n", buffer);
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
    GLenum type,
    const char* path
) {
    const char* src = GFX_read_shader_from_file(path); 

    GLuint shader = glCreateShader(type);

    if(shader == 0) {
        printf("COULD NOT LOAD SHADER: %s!\n", path);
    }

    glShaderSource(shader, 1, (const char**)&src, NULL);
    glCompileShader(shader);

    return (int)shader;
}

int GFX_link_shaders_to_program(
    GLuint vertex_id,
    GLuint fragment_id
) {
    GLuint program_id = glCreateProgram();

    glAttachShader(program_id, vertex_id);
    glAttachShader(program_id, fragment_id);
    // glBindFragDataLocation(program_id, 0, "outColor"); // use for framebuffers

    glLinkProgram(program_id);

    if(program_id == 0) {
        printf("COULD NOT CREATE PROGRAM!\n");
    }
    return program_id;
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

GFX_STATUS GFX_init_window() {
    window = SDL_CreateWindow(
        GAME_NAME,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        DEFAULT_SCREEN_WIDTH,
        DEFAULT_SCREEN_HEIGHT,
        SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL 
    );

    if (!window) {
        return GFX_WINDOW_CANNOT_BE_CREATED;
    }

    SDL_ShowCursor(SDL_DISABLE); // disable cursor on window

    return GFX_CORRECT;
};

GFX_STATUS GFX_init_video(
) {
    int init_video = SDL_Init(SDL_INIT_VIDEO);
    if(init_video) {
        return GFX_VIDEO_CANNOT_BE_INITIALIZED;
    };

    SDL_GL_SetAttribute(OPENGL_MAJOR_VERSION, OPENGL_MAJOR_VERSION);
    SDL_GL_SetAttribute(OPENGL_MINOR_VERSION, OPENGL_MINOR_VERSION);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    return GFX_CORRECT;
};

GFX_STATUS GFX_init_renderer(
) {
    gl_context = SDL_GL_CreateContext(window);
    if(!gl_context) { return GFX_OPEN_GL_CONTEXT_CANNOT_BE_INITIALIED; }

    GLenum glew_err = glewInit();
    if(glew_err != GLEW_OK) { return GFX_GLEW_CANNOT_BE_INITIALIED; }
    
    int vsync_status = SDL_GL_SetSwapInterval(1);
    if(vsync_status < 0) { return GFX_UNABLE_TO_SET_VSYNC; }

    // Initialize OpenGL
    // if(!GFX_init_OpenGL_shaders()) {
    //     printf("Unable to initialize OpenGL!\n");
    //     return 0;
    // }

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    GFX_set_global_render_scale();

    return GFX_CORRECT;
}

GFX_STATUS GFX_init_png(
) {
    int status = IMG_Init(IMG_INIT_PNG);
    if(!status) { return GFX_SDL_IMAGE_COULD_NOT_BE_INITIALIZED; }

    return GFX_CORRECT;
};

void GFX_set_viewport(
) {
    GLint m_viewport[4];
    glGetIntegerv(GL_VIEWPORT, m_viewport);
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

void GFX_aa(
    int program_id
) {
    GLfloat vertices[] = {
         0.0f,  0.5f,
         0.5f, -0.5f,
        -0.5f, -0.5f 
    };
}

GFX_STATUS GFX_init_graphics(
) {
    GFX_STATUS status;

    status = GFX_init_video();
    if(status != GFX_CORRECT) { return status; }

    status = GFX_init_window();
    if(status != GFX_CORRECT) { return status; }
    
    status = GFX_init_renderer();
    if(status != GFX_CORRECT) {  return status; }

    status = GFX_init_png();
    if(status != GFX_CORRECT) { return status; }

    GFX_set_viewport();
    GFX_init_vao();
    GFX_init_vbo();

    return GFX_CORRECT;
};

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

