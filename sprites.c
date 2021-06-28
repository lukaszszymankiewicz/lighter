#include "sprites.h"
#include "gfx.h"

Texture* hero_sprites;
Texture* light_gradient;
SDL_Rect hero_texture_clips[ 4 ];

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
    hero_sprites = TXTR_init_texture("sprites/hero.png");

    hero_texture_clips[ 0 ].x =  0;
    hero_texture_clips[ 0 ].y =  0;
    hero_texture_clips[ 0 ].w = 30;
    hero_texture_clips[ 0 ].h = 45;

    //Set top right sprite
    hero_texture_clips[ 1 ].x = 31;
    hero_texture_clips[ 1 ].y =  0;
    hero_texture_clips[ 1 ].w = 30;
    hero_texture_clips[ 1 ].h = 45;
    
    //Set bottom left sprite
    hero_texture_clips[ 2 ].x = 62;
    hero_texture_clips[ 3 ].y =  0;
    hero_texture_clips[ 2 ].w = 30;
    hero_texture_clips[ 2 ].h = 45;

    //Set bottom right sprite
    hero_texture_clips[ 3 ].x = 93;
    hero_texture_clips[ 3 ].y =  0;
    hero_texture_clips[ 3 ].w = 30;
    hero_texture_clips[ 3 ].h = 45;

    // light gradient
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

void TXTR_draw_hero(int x, int y, int hero_state) 
{
    TXTR_render_texture(hero_sprites, &hero_texture_clips[hero_state], x, y);
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
    TXTR_free(hero_sprites);
    TXTR_free(light_gradient);
};
