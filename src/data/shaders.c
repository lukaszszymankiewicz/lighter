#include <GL/glu.h>

#include "library.h"

#include "../gfx.h"
#include "../gl_util.h"

shader_program_t* shader_library[SHADER_ALL] = { NULL };


shader_program_blueprint_t shader_light = {
    "./src/data/shaders/light_vertex.glsl",
    "./src/data/shaders/light_fragment.glsl",
    "",
};

shader_program_blueprint_t shader_texture = {
    "./src/data/shaders/tex_vertex.glsl",
    "./src/data/shaders/tex_fragment.glsl",
    "",
};


void LIB_free_shader(
    shader_program_t *blueprint
) {
    // TODO: to GFX
    // glDeleteShader 
}

void LIB_free_program(
    shader_program_t *blueprint
) {
    // TODO: to GFX
    // glDetachShader
}

void LIB_create_shaders_library(
) {
    shader_library[SHADER_LIGHT] = GFX_create_gl_program(
        shader_light.vertex_shader_path,
        shader_light.fragment_shader_path,
        shader_light.geomentry_shader_path
    );

    shader_library[SHADER_TEXTURE] = GFX_create_gl_program(
        shader_texture.vertex_shader_path,
        shader_texture.fragment_shader_path,
        shader_texture.geomentry_shader_path
    );
};
