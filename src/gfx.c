#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <SDL2/SDL_video.h>

#include <math.h>
#include <stdio.h>

#include "assets.h"
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

texture_t    *sprites[ASSET_SPRITE_ALL];

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

void GFX_dealloc_buffers() {
    lightbuffer  = NULL;
    shadowbuffer = NULL;
}

void GFX_clean_buffers() {
    memset(lightbuffer, BLANK_COLOR, FULL_SCREEN_BYTE_SIZE);
    memset(shadowbuffer, BLANK_COLOR, FULL_SCREEN_BYTE_SIZE);
}

void GFX_alloc_buffers() {
    lightbuffer     = NULL;
    shadowbuffer = NULL;

    lightbuffer     = (uint32_t*)malloc(FULL_SCREEN_PIX_SIZE * sizeof(uint32_t));
    shadowbuffer = (uint32_t*)malloc(FULL_SCREEN_PIX_SIZE * sizeof(uint32_t));
}

void GFX_free_buffers() {
    free(lightbuffer);
    free(shadowbuffer);

    lightbuffer  = NULL;
    shadowbuffer = NULL;
}

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
    GFX_free_buffers();
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


// render part of the texture to the screen
// render_x1 and render_y1 are refering to global screen resolution (320x240) placement
// 'bool' argument will flip texture vertically
void GFX_render_texture_part(
    texture_t *texture,
    int        render_x1,
    int        render_y1,
    int        tex_x1, // position on texture
    int        tex_y1, // position on texture
    int        tex_x2, // position on texture
    int        tex_y2, // position on texture
    bool       flip
) {
    // printf("binded texture id: %d \n", texture->id);
    // printf("texture name: %s \n", texture->filepath);
    glBindTexture(GL_TEXTURE_2D, texture->id);

    // texture is always rendered 1:1 to achieve pixel perfect effect
    float texW = (float)TXTR_width(texture);
    float texH = (float)TXTR_height(texture);

    // orient it in a way that OpenGL will digest it
    render_x1 -= (float)SCREEN_WIDTH / 2.0;
    render_y1 -= (float)SCREEN_HEIGHT / 2.0;

    render_y1 *= -1;

    float render_y2 = (float)render_y1 - (float)abs(tex_y2 - tex_y1);
    float render_x2 = (float)render_x1 + (float)abs(tex_x2 - tex_x1);

    // printf("tex on tile: %f, %f |  %f, %f | %f %f | %f %f \n",
    //     (float)tex_x2/(float)texW,
    //     (float)tex_y2/(float)texH,
    //     (float)tex_x1/(float)texW,
    //     (float)tex_y2/(float)texH,
    //     (float)tex_x1/(float)texW,
    //     (float)tex_y1/(float)texH,
    //     (float)tex_x2/(float)texW,
    //     (float)tex_y1/(float)texH
    // );

    // printf("render on pix %f, %f | %f, %f \n",
    //     (float)render_x1 + 160,
    //     (float)render_y1 + 120,
    //     (float)render_x2 + 160,
    //     (float)render_y2 + 120
    // );

    // printf("render on %f, %f | %f, %f | %f %f | %f %f \n",
    //     global_x_scale * (float)render_x2/(float)SCREEN_WIDTH,
    //     global_y_scale * (float)render_y2/(float)SCREEN_HEIGHT,
    //     global_x_scale * (float)render_x1/(float)SCREEN_WIDTH,
    //     global_y_scale * (float)render_y2/(float)SCREEN_HEIGHT,
    //     global_x_scale * (float)render_x1/(float)SCREEN_WIDTH,
    //     global_y_scale * (float)render_y1/(float)SCREEN_HEIGHT,
    //     global_x_scale * (float)render_x2/(float)SCREEN_WIDTH,
    //     global_y_scale * (float)render_y1/(float)SCREEN_HEIGHT
    // );
    
    float tex_x1_good = (float)tex_x1/(float)texW;
    float tex_x2_good = (float)tex_x2/(float)texW;

    float tex_y1_good = (float)tex_y1/(float)texH;
    float tex_y2_good = (float)tex_y2/(float)texH;

    float temp;

    if (flip) {
        temp = tex_x1_good; 
        tex_x1_good = tex_x2_good;
        tex_x2_good = temp;
    }

    glBegin(GL_QUADS);
        // up right (1,1)
        glTexCoord2f(tex_x2_good, tex_y1_good);
        glVertex2f(
            global_x_scale * (float)render_x2/(float)SCREEN_WIDTH,
            global_y_scale * (float)render_y1/(float)(SCREEN_HEIGHT)
        );
        // up left (-1, 1)
        glTexCoord2f(tex_x1_good, tex_y1_good);
        glVertex2f(
            global_x_scale * (float)render_x1/(float)SCREEN_WIDTH,
            global_y_scale * (float)render_y1/(float)SCREEN_HEIGHT
        );
        // down left (-1, -1)
        glTexCoord2f(tex_x1_good, tex_y2_good);
        glVertex2f(
            global_x_scale * (float)render_x1/(float)SCREEN_WIDTH,
            global_y_scale * (float)render_y2/(float)SCREEN_HEIGHT
        );
        // down right (1, -1)
        glTexCoord2f(tex_x2_good, tex_y2_good); 
        glVertex2f(
            global_x_scale * (float)render_x2/(float)SCREEN_WIDTH,
            global_y_scale * (float)render_y2/(float)SCREEN_HEIGHT
        );
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

// simplest line - only for debugging
void GFX_draw_colored_line(
    int x1,
    int y1,
    int x2,
    int y2,
    int r,
    int g,
    int b,
    int a
) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);     
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
};

