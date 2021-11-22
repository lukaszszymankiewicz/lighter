#include "def.h"
#include "texture.h"
#include "primitives.h"

#ifndef SPRITES_H
#define SPRITES_H

#define MAX_HITBOXES_PER_FRAME 4

typedef struct hit_box {
    int       len;
    SDL_Rect *rects;
} hit_box_t;

typedef struct animation {
    int        delay;
    int        len;
    SDL_Rect  *frames;
    hit_box_t *hit_boxes;
} animation_t;

typedef struct animation_sheet {
  texture_t   *texture;
  int          n_animations;
  animation_t *animations;
} animation_sheet_t;

void TXTR_draw_light(int x, int y);
void TXTR_load_frames();
void TXTR_free_all();

animation_sheet_t *TXTR_init_animation_sheet(char *filepath, int n_animations);
animation_t *TXTR_init_animation(int rects[][4], int delay, int len);
void TXTR_push_animation(animation_sheet_t *, int index, int rects[][4], int delay, int len);
void TXTR_free_animation_sheet(animation_sheet_t *sheet);
void TXTR_free_animation(animation_t *animation);

hit_box_t* TXTR_init_hitbox(int rect[][4], int len);
void TXTR_free_hitbox(hit_box_t *hit_box);
void TXTR_push_hitbox(animation_sheet_t *sheet, int index, int hit_box_index, int rects[][4], int len);

#endif
