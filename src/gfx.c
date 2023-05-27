#include <stdbool.h>
#include <stdio.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "data/library.h"

#include "gfx.h"
#include "img.h"

#define GRADIENT_COEF 3.0
#define OPENGL_MAJOR_VERSION 3
#define OPENGL_MINOR_VERSION 3

GLint  gVertexPos2DLocation = -1;
GLuint opengl_program_id    = 0;
GLuint gVBO                 = 0;
GLuint gIBO                 = 0;

float global_x_scale        = 1.0;
float global_y_scale        = 1.0;

bool gRenderQuad = true;

SDL_GLContext gl_context        = NULL;
SDL_Window   *window            = NULL;

void GFX_print_program_log(
    GLuint program 
) {
    //Make sure name is shader
    if(glIsProgram(program)) {
        //Program log length
        int infoLogLength = 0;
        int maxLength = infoLogLength;
        
        //Get info string length
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &maxLength );
        
        //Allocate string
        char* infoLog = NULL;
        infoLog = (char*)malloc(maxLength * sizeof(char));
        
        //Get info log
        glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
        if( infoLogLength > 0 ) {
            printf( "%s\n", infoLog );
        }
        
        free(infoLog);
    }
    else {
        printf("Name %d is not a program\n", program);
    }
}

void GFX_print_shader_log(
    GLuint shader 
) {
    //Make sure name is shader
    if(glIsShader(shader)) {
        //Shader log length
        int infoLogLength = 0;
        int maxLength = infoLogLength;
        
        //Get info string length
        glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &maxLength );
        
        //Allocate string
        char* infoLog = NULL;
        infoLog = (char*)malloc(maxLength * sizeof(char));
        
        //Get info log
        glGetShaderInfoLog( shader, maxLength, &infoLogLength, infoLog );
        if(infoLogLength > 0) {
            //Print Log
            printf( "%s\n", infoLog );
        }

        //Deallocate string
        free(infoLog);
    }
    else {
        printf( "Name %d is not a shader\n", shader );
    }
}

// sets global rendering scale which must be a positive integer. Scale tries to fit best tile_per_x 
// and tile_per_y. This is needed to achieve pixel-perfect rendering (which can be done only if each
// pixel in every drawing routine is multiplied by some scale).
void GFX_set_global_render_scale(
) {
    float tile_per_x = 10.0;
    float tile_per_y = 7.0;

    GLint m_viewport[4];
    glGetIntegerv(GL_VIEWPORT, m_viewport);
    // printf("viewport: %i, %i %i, %i\n", m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
    
    float max_screen_w = (float)m_viewport[2];
    float max_screen_h = (float)m_viewport[3];

    int closestSclaleX = (int)((float)max_screen_w / ((float)TILE_WIDTH * tile_per_x));
    int closestSclaleY = (int)((float)max_screen_h / ((float)TILE_HEIGHT * tile_per_y));
    
    // best scaling is chosen (fit most of tile_per_x or tile_per_y)
    int scale = MAX(closestSclaleX, closestSclaleY);
    
    // global scale is calculated and set
    global_x_scale = (float)TILE_WIDTH / (max_screen_w / 2.0 / tile_per_x) * closestSclaleX;
    global_y_scale = (float)TILE_HEIGHT / (max_screen_h / 2.0 / tile_per_y) * closestSclaleX;
}

int GFX_init_window() {
    window = SDL_CreateWindow(
        GAME_NAME,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        DEFAULT_SCREEN_WIDTH,
        DEFAULT_SCREEN_HEIGHT,
        SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL 
    );

    if (!window) {
        printf("window cannot be created");
        return 0;
    }

    SDL_ShowCursor(SDL_DISABLE); // disable cursor on window

    return 1;
};

int GFX_init_video() {
    int init_video = SDL_Init(SDL_INIT_VIDEO);

    if(init_video) {
        printf("video cannot be initialized!");
        return 0;
    };

    SDL_GL_SetAttribute(OPENGL_MAJOR_VERSION, OPENGL_MAJOR_VERSION);
    SDL_GL_SetAttribute(OPENGL_MINOR_VERSION, OPENGL_MINOR_VERSION);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    return 1;
};

