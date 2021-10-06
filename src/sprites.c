#include "sprites.h"
#include "gfx.h"

animation_sheet_t* TXTR_init_animation_sheet (
    char *filepath,     // path to animation sheet image
    int n_animations    // number of animation to created from sheet
) {
    animation_sheet_t* new_sheet = malloc(sizeof(animation_sheet_t));

    new_sheet->texture      = GFX_read_texture(filepath);
    new_sheet->n_animations = n_animations;
    animation_t* animations = malloc(sizeof(animation_t) * n_animations);
    new_sheet->animations   = animations;

    return new_sheet;
}


// creates animations (set of sprites with given delay and length). Each sprite from animation is
// just SDL_Rect indicating area to be cut from sprite sheet.
animation_t* TXTR_init_animation (
    int xs[],      // x-axis points of rect begginig
    int ys[],      // y-axis points of rect begginig
    int ws[],      // rect widths
    int hs[],      // rect heaights
    int delay,     // delay (in game-frames) before next frame animations occures
    int len        // animation lengths (number of sprites creating given animation)
) {
    animation_t* new_animation = NULL;
    SDL_Rect* frames           = NULL;

    new_animation = malloc(sizeof(animation_t));
    frames        = malloc(sizeof(SDL_Rect) * len);

    for (int i=0; i<len; i++) {
        frames[i] = (SDL_Rect){xs[i], ys[i], ws[i], hs[i]};
    }
    new_animation->frames = frames;
    new_animation->delay  = delay;
    new_animation->len    = len;
        
    return new_animation;
}

// cuts animation sheet according to inputted rects and change it into animation struct
void TXTR_push_animation (
    animation_sheet_t *sheet,  // animation sheet stuct holding image to be cut
    int                index,  // index of animation (every animation should have unique index value)
    int                xs[],   // x-axis points of rect begginig
    int                ys[],   // y-axis points of rect begginig
    int                ws[],   // rect widths
    int                hs[],   // rect heaights
    int                delay,  // delay (in game-frames) before next frame animations occures
    int                len     // animation lengths (number of sprites creating given animation)
) {
    sheet->animations[index] = *(TXTR_init_animation(xs, ys, ws, hs, delay, len));
}

void TXTR_free_animation_sheet(animation_sheet_t *sheet) {
    GFX_free_texture(sheet->texture);
    free(sheet);
}

void TXTR_free_animation(animation_t *animation) {
    free(animation);
}

