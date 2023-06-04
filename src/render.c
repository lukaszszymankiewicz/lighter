#include <GL/glew.h>
// #include <GL/gl.h>
// #include <GL/glu.h>

#include <stdbool.h>

#include "render.h"
#include "texture.h"

void RENDER_texture(
    texture_t *texture,
    float      render_x1,   // place on screen (OpenGL coords)
    float      render_y1,   // place on screen (OpenGL coords)
    float      render_x2,   // place on screen (OpenGL coords)
    float      render_y2,   // place on screen (OpenGL coords)
    float      clip_x1,      // position on clipture (OpenGL coords)
    float      clip_y1,      // position on clipture (OpenGL coords)
    float      clip_x2,      // position on clipture (OpenGL coords)
    float      clip_y2,      // position on clipture (OpenGL coords)
    bool       flip
) {
    // printf("binded clipture id: %d \n", clipture->id);
    // printf("clipture name: %s \n", clipture->filepath);
    glBindTexture(GL_TEXTURE_2D, texture->id);

    // clipture is always rendered 1:1 to achieve pixel perfect effect
    // float clipW = (float)TXTR_width(clipture);
    // float clipH = (float)TXTR_height(clipture);

    // orient it in a way that OpenGL will digest it
    // render_x1 -= (float)SCREEN_WIDTH / 2.0;
    // render_y1 -= (float)SCREEN_HEIGHT / 2.0;

    // render_y1 *= -1;
    // float render_y2 = (float)render_y1 - ((clip_y2 - clip_y1) * clipH);
    // float render_x2 = (float)render_x1 + ((clip_x2 - clip_x1) * clipW);
    // float render_y2 = (float)render_y1 - (float)TILE_WIDTH;
    // float render_x2 = (float)render_x1 + (float)TILE_HEIGHT;

    float temp;

    if (flip) {
        temp = clip_x1; 
        clip_x1 = clip_x2;
        clip_x2 = temp;
    }

    printf("render on : %f, %f |  %f, %f \n",
        render_x1,
        render_y1,
        render_x2,
        render_y2
    );

    printf("clip on : %f, %f |  %f, %f \n",
        clip_x1,
        clip_y1,
        clip_x2,
        clip_y2
    );

    // printf("this is correct: %f, %f | %f, %f \n",
    //     global_x_scale * (float)render_x1/(float)SCREEN_WIDTH,
    //     global_y_scale * (float)render_y1/(float)SCREEN_HEIGHT,
    //     global_x_scale * (float)render_x2/(float)SCREEN_WIDTH,
    //     global_y_scale * (float)render_y2/(float)SCREEN_HEIGHT
    // );
    
    glBegin(GL_QUADS);
        // up right (1,1)
        glTexCoord2f(clip_x2, clip_y1);
        glVertex2f(render_x2, render_y1);
        // up left (-1, 1)
        glTexCoord2f(clip_x1, clip_y1);
        glVertex2f(render_x1, render_y1);
        // down left (-1, -1)
        glTexCoord2f(clip_x1, clip_y2);
        glVertex2f(render_x1, render_y2);
        // down right (1, -1)
        glTexCoord2f(clip_x2, clip_y2); 
        glVertex2f(render_x2, render_y2);
    glEnd();
};
