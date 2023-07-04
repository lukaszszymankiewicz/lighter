#include "library.h"

#include "../gfx.h"

gfx_module_t* gfx_module_library[GFX_MODULE_ALL] = { NULL };

gfx_module_blueprint_t init_sdl = (gfx_module_blueprint_t){ GFX_init_video, "SDL GFX library" };
gfx_module_blueprint_t init_sdl_gl = (gfx_module_blueprint_t){ GFX_init_sdl_with_gl, "OpenGL with SDL" };
gfx_module_blueprint_t init_window = (gfx_module_blueprint_t){ GFX_init_window, "Window" };
gfx_module_blueprint_t init_gl_context = (gfx_module_blueprint_t){ GFX_create_gl_context, "GL Context" };
gfx_module_blueprint_t init_glew = (gfx_module_blueprint_t){ GFX_init_glew, "GLEW library" };
gfx_module_blueprint_t init_vsync = (gfx_module_blueprint_t){ GFX_init_vsync, "VSYNC" };
gfx_module_blueprint_t init_gl_params = (gfx_module_blueprint_t){ GFX_init_gl_params, "GL params" };
gfx_module_blueprint_t init_png = (gfx_module_blueprint_t){ GFX_init_png, "PNG filesystem" };
gfx_module_blueprint_t init_viewport = (gfx_module_blueprint_t){ GFX_set_viewport, "Viewport" };

gfx_module_t* LIB_init_gfx_module(
    bool       (*fun)(),
    const char  *name
) {
    gfx_module_t* module = NULL;
    module               = (gfx_module_t*)malloc(sizeof(gfx_module_t));

    module->error_symbol = GRAPHIC_ERROR_SYMBOL;
    module->status       = fun();
    module->name         = name;

    return module;
}

int LIB_check_modules(
) {
    for (int i=0; i<GFX_MODULE_ALL; i++) {
        if (gfx_module_library[i]->status == false) {
            printf("ERROR %c%d: ", gfx_module_library[i]->error_symbol, i);
            printf("%s cannot be initialized! \n", gfx_module_library[i]->name);
            // TODO: run method to log error?
            return 0;
        }
    }
    return 1;
}

void LIB_gfx_module_free(
    gfx_module_t* module
) {
    free(module);
    module = NULL;
}

void LIB_init_gfx_modules(
) {
    gfx_module_library[GFX_MODULE_SDL] = LIB_init_gfx_module(init_sdl.fun, init_sdl.name);
    gfx_module_library[GFX_MODULE_SDL_GL] = LIB_init_gfx_module(init_sdl_gl.fun, init_sdl_gl.name);
    gfx_module_library[GFX_MODULE_WINDOW] = LIB_init_gfx_module(init_window.fun, init_window.name);
    gfx_module_library[GFX_MODULE_GL_CONTEXT] = LIB_init_gfx_module(init_gl_context.fun, init_gl_context.name);
    gfx_module_library[GFX_MODULE_GLEW] = LIB_init_gfx_module(init_glew.fun, init_glew.name);
    gfx_module_library[GFX_MODULE_VSYNC] = LIB_init_gfx_module(init_vsync.fun, init_vsync.name);
    gfx_module_library[GFX_MODULE_GL_PARAMS] = LIB_init_gfx_module(init_gl_params.fun, init_gl_params.name);
    gfx_module_library[GFX_MODULE_PNG] = LIB_init_gfx_module(init_png.fun, init_png.name);
    gfx_module_library[GFX_MODULE_VIEWPORT] = LIB_init_gfx_module(init_viewport.fun, init_viewport.name);
}

void LIB_init_all_modules(
) {
    LIB_init_gfx_modules();
}
