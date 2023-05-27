#include <stdio.h>

#include <GL/glu.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>

#include "global.h"
#include "texture.h"

int texture_id_counter = 0;

// generate unique ID for texture
GLuint TXTR_generate_texture_ID(
) {
    texture_id_counter++;

    return (GLuint) texture_id_counter;
}

// set texture interpolation method in OpenGL
void TXTR_set_interpolation_2d(
    int interpolation
) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpolation);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolation);
}

// bind texture to OpenGL
void TXTR_bind_to_gl(
    GLuint texture_id
) {
    // glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
}

// specify texture in OpenGL
void TXTR_specify_to_gl(
    SDL_Surface *surface,
    int mode
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

// create new texture_t struct
texture_t* TXTR_new(
    SDL_Surface *surface,
    int mode,
    const char *filepath
) {

    GLuint texture_id                       = TXTR_generate_texture_ID();
    TXTR_bind_to_gl(texture_id);
    TXTR_set_interpolation_2d(GL_NEAREST);
    TXTR_specify_to_gl(surface, mode);

    texture_t* tex = malloc(sizeof(texture_t));
    tex->surface   = surface;
    tex->id        = texture_id;
    // tex->filepath  = filepath;

    return tex;
}

// get texture overall width
int TXTR_width(
    texture_t* texture
) {
    if (!texture || !(texture->surface)) {
        return -1;
    } else {
        return texture->surface->w;
    }
}

// get texture overall height
int TXTR_height(
    texture_t* texture
) {
    if (!texture || !(texture->surface)) {
        return -1;
    } else {
        return texture->surface->h;
    }
}

// create texture_t from file image
texture_t* TXTR_read_from_file(
    const char *filepath
) {
    SDL_Surface *surface        = NULL;
    surface                     = IMG_Load(filepath);
     
    return TXTR_new(surface, GL_RGB, filepath);
}

void TXTR_free(
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

