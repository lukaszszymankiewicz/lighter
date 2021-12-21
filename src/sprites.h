#include "global.h"
#include "primitives.h"

#ifndef SPRITES_H
#define SPRITES_H

#define MAX_HITBOXES_PER_FRAME   4
#define MAX_FRAMES_PER_ANIMATION 4
#define MAX_ANIMATION            10

enum { FRAME_X, FRAME_Y, FRAME_WID, FRAME_HS };

typedef struct frame {
    SDL_Rect  rect;
    SDL_Rect  hit_boxes[MAX_HITBOXES_PER_FRAME];
    int n_hit_box;
} frame_t;

typedef struct animation {
    int        delay;
    int        len;
    frame_t    frames[MAX_FRAMES_PER_ANIMATION];
} animation_t;

typedef struct animation_sheet {
  texture_t   *texture;
  int          n_animations;
  animation_t *animations[MAX_ANIMATION];

} animation_sheet_t;

animation_sheet_t *TXTR_init_animation_sheet(char *filepath, int n_animations);
animation_t* TXTR_init_animation(int rect[][4], int hit_boxes[][4], int delay, int n_frames, int n_hit_box[]);

void TXTR_push_animation(animation_sheet_t *sheet, int index, int rects[][4], int hit_boxes[][4], int delay, int n_frames, int n_hit_box[]);
void TXTR_free_animation_sheet(animation_sheet_t *sheet);

#endif
