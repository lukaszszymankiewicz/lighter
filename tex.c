#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "tex.h"

Texture *init_texture(char *filepath, SDL_Renderer* renderer) {
	SDL_Texture* new_texture = NULL;
    SDL_Surface* loaded_surface = NULL;

    loaded_surface = IMG_Load(filepath);

    // this will set bg transparency color
    SDL_SetColorKey(loaded_surface, SDL_TRUE, SDL_MapRGB(loaded_surface->format, 0, 0xFF, 0xFF));

    new_texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);

    Texture *p = malloc(sizeof(Texture));

    p->surface = new_texture;
    p->width = loaded_surface->w;
    p->height = loaded_surface->h;

    SDL_FreeSurface(loaded_surface);

    return p;
};

void free_texture(Texture *texture) {
    SDL_DestroyTexture(texture->surface);
    free(texture);
};

void render_texture(SDL_Renderer* renderer, Texture* texture, int scale, int x, int y) {
	SDL_Rect renderQuad = {x, y, (texture->width) * scale, (texture->height) * scale};
	SDL_RenderCopy(renderer, texture->surface, NULL, &renderQuad);
}
