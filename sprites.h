#include <SDL2/SDL.h>

#ifndef SPRITES_H
#define SPRITES_H

typedef struct texture {
    SDL_Texture * surface;
    int width;
    int height;
} Texture;

Texture * TXTR_init_texture(char *filepath);
void TXTR_render_texture(Texture* texture, SDL_Rect* clip, int x, int y);
void TXTR_draw_light(int x, int y);
void TXTR_load_frames();
void TXTR_free(Texture *texture);
void TXTR_free_all();

#endif
