#include "global.h"
#include "primitives.h"

#ifndef SPRITES_H
#define SPRITES_H

#define MAX_HITBOXES_PER_FRAME     4
#define MAX_FRAMES_PER_ANIMATION   4
#define MAX_ANIMATION              10

typedef struct frame {
    SDL_Rect  rect;
    SDL_Rect  hit_boxes[MAX_HITBOXES_PER_FRAME];
    int       n_hit_box;
    int       delay;
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

#endif
