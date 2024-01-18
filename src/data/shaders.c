#include <GL/glu.h>

#include "library.h"

#include "../gfx.h"

shader_program_t* shader_library[SHADER_ALL] = { NULL };

shader_program_blueprint_t shader_light = {
    SHADER_LIGHT,
    "./src/data/shaders/light_vertex.glsl",
    "./src/data/shaders/light_fragment.glsl",
    "",
};

shader_program_blueprint_t shader_texture = {
    SHADER_TEXTURE,
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

void LIB_read_shader_blueprint(
    shader_program_blueprint_t *blueprint
) {
    shader_program_t* shader = GFX_create_gl_program(
        blueprint->vertex_shader_path,
        blueprint->fragment_shader_path,
        blueprint->geomentry_shader_path
    );

    if (shader) {
        printf("SHADER ID = %d read\n", blueprint->id);
    }
    shader_library[blueprint->id] = shader;
};

void LIB_create_shaders_library(
) {
    LIB_read_shader_blueprint(&shader_light);
    LIB_read_shader_blueprint(&shader_texture);
};
