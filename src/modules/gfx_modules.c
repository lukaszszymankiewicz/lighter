#include "modules.h"

#include "../gfx.h"

gfx_module_t* gfx_modules[GFX_MODULE_ALL] = { NULL };

gfx_module_blueprint_t init_sdl_gl     = (gfx_module_blueprint_t){ GFX_init_sdl_with_gl };
gfx_module_blueprint_t init_window     = (gfx_module_blueprint_t){ GFX_init_window };
gfx_module_blueprint_t init_gl_context = (gfx_module_blueprint_t){ GFX_create_gl_context };
gfx_module_blueprint_t init_glew       = (gfx_module_blueprint_t){ GFX_init_glew };
gfx_module_blueprint_t init_gl_params  = (gfx_module_blueprint_t){ GFX_init_gl_params };
gfx_module_blueprint_t init_png        = (gfx_module_blueprint_t){ GFX_init_png };
gfx_module_blueprint_t viewport        = (gfx_module_blueprint_t){ GFX_set_viewport };

gfx_module_t* MOD_init_gfx_module(
    bool       (*fun)()
) {
    gfx_module_t* module = NULL;

    module               = (gfx_module_t*)malloc(sizeof(gfx_module_t));
    module->status       = fun();

    return module;
}

void MOD_init_gfx(
) {
    gfx_modules[GFX_MODULE_SDL_GL]     = MOD_init_gfx_module(init_sdl_gl.fun);
    gfx_modules[GFX_MODULE_WINDOW]     = MOD_init_gfx_module(init_window.fun);
    gfx_modules[GFX_MODULE_GL_CONTEXT] = MOD_init_gfx_module(init_gl_context.fun);
    gfx_modules[GFX_MODULE_GLEW]       = MOD_init_gfx_module(init_glew.fun);
    gfx_modules[GFX_MODULE_GL_PARAMS]  = MOD_init_gfx_module(init_gl_params.fun);
    gfx_modules[GFX_MODULE_PNG]        = MOD_init_gfx_module(init_png.fun);
    gfx_modules[GFX_VIEWPORT]          = MOD_init_gfx_module(viewport.fun);
}

void MOD_init(
) {
    MOD_init_gfx();
}