void GFX_draw_rect_border(
    int x,
    int y,
    int w,
    int h,
    int r,
    int g,
    int b,
    int a
) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);     
    SDL_Rect rect = (SDL_Rect){x, y, w, h};
    SDL_RenderFillRect(renderer, &rect);
};

/*
 * THIS SECONDTION WILL BE REPLACED BY OPENGL VERSION
 */
// fills rect with shader function
void GFX_fill_rect(
    shader_t shader,
    int x,
    int y,
    int w,
    int h,
    int r,
    int g,
    int b,
    int power 
) {
    uint32_t red_color = r << 24;
    uint32_t green_color = g << 16;
    uint32_t blue_color = b << 8;
    uint32_t light_color = red_color | green_color | blue_color;

    for (int xx=x; xx<x+w; xx++) { 
        for(int yy=y; yy<y+h; yy++) {
            shader(light_color, xx, yy, power, 0, 0);
        }
    }
};

void GFX_draw_line_to_buffer(
    int       x1,                                       // starting point
    int       x2,                                       // end point
    shader_t  shader,
    int       y,                                        // y-axis coef
    int       r,                                        // red color (from 00 to FF)
    int       g,                                        // green color (from 00 to FF)
    int       b,                                        // blue color (from 00 to FF)
    int       power,                                    // power of  color (mostly alpha channel)
    int       x0,
    int       y0
) {

    uint32_t red_color = r << 24;
    uint32_t green_color = g << 16;
    uint32_t blue_color = b << 8;
    uint32_t light_color = red_color | green_color | blue_color;

    for (int x=MAX(x1, 0); x<MIN(SCREEN_WIDTH, x2); x++) {
        shader(light_color, x, y, power, x0, y0);
    }
};

sorted_list_t* GFX_calc_intersections_in_scanline(
    segment_t *segments,
    int        y,
    int       *n
) {
    sorted_list_t *intersections = NULL;
    segment_t        *ptr        = NULL;
    int               x;

    ptr = segments;

    while(ptr){
        // line in perpendicular to y-axis
        if (ptr->y1 == ptr->y2){
            ptr=ptr->next;
        }
        // line in perpendicular to x-axis
        else if (ptr->x1 == ptr->x2) {
            SRTLST_insert(&intersections, ptr->x1,0 );
            ptr=ptr->next;
            (*n)++;
        }
        else {
            x = GEO_x_intersection_with_slope(y, ptr->x1, ptr->y1, ptr->slope);
            SRTLST_insert(&intersections, x, 0);
            ptr=ptr->next;
            (*n)++;
        }
    }

    return intersections;
}

