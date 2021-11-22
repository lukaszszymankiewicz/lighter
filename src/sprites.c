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

enum { FRAME_X, FRAME_Y, FRAME_WID, FRAME_HS };

// creates animations (set of sprites with given delay and length). Each sprite from animation is
// just SDL_Rect indicating area to be cut from sprite sheet.
animation_t* TXTR_init_animation (
    int rect[][4],
    int delay,     // delay (in game-frames) before next frame animations occures
    int len        // animation lengths (number of sprites creating given animation)
) {
    animation_t *new_animation = NULL;
    SDL_Rect    *frames        = NULL;
    hit_box_t   *hit_boxes     = NULL;

    new_animation = malloc(sizeof(animation_t));
    frames        = malloc(sizeof(SDL_Rect) * len);
    hit_boxes     = malloc(sizeof(hit_box_t) * len);

    for (int i=0; i<len; i++) {
        frames[i] = (SDL_Rect){rect[i][FRAME_X], rect[i][FRAME_Y], rect[i][FRAME_WID], rect[i][FRAME_HS]};
    }
    new_animation->frames    = frames;
    new_animation->delay     = delay;
    new_animation->len       = len;
    new_animation->hit_boxes = hit_boxes;        

    return new_animation;
}

hit_box_t* TXTR_init_hitbox (
    int rect[][4],
    int len        // hitbox lengths (number of sprites creating given animation)
) {
    hit_box_t *hit_box     = NULL;
    hit_box                = malloc(sizeof(hit_box_t));
    hit_box->rects         = malloc(sizeof(SDL_Rect) * len);

    for (int i=0; i<len; i++) {
        hit_box->rects[i] = (SDL_Rect){rect[i][FRAME_X], rect[i][FRAME_Y], rect[i][FRAME_WID], rect[i][FRAME_HS]};
    }

    hit_box->len = len;

    return hit_box;
}

// cuts animation sheet according to inputted rects and change it into animation struct
void TXTR_push_animation (
    animation_sheet_t *sheet,       // animation sheet stuct holding image to be cut
    int                index,       // index of animation (every animation should have unique index value)
    int                rects[][4],  // rects of framesheet
    int                delay,       // delay (in game-frames) before next frame animations occures
    int                len          // animation lengths (number of sprites creating given animation)
) {
    sheet->animations[index] = *(TXTR_init_animation(rects, delay, len));
}

// cuts animation sheet according to inputted rects and change it into animation struct
void TXTR_push_hitbox(
    animation_sheet_t *sheet,           // animation sheet stuct holding image to be cut
    int                index,           // index of animation (every animation should have unique index value)
    int                hit_box_index,   // index of animation (every animation should have unique index value)
    int                rects[][4],      // rects of framesheet
    int                len              // hit box lengths (number of sprites creating given animation)
) {
    sheet->animations[index].hit_boxes[hit_box_index] = *(TXTR_init_hitbox(rects, len));
}

void TXTR_free_animation_sheet(animation_sheet_t *sheet) {
    GFX_free_texture(sheet->texture);
    free(sheet);
}

void TXTR_free_animation(animation_t *animation) {
    free(animation);
}

void TXTR_free_hitbox(hit_box_t *hit_box) {
    free(hit_box);
}
