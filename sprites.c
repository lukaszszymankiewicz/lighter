#include "sprites.h"
#include "gfx.h"

Texture* light_gradient;

Texture * TXTR_init_texture(char *filepath) 
{
	SDL_Texture* new_texture = NULL;
    SDL_Surface* loaded_surface = NULL;

    loaded_surface = IMG_Load(filepath);

    SDL_SetColorKey(loaded_surface, SDL_TRUE, SDL_MapRGB(loaded_surface->format, 0x80, 0xFF, 0xFF));

    new_texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);

    Texture *p = malloc(sizeof(Texture));

    p->surface = new_texture;
    p->width = loaded_surface->w;
    p->height = loaded_surface->h;

    SDL_FreeSurface(loaded_surface);

    return p;
};

void TXTR_load_frames() 
{
    light_gradient = TXTR_init_texture("sprites/gradient.png");
};

void TXTR_render_texture(Texture* texture, SDL_Rect* clip, int x, int y) 
{
    SDL_Rect render_quad = {x, y, texture->width, texture->height};

    if(clip != NULL) 
    {
        render_quad.w = clip->w;
        render_quad.h = clip->h;
    }

    SDL_RenderCopy(renderer, texture->surface, clip, &render_quad);
};

void TXTR_draw_light(int x, int y) 
{
    TXTR_render_texture(light_gradient, NULL, x-256, y-256);
};

void TXTR_free(Texture *texture) 
{
    SDL_DestroyTexture(texture->surface);
    free(texture);
};

void TXTR_free_all() 
{
    TXTR_free(light_gradient);
};
