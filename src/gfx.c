#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <SDL2/SDL_image.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "gfx.h"
#include "global.h"

#define OPENGL_MAJOR_VERSION 3
#define OPENGL_MINOR_VERSION 3

#define SHADER_CHECK_MODE    "rb"
#define SHADER_READ_MODE     "r"

#define ALLOWED_UNIFORM_NAME_LEN 16
#define ALLOWED_ATTRIB_NAME_LEN  16

#define UNUSED_SHADER_PROGRAM_ID -1

SDL_GLContext gl_context        = NULL;
SDL_Window   *window            = NULL;

int max_buffer_len              = 512;
int TEXTURE_MODE                = GL_RGB;

int   camera_x              = 0;
int   camera_y              = 0;

float FRAMEBUFFER_WIDTH     = 1.0;
float FRAMEBUFFER_HEIGHT    = 1.0;

// shamesly copied from Stack Overflow
void GLAPIENTRY MessageCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam 
) {
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n", ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ), type, severity, message);
}

// a lot of defaults, but I dont care
texture_t* GFX_read_texture(
    const char *filepath
) {
    SDL_Surface  *surface = NULL;
    texture_t    *texture = NULL;
    GLuint        id;

    surface                = IMG_Load(filepath);
    texture                = (texture_t*)malloc(sizeof(texture_t));

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if(surface && surface->format->BytesPerPixel == 4) {
        printf("mode read \n");
        TEXTURE_MODE = GL_RGBA; 
    }

    // surface can be NULL, check must be done beforehand
    if (surface) {
        printf("surface read \n");
        glTexImage2D(
            GL_TEXTURE_2D, 0, TEXTURE_MODE,
            surface->w, surface->h,
            0, TEXTURE_MODE, GL_UNSIGNED_BYTE, surface->pixels
        );
    }

    texture->surface   = surface;
    texture->id        = id;

    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

void GFX_free_texture(
    texture_t* texture
) {
    if (texture && texture->surface) {
        SDL_FreeSurface(texture->surface);
        texture->surface    = NULL;
        GLuint TextureID    = texture->id;
        glDeleteTextures(1, &TextureID);
        free(texture);
        texture = NULL;
    }
}

int GFX_check_shader_compile_status(
    GLuint shader_id
) {
    GLint status;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &status);
    
    if (status == GL_FALSE) {
        char buffer[max_buffer_len];
        glGetShaderInfoLog(shader_id, max_buffer_len, NULL, buffer);
        printf("Shader compilation error: %s", buffer);
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
    const char *path,
    GLenum      type
) {
    const char* src   = GFX_read_shader_from_file(path); 
    GLuint id         = glCreateShader(type);
    
    glShaderSource(id, 1, (const GLchar**)&src, NULL);
    glCompileShader(id);

    if (!GFX_check_shader_compile_status(id)) {
        printf("Shader %s failed to compiled!\n", path);
        return -1; 
    }
    return (int)id;
}

int GFX_type_size(
    GLenum type
) {
    switch (type)
    {
        case GL_FLOAT_VEC2:
            return 2 * sizeof(GL_FLOAT);
        case GL_FLOAT_VEC3:
            return 3 * sizeof(GL_FLOAT);
        case GL_FLOAT_VEC4:
            return 4 * sizeof(GL_FLOAT);
        case GL_SAMPLER_2D:
            return 1 * sizeof(GLuint);
        case GL_FLOAT_MAT2:
            return 4 * sizeof(GL_FLOAT);
        case GL_FLOAT:
            return 1 * sizeof(GL_FLOAT);
        default:
            return -1;
    }
}

shader_program_t* GFX_create_gl_program(
    const char* vertex_shader_path,
    const char* fragment_shader_path,
    const char* geometry_shader_path
) {
    GLint i;
    GLint n_uniforms, n_attribs;

    GLenum  type; 
    GLchar  name[ALLOWED_UNIFORM_NAME_LEN];
    GLsizei length;

    shader_program_t* shader_program = NULL;
    shader_program                   = (shader_program_t*)malloc(sizeof(shader_program_t));

    int vertex_shader_id = GFX_compile_shader(vertex_shader_path, GL_VERTEX_SHADER);
    if (vertex_shader_id == -1) { return NULL; }

    int fragment_shader_id = GFX_compile_shader(fragment_shader_path, GL_FRAGMENT_SHADER);
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
    shader_program->geometry_shader_id = UNUSED_SHADER_PROGRAM_ID;
    shader_program->program            = program_id;

    // get uniforms
    glGetProgramiv(program_id, GL_ACTIVE_UNIFORMS, &n_uniforms);
    shader_program->n_uniforms = n_uniforms;

    for (i=0; i<n_uniforms; i++) {
        shader_program->uniforms[i] = NULL;
        shader_program->uniforms[i] = (uniform_t*)malloc(sizeof(uniform_t));

        int loc;
        glGetActiveUniform(program_id, (GLuint)i, ALLOWED_UNIFORM_NAME_LEN, &length, NULL, &type, name);
        shader_program->uniforms[i]->name = NULL;
        shader_program->uniforms[i]->name = (char*)malloc(sizeof(char) * strlen(name) + 1);
        strcpy(shader_program->uniforms[i]->name, name);

        loc                                = glGetUniformLocation(program_id, name);
        shader_program->uniforms[i]->loc   = loc;
        shader_program->uniforms[i]->type  = type;
        shader_program->uniforms[i]->size  = GFX_type_size(type);
    }
    
    // create VBO
    glGenBuffers(1, &shader_program->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, shader_program->vbo);

    // create VAO 
    glGenVertexArrays(1, &shader_program->vao);
    glBindVertexArray(shader_program->vao);

    // get attribs
    n_attribs = (GLuint)0;
    glGetProgramiv(program_id, GL_ACTIVE_ATTRIBUTES, &n_attribs);
    
    int    locs[n_attribs]; 
    int    vec_sizes[n_attribs]; 
    int    stride = 0;

    for (i=0; i<n_attribs; i++) {
        GLenum type; int loc; 

        glGetActiveAttrib(program_id, (GLuint)i, ALLOWED_ATTRIB_NAME_LEN, NULL, NULL, &type, name);
        loc      = (int)glGetAttribLocation(program_id, name);
        locs[i]  = loc;

        vec_sizes[i]  = GFX_type_size(type) / sizeof(GL_FLOAT);
        stride       += vec_sizes[i];
    }

    int    atrb_size = 0;

    for (i=0; i<n_attribs; i++) {
        glEnableVertexAttribArray(locs[i]);
        glVertexAttribPointer(
            locs[i],
            vec_sizes[i],
            GL_FLOAT,
            GL_FALSE,
            stride*sizeof(float),
            (void*)(atrb_size*sizeof(float))
        );

        atrb_size += vec_sizes[i];
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return shader_program;
}

bool GFX_set_viewport(
) {
    GLint m_viewport[4];
    glGetIntegerv(GL_VIEWPORT, m_viewport);
    
    FRAMEBUFFER_HEIGHT = (float)m_viewport[3];
    FRAMEBUFFER_WIDTH  = (float)m_viewport[2];

    return true;
}

bool GFX_init_window(
) {
    window = SDL_CreateWindow(
        GAME_NAME,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL
    );

    if (!window) {
        printf("Error: %s \n", SDL_GetError());
        return false;
    }

    // disable cursor on window
    SDL_ShowCursor(SDL_DISABLE);

    return true;
};

bool GFX_init_sdl_with_gl(
) {
    SDL_GL_SetAttribute(OPENGL_MAJOR_VERSION, OPENGL_MAJOR_VERSION);
    SDL_GL_SetAttribute(OPENGL_MINOR_VERSION, OPENGL_MINOR_VERSION);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 1);

    return true;
}

bool GFX_create_gl_context(
) {
    gl_context = SDL_GL_CreateContext(window);
    return (gl_context != NULL);
}

bool GFX_init_glew(
) {
    return (glewInit() == GLEW_OK);
}

bool GFX_init_gl_params(
) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    return true;
}

