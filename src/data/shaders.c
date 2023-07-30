#include <GL/glu.h>

#include "library.h"

#include "../gfx.h"
#include "../gl_util.h"

shader_program_t* shader_library[SHADER_ALL] = { NULL };


shader_program_blueprint_t shader_test = {
    SHADER_TEST,
    0, 1, 0,  // uniforms number
    {},
    {"lightPolygonColor"},
    {},
    "./src/data/shaders/vertex_test.glsl",
    "./src/data/shaders/fragment_test.glsl",
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
    shader_library[SHADER_TEST] = GFX_create_gl_program(
        shader_test.vertex_shader_path,
        shader_test.fragment_shader_path,
        shader_test.geomentry_shader_path,
        shader_test.n_uniforms_vertex,
        shader_test.n_uniforms_fragment,
        shader_test.n_uniforms_geometry,
        shader_test.shader_uniforms,
        shader_test.fragment_uniforms,
        shader_test.geometry_uniforms
    );
};
