#include "./data/library.h"

#include <GL/glew.h>
#include <stdio.h>

#include "render.h"

void RENDER_shader(
    int      shader,
    int      texture,
    GLfloat *vertices,
    int      n_vertices,
    float   *uniforms
) { 
    if (texture != NO_TEXTURE) {
        glBindTexture(GL_TEXTURE_2D, texture);
    }
      
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * n_vertices, vertices, GL_STATIC_DRAW);
    
    int i=0;
    for (int u=0; u<shader_library[shader]->n_uniforms; u++) { 
        int uniform_id = shader_library[shader]->uniform_ids[u];
        glUniform4f(uniform_id, uniforms[i], uniforms[i+1], uniforms[i+2], uniforms[i+3]);
        printf("uniform id :%d ", uniform_id);
        printf("uniforms: %f %f %f %f \n", uniforms[i], uniforms[i+1], uniforms[i+2], uniforms[i+3]);
        i+=MAX_SHADER_UNIFORMS_ARGS;
    }

    int cur_shift = 0;
    int size      = shader_library[shader]->attrib_size;

    for (int i=0; i<shader_library[shader]->n_attribs; i++) {
        GLint attrib = shader_library[shader]->attrib[i];
        glEnableVertexAttribArray(attrib);

        glVertexAttribPointer(
            attrib,
            shader_library[shader]->attrib_shift[i],
            GL_FLOAT,
            GL_FALSE,
            (size*sizeof(float)),
            (void*)(cur_shift*sizeof(float))
        );

        cur_shift+=shader_library[shader]->attrib_shift[i];
    }

    glDrawArrays(GL_POLYGON, 0, (int)(n_vertices/size));
};

