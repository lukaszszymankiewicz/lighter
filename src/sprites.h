#include <SDL2/SDL.h>
#include <stdbool.h>

#ifndef SPRITES_H
#define SPRITES_H

typedef struct texture {
    SDL_Texture* surface;
    int width;
    int height;
} Texture;

typedef struct animation {
    int delay;
    int len;
    SDL_Rect* frames;
} animation_t;

typedef struct animation_sheet {
    Texture* texture;
    int n_animations;
    animation_t* animations;
} animation_sheet_t;

Texture* TXTR_init_texture(char *filepath);
void TXTR_render_texture(Texture* texture, SDL_Rect* clip, int x, int y, bool flip);
void TXTR_draw_light(int x, int y);
void TXTR_load_frames();
void TXTR_free(Texture *texture);
void TXTR_free_all();

animation_sheet_t* TXTR_init_animation_sheet(char *filepath, int n_animations);
animation_t* TXTR_init_animation(int xs[], int ys[], int ws[], int hs[], int delay, int len);
void TXTR_push_animation(animation_sheet_t*, int, int xs[], int ys[], int ws[], int hs[], int, int);
void TXTR_free_animation_sheet(animation_sheet_t* sheet);
void TXTR_free_animation(animation_t* animation);

#endif
