#include "library.h"

#include "../gfx.h"

gfx_module_t* gfx_module_library[GFX_MODULE_ALL] = { NULL };

gfx_module_blueprint_t init_sdl_gl     = (gfx_module_blueprint_t){ GFX_init_sdl_with_gl };
gfx_module_blueprint_t init_window     = (gfx_module_blueprint_t){ GFX_init_window };
gfx_module_blueprint_t init_gl_context = (gfx_module_blueprint_t){ GFX_create_gl_context };
gfx_module_blueprint_t init_glew       = (gfx_module_blueprint_t){ GFX_init_glew };
gfx_module_blueprint_t init_gl_params  = (gfx_module_blueprint_t){ GFX_init_gl_params };
gfx_module_blueprint_t init_png        = (gfx_module_blueprint_t){ GFX_init_png };
gfx_module_blueprint_t init_scale      = (gfx_module_blueprint_t){ GFX_set_global_render_scale };

gfx_module_t* LIB_init_gfx_module(
    bool       (*fun)()
) {
    gfx_module_t* module = NULL;

    module               = (gfx_module_t*)malloc(sizeof(gfx_module_t));
    module->status       = fun();

    return module;
}

void LIB_init_gfx_modules(
) {
    gfx_module_library[GFX_MODULE_SDL_GL]     = LIB_init_gfx_module(init_sdl_gl.fun);
    gfx_module_library[GFX_MODULE_WINDOW]     = LIB_init_gfx_module(init_window.fun);
    gfx_module_library[GFX_MODULE_GL_CONTEXT] = LIB_init_gfx_module(init_gl_context.fun);
    gfx_module_library[GFX_MODULE_GLEW]       = LIB_init_gfx_module(init_glew.fun);
    gfx_module_library[GFX_MODULE_GL_PARAMS]  = LIB_init_gfx_module(init_gl_params.fun);
    gfx_module_library[GFX_MODULE_PNG]        = LIB_init_gfx_module(init_png.fun);
    gfx_module_library[GFX_MODULE_SCALE]      = LIB_init_gfx_module(init_scale.fun);
}

void LIB_init_all_modules(
) {
    LIB_init_gfx_modules();
}
