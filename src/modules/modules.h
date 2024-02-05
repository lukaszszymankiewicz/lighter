#include <stdbool.h>

#ifndef MODULES_H
#define MODULES_H

enum GFX_MODULE {
    GFX_MODULE_SDL_GL,
    GFX_MODULE_WINDOW,
    GFX_MODULE_GL_CONTEXT,
    GFX_MODULE_GLEW,
    GFX_MODULE_GL_PARAMS,
    GFX_MODULE_PNG,
    GFX_VIEWPORT,
    GFX_MODULE_ALL
};

typedef struct gfx_module_blueprint {
    bool       (*fun)();
} gfx_module_blueprint_t;

typedef struct gfx_module {
    bool        status;
} gfx_module_t;

extern gfx_module_t       *gfx_modules[GFX_MODULE_ALL];

void MOD_init();

#endif