bool GFX_init_png(
) {
    return (bool)IMG_Init(IMG_INIT_PNG);
};

void GFX_update() {
    SDL_GL_SwapWindow(window);
    SDL_UpdateWindowSurface(window);
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

framebuffer_t* GFX_create_framebuffer(
    int   buffer_id,
    int   w,
    int   h
) {
    framebuffer_t* framebuffer = NULL;
    framebuffer                = (framebuffer_t*)malloc(sizeof(framebuffer_t));

    GLuint id;
    glGenFramebuffers(1, &id);
    glBindFramebuffer(GL_FRAMEBUFFER, id);

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        0, GL_RGBA, GL_UNSIGNED_BYTE, NULL
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    glFramebufferTexture2D(
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0
    );

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("Framebuffer cannot be created! \n");
        return NULL;
    }
    
    framebuffer->id         = id;
    framebuffer->texture    = tex;
    framebuffer->x0         = 0;
    framebuffer->y0         = 0;
    framebuffer->w          = w;
    framebuffer->h          = h;

    glEnablei(GL_BLEND, id);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return framebuffer;
}

void GFX_destroy_framebuffer(
    GLuint id
) {
    glDeleteFramebuffers(1, &id);
}

void LIB_free_shader(
    shader_program_t *shader
) {
    glDeleteProgram(shader->program);
}
