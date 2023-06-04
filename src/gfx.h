#include <SDL2/SDL.h>

#ifndef GFX_H
#define GFX_H

extern SDL_Window *window;

extern float global_x_scale;
extern float global_y_scale;

int GFX_init_graphics();

void GFX_free();
void GFX_clear_screen();
void GFX_update();

#endif
