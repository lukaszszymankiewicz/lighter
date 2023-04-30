#include "assets.h"
#include "global.h"
#include "assets.h"
#include "primitives.h"
#include "texture.h"

#ifndef SPRITES_H
#define SPRITES_H

#define MAX_HITBOXES_PER_FRAME     4
#define MAX_FRAMES_PER_ANIMATION   4
#define MAX_ANIMATION              10


typedef struct frame {
    SDL_Rect  rect;
    SDL_Rect  hit_boxes[MAX_HITBOXES_PER_FRAME];
    int       handle_x;
    int       handle_y;
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

extern animation_sheet_t *animations[ASSET_ANIMATION_ALL];

SDL_Rect* ANIM_get_whole_texture_size(animation_sheet_t* animation);
animation_sheet_t* ANIM_init(animation_sheet_t* animation, texture_t* texture);
void ANIM_free(animation_sheet_t* animation);
animation_sheet_t* ANIM_read_animation(const char *filepath);

#endif
