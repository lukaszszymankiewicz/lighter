#ifndef GLOBAL_H
#define GLOBAL_H

// version
#define MAJOR = 0
#define MINOR = 2
#define PATCH = 0

// macros
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define SIGN(a) ((a) > 0 ? (1) : (-1))

// constans
#define DEFAULT_SCREEN_WIDTH         1366               // default screen resolution
#define DEFAULT_SCREEN_HEIGHT        768                // default screen resolution
#define SCREEN_FPS                   20                 // Frame per second
#define SCREEN_TICKS_PER_FRAME       1000 / SCREEN_FPS  // ticks per frame in ms
#define PI                           3.1415             // best approximation of pi known to mankind
#define LEFT_RAD                     PI/2               // angle on which hero looks left
#define RIGHT_RAD                    PI/2 *3            // angle on which hero looks right
#define DEG30                        0.523583333        // shortcut
#define DEG90                        PI/2               // shortcut
#define DEG45                        PI/4               // shortcut
#define DEG15                        PI/12              // shortcut
#define DEG10                        PI/18              // shortcut
#define DEG5                         PI/36              // shortcut
#define DEG1                         PI/(36*5)          // shortcut
#define SCREEN_WIDTH                 320                // screen width in pixels
#define SCREEN_HEIGHT                240                // screen height in pixels
#define GAME_NAME                    "Lighter"          // game windom nape
#define TILE_HEIGHT                  32                 // single tile height (in px)
#define TILE_WIDTH                   32                 // single tile width (in px)
#define N_COLOR_CHANNELS             4                  // number of color channels for each pixel
#define EMPTY_TILE                   0                  // index of empty tile in default tileset
#define BLANK_COLOR                  0x00000000         // this color is fully transparent
#define BLACK_COLOR                  0xFFFFFFFF         // this color is blackes black
#define MAX_LIGHT_ON_SCENE           20                 // maximum number of rendered polygon
#define MAX_ENTITY                   32
#define ENTITY_DRAW_X_RANGE          SCREEN_WIDTH / 2 
#define ENTITY_DRAW_Y_RANGE          SCREEN_HEIGHT / 2
#define ENTITY_UPDATE_DRAW_X_RANGE   (SCREEN_WIDTH / 2) + 32
#define ENTITY_UPDATE_DRAW_Y_RANGE   (SCREEN_HEIGHT / 2) + 32
#define COLLISION_UPDATE_X_RANGE     100
#define COLLISION_UPDATE_Y_RANGE     100
#define ENTITY_UPDATE_X_RANGE        SCREEN_WIDTH + 32
#define ENTITY_UPDATE_Y_RANGE        SCREEN_HEIGHT + 32
#define ENTITY_UPDATE_LIGHT_X_RANGE  SCREEN_WIDTH * 2
#define ENTITY_UPDATE_LIGHT_Y_RANGE  SCREEN_HEIGHT * 2
#define SCREEN_TILE_PER_X            10
#define SCREEN_TILE_PER_Y            12

// TODO: after implementing OpenGL it should be deleted
#define FULL_SCREEN_PIX_SIZE     SCREEN_WIDTH * (SCREEN_HEIGHT+40)
#define FULL_SCREEN_BYTE_SIZE    SCREEN_WIDTH * SCREEN_HEIGHT * N_COLOR_CHANNELS
#define PIX_PER_SCREEN_ROW       SCREEN_WIDTH * N_COLOR_CHANNELS

#define CAMERA_X                 (int)SCREEN_WIDTH / 2
#define CAMERA_Y                 (int)SCREEN_HEIGHT / 2

#define DEFAULT_LIGHT_R          255                // default light R color
#define DEFAULT_LIGHT_G          252                // default light G color
#define DEFAULT_LIGHT_B          187                // default light B color
#define DEFAULT_LIGHT_A          10                 // default light ALPHA channel
                                                    //
#endif