bool GFX_init_OpenGL_shaders(
) {
	//Generate program
	opengl_program_id = glCreateProgram();

	//Create vertex shader
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);

	//Get vertex source
	const GLchar* vertex_shader_src[] = {
		"#version 140\nin vec2 LVertexPos2D; void main() { gl_Position = vec4( LVertexPos2D.x, LVertexPos2D.y, 0, 1 ); }"
	};

	//Set vertex source
	glShaderSource(vertex_shader, 1, vertex_shader_src, NULL);

	//Compile vertex source
	glCompileShader(vertex_shader);

	//Check vertex shader for errors
	GLint vertex_shader_compiled = GL_FALSE;
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &vertex_shader_compiled);

	if(vertex_shader_compiled != GL_TRUE) {
		printf("Unable to compile vertex shader %d!\n", vertex_shader);
		GFX_print_shader_log(vertex_shader);

        return false;
    }

    // Attach vertex shader to program
    glAttachShader(opengl_program_id, vertex_shader);

    // Create fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Get fragment source
    const GLchar* fragmentShaderSource[] = {
        "#version 140\nout vec4 LFragment; void main() { LFragment = vec4( 1.0, 1.0, 1.0, 1.0 ); }"
    };

    // Set fragment source
    glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);

    //Compile fragment source
    glCompileShader(fragmentShader);

    // Check fragment shader for errors
    GLint fShaderCompiled = GL_FALSE;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);

    if(fShaderCompiled != GL_TRUE) {
        printf("Unable to compile fragment shader %d!\n", fragmentShader );
        GFX_print_shader_log(fragmentShader);
        return false;
    }

    // Attach fragment shader to program
    glAttachShader(opengl_program_id, fragmentShader);

    // Link program
    glLinkProgram(opengl_program_id);

    //Check for errors
    GLint programSuccess = GL_TRUE;
    glGetProgramiv( opengl_program_id, GL_LINK_STATUS, &programSuccess );

    if(programSuccess != GL_TRUE) {
        printf("Error linking program %d!\n", opengl_program_id );
        GFX_print_program_log(opengl_program_id);
        return false;
    }

    //Get vertex attribute location
    gVertexPos2DLocation = glGetAttribLocation(opengl_program_id, "LVertexPos2D");

    if(gVertexPos2DLocation == -1) {
        printf("LVertexPos2D is not a valid glsl program variable!\n" );
        return false;
    }

    // Initialize clear color
    glClearColor(0.f, 0.f, 0.f, 1.f);

    //VBO data
    GLfloat vertexData[] =
    {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f
    };

    // IBO data
    GLuint indexData[] = { 0, 1, 2, 3 };

    // Create VBO
    glGenBuffers(1, &gVBO );
    glBindBuffer(GL_ARRAY_BUFFER, gVBO );
    glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

    // Create IBO
    glGenBuffers(1, &gIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO );
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW);
	
	return true;
}

int GFX_init_renderer() {
    gl_context = SDL_GL_CreateContext(window);

    if(!gl_context) {
        printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
        return 0;
    }

    GLenum glew_err = glewInit();

    if(glew_err != GLEW_OK) {
        printf( "Error initializing GLEW! %s\n", glewGetErrorString(glew_err));
        return 0;
    }

    if(SDL_GL_SetSwapInterval( 1 ) < 0) {
        printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
        return 0;
    }

    // Initialize OpenGL
    // if(!GFX_init_OpenGL_shaders()) {
    //     printf("Unable to initialize OpenGL!\n");
    //     return 0;
    // }

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    GFX_set_global_render_scale();

    return 1;
}

int GFX_init_png(
) {
    int imgFlags = IMG_INIT_PNG;

    if(!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return 0;
    }

    return 1;
};

int GFX_init_graphics(
) {

    if(!GFX_init_video()) {
        printf("Unable to initialize Video Settings\n");
        return 0;
    }

    if(!GFX_init_window()) {
        printf("Unable to initialize Window\n");
        return 0;
    }

    if(!GFX_init_renderer()) {
        printf("Unable to initialize Renderer\n");
        return 0;
    }

    // TODO: check if this is needed
    if(!GFX_init_png()) {
        printf("Unable to initialize PNG images rendering\n");
        return 0;
    }

    // printf("after gfx init\n");
    GLint m_viewport[4];
    glGetIntegerv(GL_VIEWPORT, m_viewport);
    // printf("viewport: %i, %i %i, %i\n", m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
    
    return 1;
};

void GFX_free(
) {
    if (gl_context) {
        glDeleteProgram(opengl_program_id);
    }

    if (window) {
        SDL_DestroyWindow(window);
    }

    IMG_Quit();
};

void GFX_clear_screen() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    uint32_t z = 0;
    // for (int i=0; i<10; i++) {
    //     glClearTexImage(i, 0, GL_RGBA, GL_UNSIGNED_BYTE, &z);
    // }
};

void GFX_update() {
    SDL_GL_SwapWindow(window);
    // SDL_UpdateWindowSurface(window);
};

void GFX_render_texture_part(
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

void GFX_draw_scene(
    scene_t* scene 
) {
    for (int tile=0; tile<scene->n_tile; tile++) {
        GFX_render_texture_part(
            tilesets_library[scene->tile_layer[tile].id],
            scene->tile_layer[tile].render.x1,
            scene->tile_layer[tile].render.y1,
            scene->tile_layer[tile].render.x2,
            scene->tile_layer[tile].render.y2,
            scene->tile_layer[tile].clip.x1,
            scene->tile_layer[tile].clip.y1,
            scene->tile_layer[tile].clip.x2,
            scene->tile_layer[tile].clip.y2,
            false
        );
    }

    for (int sprite=0; sprite<scene->n_sprite; sprite++) {
        GFX_render_texture_part(
            sprites_library[scene->sprite_layer[sprite].id],
            scene->sprite_layer[sprite].render.x1,
            scene->sprite_layer[sprite].render.y1,
            scene->sprite_layer[sprite].render.x2,
            scene->sprite_layer[sprite].render.y2,
            scene->sprite_layer[sprite].clip.x1,
            scene->sprite_layer[sprite].clip.y1,
            scene->sprite_layer[sprite].clip.x2,
            scene->sprite_layer[sprite].clip.y2,
            scene->sprite_layer[sprite].flip_w
        );
    }
}
