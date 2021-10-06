#include "def.h"
#include "primitives.h"

#ifndef SPRITES_H
#define SPRITES_H

typedef struct animation {
    int       delay;
    int       len;
    SDL_Rect *frames;
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
animation_t *TXTR_init_animation(int xs[], int ys[], int ws[], int hs[], int delay, int len);
void TXTR_push_animation(animation_sheet_t *, int, int xs[], int ys[], int ws[], int hs[], int, int);
void TXTR_free_animation_sheet(animation_sheet_t *sheet);
void TXTR_free_animation(animation_t *animation);

#endif
