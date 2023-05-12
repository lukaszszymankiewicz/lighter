#include <SDL2/SDL.h>

#include "img.h"
#include "texture.h"

#ifndef SPRITES_H
#define SPRITES_H

#define MAX_ANIMATION              10
#define MAX_HITBOXES_PER_FRAME     4
#define MAX_FRAMES_PER_ANIMATION   4


typedef struct frame {
    render_img_t  img;
    SDL_Rect      rect;
    SDL_Rect      hit_boxes[MAX_HITBOXES_PER_FRAME];
    int           handle_x;
    int           handle_y;
    int           n_hit_box;
    int           delay;
} frame_t;

typedef struct animation {
    int        len;
    frame_t    frames[MAX_FRAMES_PER_ANIMATION];
} animation_t;

typedef struct animation_sheet {
  texture_t   *texture;
  int          n_animations;
  animation_t  animations[MAX_ANIMATION];
} animation_sheet_t;

SDL_Rect* ANIM_get_whole_texture_size(animation_sheet_t* animation);

void ANIM_free(animation_sheet_t* animation);

render_img_t ANIM_texture_coord(animation_t *animation, int idx, int frame);
render_img_t ANIM_texture_coord_full(animation_sheet_t* animation);

#endif
