#include <SDL2/SDL.h>

#include "img.h"

#ifndef SPRITES_H
#define SPRITES_H

#define MAX_ANIMATION              10
#define MAX_HITBOXES_PER_FRAME     4
#define MAX_FRAMES_PER_ANIMATION   4

typedef struct frame {
    render_coord_t  img;
    SDL_Rect        rect;
    SDL_Rect        hit_boxes[MAX_HITBOXES_PER_FRAME];
    int             handle_x;
    int             handle_y;
    int             n_hit_box;
    int             delay;
} frame_t;

typedef struct animation {
    int        len;
    int        wobble_id;
    frame_t    frames[MAX_FRAMES_PER_ANIMATION];
} animation_t;

typedef struct animation_sheet {
  int          texture_id;
  int          n_animations;
  int          width;
  int          height;
  animation_t  animations[MAX_ANIMATION];
} animation_sheet_t;

render_coord_t ANIM_texture_coord(animation_t animation, int idx, int frame);
render_coord_t ANIM_texture_coord_full(animation_sheet_t animation);

int ANIM_get_texture_width(animation_sheet_t animation);
int ANIM_get_texture_height(animation_sheet_t animation);

void ANIM_free(animation_sheet_t* animation);

#endif
