#include "sprites.h"
#include "gfx.h"

// implementation of game sprite

// creates animation sheet (set of animations)
animation_sheet_t* TXTR_init_animation_sheet
(
    char *filepath,     // path to animation sheet image
    int n_animations    // number of animation to created from sheet
)
{
    animation_sheet_t* new_sheet = malloc(sizeof(animation_sheet_t));

    new_sheet->texture = TXTR_init_texture(filepath);
    new_sheet->n_animations = n_animations;
    animation_t* animations = malloc(sizeof(animation_t) * n_animations);
    new_sheet->animations=animations;

    return new_sheet;
}


// creates animations (set of sprites wit given delay and length). Each sprite from animation is
// just SDL_Rect indicating area to be cut from image sheet.
animation_t* TXTR_init_animation
(
    int xs[],      // x-axis points of rect begginig
    int ys[],      // y-axis points of rect begginig
    int ws[],      // rect widths
    int hs[],      // rect heaights
    int delay,     // delay (in game-frames) before next frame animations occures
    int len        // animation lengths (number of sprites creating given animation)
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

// cuts animation sheet according to inputted rects and change it into animation struct
void TXTR_push_animation
(
    animation_sheet_t* sheet, // animation sheet stuct holding image to be cut
    int index,     // index of animation (every animation should have unique index value)
    int xs[],      // x-axis points of rect begginig
    int ys[],      // y-axis points of rect begginig
    int ws[],      // rect widths
    int hs[],      // rect heaights
    int delay,     // delay (in game-frames) before next frame animations occures
    int len        // animation lengths (number of sprites creating given animation)
)
{
    sheet->animations[index] = *(TXTR_init_animation(xs, ys, ws, hs, delay, len));
}

// initalize texture stuct from given image file
Texture* TXTR_init_texture(char *filepath) 
{
	SDL_Texture* new_texture = NULL;
    SDL_Surface* loaded_surface = NULL;

    loaded_surface = IMG_Load(filepath);
    SDL_SetColorKey(loaded_surface, SDL_TRUE, SDL_MapRGB(loaded_surface->format, 0x80, 0xFF, 0xFF));

    new_texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);

    Texture* p = malloc(sizeof(Texture));

    p->surface = new_texture;
    p->width = loaded_surface->w;
    p->height = loaded_surface->h;

    SDL_FreeSurface(loaded_surface);

    return p;
};

// renders texture to screen
void TXTR_render_texture(
    Texture* texture,     // Texture to be rendered
    SDL_Rect* clip,       // rect from texture to be rendered
    int x,                // x coord of screen to have texture rendered
    int y,                // y coord of screen to have texture rendered
    bool flip             // indiaction if texture should be flipped horizontally
) 
{
    SDL_Rect render_quad = {x, y, texture->width, texture->height};
    SDL_RendererFlip flip_tex;

    // if clip is not given render whole texture
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

void TXTR_free_animation_sheet(animation_sheet_t* sheet)
{
    TXTR_free(sheet->texture);
    free(sheet);
}

void TXTR_free_animation(animation_t* animation)
{
    free(animation);
}

