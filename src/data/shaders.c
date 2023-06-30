#include <GL/glu.h>

#include "library.h"

#include "../gfx.h"

shader_program_t* shader_library[SHADER_ALL] = { NULL };


shader_program_t shader_test = {
    SHADER_TEST,
    0,
    (shader_t) {
        -1,
        GL_VERTEX_SHADER,
        "./src/data/shaders/vertex_test.glsl"
    },
    (shader_t) {
        -1,
        GL_FRAGMENT_SHADER,
        "./src/data/shaders/fragment_test.glsl"
    },
};


shader_program_t* LIB_create_shader(
    shader_program_t *blueprint
) {
    int vertex, fragment, program;

    vertex = GFX_compile_shader(blueprint->vertex.type, blueprint->vertex.path);
    fragment = GFX_compile_shader(blueprint->fragment.type, blueprint->fragment.path);
    
    if (GFX_check_shader_compile_status(vertex) == 0) {
        return NULL;
    }
    if (GFX_check_shader_compile_status(fragment) == 0) {
        return NULL;
    }

    program = GFX_link_shaders_to_program(vertex, fragment);

    if (GFX_check_program_link_status(program) == 0) {;
        return NULL;
    }

    blueprint->vertex.id = (int)vertex;
    blueprint->fragment.id = (int)fragment;
    blueprint->id = (int)program;

    return blueprint;
}

void LIB_free_shader(
    shader_program_t *blueprint
) {
    // glDeleteShader 
}

void LIB_free_program(
    shader_program_t *blueprint
) {
    // glDetachShader
}

void LIB_create_shaders_library(
) {
    shader_library[SHADER_TEST] = LIB_create_shader(&shader_test);
};
