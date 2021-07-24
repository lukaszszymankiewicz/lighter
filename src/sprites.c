#include <stdbool.h>
#include "sprites.h"
#include "gfx.h"

animation_sheet_t* TXTR_init_animation_sheet
(
    char *filepath,
    int n_animations
)
{
    animation_sheet_t* new_sheet = malloc(sizeof(animation_sheet_t));

    new_sheet->texture = TXTR_init_texture(filepath);
    new_sheet->n_animations = n_animations;
    animation_t* animations = malloc(sizeof(animation_t) * n_animations);
    new_sheet->animations=animations;

    return new_sheet;
}

animation_t* TXTR_init_animation
(
    int xs[], int ys[],
    int ws[], int hs[],
    int delay, int len
)
{
    animation_t* new_animation = malloc(sizeof(animation_t));
    SDL_Rect* frames = malloc(sizeof(SDL_Rect) * len);

    for (int i=0; i<len; i++)
    {
        frames[i] = (SDL_Rect){xs[i], ys[i], ws[i], hs[i]};
    }
    new_animation->frames = frames;
    new_animation->delay = delay;
    new_animation->len = len;
        
    return new_animation;
}

void TXTR_push_animation
(
    animation_sheet_t* sheet,
    int index,
    int xs[], int ys[],
    int ws[], int hs[],
    int delay, int len
)
{
    sheet->animations[index] = *(TXTR_init_animation(xs, ys, ws, hs, delay, len));
}

Texture* TXTR_init_texture(char *filepath) 
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

void TXTR_render_texture(Texture* texture, SDL_Rect* clip, int x, int y, bool flip) 
{
    SDL_Rect render_quad = {x, y, texture->width, texture->height};
    SDL_RendererFlip flip_tex;

    if(clip != NULL) 
    {
        render_quad.w = clip->w;
        render_quad.h = clip->h;
    }
    
    if (flip) { flip_tex = SDL_FLIP_HORIZONTAL; }
    else { flip_tex = SDL_FLIP_NONE; }

    SDL_RenderCopyEx(renderer, texture->surface, clip, &render_quad, 0, NULL, flip_tex);
};

void TXTR_free(Texture *texture) 
{
    SDL_DestroyTexture(texture->surface);
    free(texture);
};

void TXTR_free_animation_sheet
(
    animation_sheet_t* sheet
)
{
    TXTR_free(sheet->texture);
    free(sheet);
}

void TXTR_free_animation
(
    animation_t* animation
)
{
    free(animation);
}

