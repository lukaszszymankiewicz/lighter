#include "./data/library.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <stdio.h>

#include "render.h"

void RENDER_shader_texture(
    int      shader,
    int      texture,
    GLfloat *vertices,
    int      n_vertices,
    float   *uniforms
) { 
    printf("floats provided: %d \n", n_vertices);
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * n_vertices, vertices, GL_STATIC_DRAW);

    // pos
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // tex
    // GLint texAttrib = shader_library[shader]->attrib[2];
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // TODO: this can be important but i cannot tell by now :C
    // texture
    GLint uniform_id = shader_library[shader]->uniform_ids[0];
    glUniform4f(uniform_id, uniforms[0], uniforms[1], uniforms[2], uniforms[3]);

    // GLint uniform_loc = shader_library[shader]->n_uniforms;
    // printf("number of uniforms: %d \n", uniform_loc);
    // glUniform1i(uniform_loc, texture);
    // glBindTexture(GL_TEXTURE_2D, texture);

    printf("triangles to render: %d \n", (int)(n_vertices/12) );
    glBindVertexArray(VAO);
    // glDrawElements(GL_TRIANGLES, (int)(n_vertices/12), GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, (int)(n_vertices/4));
    // glDrawElements(GL_TRIANGLES, n_vertices, GL_UNSIGNED_INT, 0);
    // glDrawArrays(GL_POLYGON, 0, n_vertices);
}

// TODO: each shader should have its own small function to render it
void RENDER_shader_light(
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

