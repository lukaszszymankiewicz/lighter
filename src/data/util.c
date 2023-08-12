#include <GL/glu.h>
#include <SDL2/SDL_image.h>

#include "../gfx.h"
#include "../texture.h"

const int TEXTURE_MODE = GL_RGB;

// TODO: to GFX
texture_t* TXTR_read_from_file(
    const char *filepath
) {
    SDL_Surface *surface        = NULL;
    surface                     = IMG_Load(filepath);

    GLuint texture_id                       = GFX_generate_texture_ID();
    GFX_bind_texture(texture_id);
    GFX_set_interpolation_2d(GL_NEAREST);
    GFX_specify_texture(surface, TEXTURE_MODE);

    texture_t* tex = malloc(sizeof(texture_t));
    tex->surface   = surface;
    tex->id        = texture_id;
    // tex->filepath  = filepath;
    printf("texture %s read and has id %d\n", filepath, texture_id);
    return tex;
}

// TODO: to GFX
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
