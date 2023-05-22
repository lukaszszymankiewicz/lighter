#include <stdbool.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>

#include <math.h>
#include <stdio.h>

#include "global.h"
#include "gfx.h"
#include "texture.h"
#include "primitives.h"
#include "sorted_list.h"
#include "geometry.h"
#include "vertex.h"
#include "segment.h"


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

SDL_GLContext gl_context;

SDL_Window   *window            = NULL;
SDL_Renderer *renderer          = NULL;
SDL_Texture  *screen_texture    = NULL;

uint32_t     *lightbuffer;
uint32_t     *shadowbuffer;

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

/*
 * THIS SECONDTION WILL BE REPLACED BY OPENGL VERSION
 */
float GFX_dist(
    int x0,
    int y0,
    int x1,
    int y1
) {
    return sqrt((x0-x1)*(x0-x1)+(y0-y1)*(y0-y1)) * GRADIENT_COEF;
}

// this is OpenGL version of mix function
float GFX_lerp(
    float v0,
    float v1,
    float t
) {
    return (1 - t) * v0 + t * v1;
}

// this is the only way to use OpenGL without really calling it. OpenGL implementaiton must be done
// in separate ticket
// proper gradient version for GLSL:
// float dist(vec2 p0, vec2 pf) {return sqrt((pf.x-p0.x)*(pf.x-p0.x)+(pf.y-p0.y)*(pf.y-p0.y));}
//
// void mainImage( out vec4 fragColor, in vec2 fragCoord )
// {
//  float d = dist(iResolution.xy*0.5,fragCoord.xy)*0.01;
// 	fragColor = mix(vec4(1.0, 1.0, 1.0, 1.0), vec4(0.0, 0.0, 0.0, 1.0), d);
// }
//
void GFX_fill_lightbuffer(
    uint32_t  color,
    int       x,
    int       y,
    int       power,
    int       x0,
    int       y0
) {
    // TODO (LG-10): this should be a three (?) separate shaders in OpenGL!
    int      shadow = 0;
    int      old_shadow = 0;
    int      pos    = x+y*SCREEN_WIDTH;
    uint32_t dist   = (uint32_t)GFX_dist(x0, y0, x, y);

    old_shadow = (shadowbuffer[pos] & 0xFF000000) >> 24;
    shadow = (int)(GFX_lerp(255, old_shadow, (float)MIN(dist, 255)/255));
    
    if (shadow == 255) {
        lightbuffer[pos] = color | ((lightbuffer[pos] & 0xFF) + power);
    }

    // add shadow with light gradient
    shadowbuffer[pos] = (shadow << 24) | (shadow << 16) | (shadow << 8) | 255; 
}

/*
 * END OPENGL SECTION
 */

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

    if (window == NULL) {
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
    // Create context
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

// every pixel specific graphic will be stored in this texture. Sprites are rendered in normal
// fashion but pixel-sharp shapes needs to be put in the buffer first.
void GFX_init_screen_buffer_texture(
) {
    screen_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        SCREEN_WIDTH,
        SCREEN_HEIGHT
    );
}

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

void GFX_free_texture(
    texture_t *texture
) {
    if (texture && texture->surface) {
        SDL_FreeSurface(texture->surface);
        texture->surface    = NULL;
        GLuint TextureID    = texture->id;
        glDeleteTextures(1, &TextureID);
        free(texture);
        texture = NULL;
    }
};

void GFX_free() {
    SDL_DestroyTexture(screen_texture);

	// Deallocate program
	glDeleteProgram(opengl_program_id);

    if (renderer) {
        SDL_DestroyRenderer(renderer);
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


// Render part of the texture to the screen. render and tex parameters are expressed in OpenGL
// orientation units, but in "global" orientation (not taking camera position). 'bool' argument will
// flip texture vertically
void GFX_render_texture_part(
    texture_t *texture,
    float      render_x1,   // place on screen (OpenGL coords)
    float      render_y1,   // place on screen (OpenGL coords)
    float      render_x2,   // place on screen (OpenGL coords)
    float      render_y2,   // place on screen (OpenGL coords)
    float      tex_x1,      // position on texture (OpenGL coords)
    float      tex_y1,      // position on texture (OpenGL coords)
    float      tex_x2,      // position on texture (OpenGL coords)
    float      tex_y2,      // position on texture (OpenGL coords)
    bool       flip
) {
    // printf("binded texture id: %d \n", texture->id);
    // printf("texture name: %s \n", texture->filepath);
    glBindTexture(GL_TEXTURE_2D, texture->id);

    // texture is always rendered 1:1 to achieve pixel perfect effect
    // float texW = (float)TXTR_width(texture);
    // float texH = (float)TXTR_height(texture);

    // orient it in a way that OpenGL will digest it
    // render_x1 -= (float)SCREEN_WIDTH / 2.0;
    // render_y1 -= (float)SCREEN_HEIGHT / 2.0;

    // render_y1 *= -1;
    // float render_y2 = (float)render_y1 - ((tex_y2 - tex_y1) * texH);
    // float render_x2 = (float)render_x1 + ((tex_x2 - tex_x1) * texW);
    // float render_y2 = (float)render_y1 - (float)TILE_WIDTH;
    // float render_x2 = (float)render_x1 + (float)TILE_HEIGHT;

    float tex_x1_good = (float)tex_x1;
    float tex_x2_good = (float)tex_x2;
    float tex_y1_good = (float)tex_y1;
    float tex_y2_good = (float)tex_y2;

    float temp;

    if (flip) {
        temp = tex_x1_good; 
        tex_x1_good = tex_x2_good;
        tex_x2_good = temp;
    }

    printf("texture : %f, %f |  %f, %f \n",
        tex_x1_good,
        tex_x2_good,
        tex_y1_good,
        tex_y2_good
    );

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
        glTexCoord2f(tex_x2_good, tex_y1_good);
        glVertex2f(render_x2, render_y1);
        // up left (-1, 1)
        glTexCoord2f(tex_x1_good, tex_y1_good);
        glVertex2f(render_x1, render_y1);
        // down left (-1, -1)
        glTexCoord2f(tex_x1_good, tex_y2_good);
        glVertex2f(render_x1, render_y2);
        // down right (1, -1)
        glTexCoord2f(tex_x2_good, tex_y2_good); 
        glVertex2f(render_x2, render_y2);
    glEnd();
};

// renders texture to screen
void GFX_render_texture(
    texture_t *texture,   // Texture to be rendered
    SDL_Rect  *clip,      // rect from texture to be rendered
    int        x,         // x coord of screen to have texture rendered
    int        y,         // y coord of screen to have texture rendered
    bool       flip       // indiaction if texture should be flipped horizontally
) {
    SDL_Rect render_quad = {x, y, texture->surface->w, texture->surface->h};
    SDL_RendererFlip flip_tex;
    
    // if clip is not given render whole texture
    if(clip != NULL) {
        render_quad.w = clip->w;
        render_quad.h = clip->h;
    }

    if (flip) {
        flip_tex = SDL_FLIP_HORIZONTAL; 
    }
    else {
        flip_tex = SDL_FLIP_NONE; 
    }

    // SDL_RenderCopyEx(renderer, texture->surface, clip, &render_quad, 0, NULL, flip_tex);

    free(clip);
};