void GFX_draw_scanline(
    segment_t     *segments,
    shader_t       shader,
    int            y,
    int            r,
    int            g,
    int            b,
    int            power,
    int            x0,
    int            y0
) {
    int            n      = 0;
    sorted_list_t *intscs = NULL;
    intscs                = GFX_calc_intersections_in_scanline(segments, y, &n);

    if (intscs) {

        sorted_list_t* ptr = NULL;

        if (n==2) {
            GFX_draw_line_to_buffer(intscs->value, intscs->next->value, shader, y, r, g, b, power, x0, y0);
        }

        else if (n>2) {
            ptr = intscs;

            while (ptr->next) {
                GFX_draw_line_to_buffer(ptr->value, ptr->next->value, shader, y, r, g, b, power, x0, y0);

                ptr=ptr->next;
                if (ptr->next == NULL) {
                    break;
                }
                ptr=ptr->next;
            }
        }

        SRTLST_free(intscs);
    }
}

// Fill texture (lightbuffer) with polygon (being geometric shape where light is present and should
// be drawn). Polygon is expressed as linked list of vertices, scanline algorithm is used to
// transpose those vertices to filled polygon.
// Function is secured from drawing outside the screen, as such behavior is unsave and wastes
// resources.
void GFX_fill_light(
    shader_t shader,
    vertex_t     *poly,
    int           r,
    int           g,
    int           b,
    int           power,
    int           x0,
    int           y0
) {
    int         y            = 0;
    int         highest      = VRTX_highest_y(poly);
    int         lowest       = VRTX_lowest_y(poly);
    
    segment_t *not_drawn_yet = NULL;
    segment_t *current_draw  = NULL;
    segment_t *obstacle_ptr  = NULL;
    segment_t *candidates    = NULL;

    not_drawn_yet = SEG_get_segments_of_polygon(poly); 

    // algorithm is starting from highest vertex of polygon...
    y = highest;

    // up to the lowest vertex of polygon
    while(y<lowest) {

        // if algorithm reaches point below screen, no further drawing is needed, algorithm is
        // terminated
        if (y>SCREEN_WIDTH) {
            y++;
            break;
        }

        // delete segments from current drawn scan_y must be higher than y of any point
        SEG_delete(&current_draw, y);

        // get candidates to draw
        candidates = SEG_find_candidates(&not_drawn_yet, y);

        // add candidates to current draw
        SEG_merge(&current_draw, candidates);       

        SEG_free(candidates);
        candidates = NULL;

        // if there isn`t anything to draw or anything to be drawn in future - stop
        if (!not_drawn_yet && !current_draw) { break; }

        // if current scanline y is below 0 (outside the screen, no drawing is needed, go to next
        // scanline)
        if (y<0) {
            y++;
            continue; 
        }

        else { 
            GFX_draw_scanline(current_draw, shader, y, r, g, b, power, x0, y0);
            y++;
        }
    }

    SEG_free(not_drawn_yet);
    not_drawn_yet = NULL;

    if (current_draw) { SEG_free(current_draw); }
    if (obstacle_ptr) { SEG_free(obstacle_ptr); }
    if (candidates)  { SEG_free(candidates); }
}

// TODO: this should be deleted later on!
void GFX_draw_light(
) {
    SDL_SetTextureBlendMode(screen_texture, SDL_BLENDMODE_BLEND);
    SDL_UpdateTexture(screen_texture, NULL, lightbuffer, PIX_PER_SCREEN_ROW);
    SDL_RenderCopy(renderer, screen_texture, NULL, NULL);
}

// TODO: this should be deleted later on!
void GFX_draw_darkness(
) {
    SDL_SetTextureBlendMode(screen_texture, SDL_BLENDMODE_MOD);
    SDL_UpdateTexture(screen_texture, NULL, shadowbuffer, PIX_PER_SCREEN_ROW);
    SDL_RenderCopy(renderer, screen_texture, NULL, NULL);
}
