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
#define SCREEN_FPS                   20                 // Frame per second
#define SCREEN_TICKS_PER_FRAME       1000 / SCREEN_FPS  // ticks per frame in ms
                                                        //
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
                                                        //
#define SCREEN_WIDTH                 320                // screen width in pixels
#define SCREEN_HEIGHT                240                // screen height in pixels
#define GAME_NAME                    "Lighter"          // game windom nape
                                                        //
// TODO: move it accrdingly
#define TILE_HEIGHT                  32                 // single tile height (in px)
#define TILE_WIDTH                   32                 // single tile width (in px)

// TODO: move it approprialty
#define MAX_LIGHT_ON_SCENE           20                 // maximum number of rendered polygon
#define MAX_ENTITY                   32
#define ENTITY_DRAW_X_RANGE          SCREEN_WIDTH / 2 
#define ENTITY_DRAW_Y_RANGE          SCREEN_HEIGHT / 2
#define ENTITY_UPDATE_DRAW_X_RANGE   (SCREEN_WIDTH / 2) + 32
#define ENTITY_UPDATE_DRAW_Y_RANGE   (SCREEN_HEIGHT / 2) + 32
#define ENTITY_UPDATE_X_RANGE        SCREEN_WIDTH + 32
#define ENTITY_UPDATE_Y_RANGE        SCREEN_HEIGHT + 32
#define ENTITY_UPDATE_LIGHT_X_RANGE  SCREEN_WIDTH * 2
#define ENTITY_UPDATE_LIGHT_Y_RANGE  SCREEN_HEIGHT * 2
#define COLLISION_UPDATE_X_RANGE     100
#define COLLISION_UPDATE_Y_RANGE     100

#define Y_DIRECTION                  -1

#define COLOR_COEF               256.0      // default light R color
#define DEFAULT_LIGHT_R          238        // default light R color
#define DEFAULT_LIGHT_G          220        // default light G color
#define DEFAULT_LIGHT_B          91         // default light B color
#define DEFAULT_LIGHT_A          10         // default light ALPHA channel

// TODO: move it accrdingly
// LEVEL CONTANTS
#define MAX_LEVEL_X_SIZE      20
#define MAX_LEVEL_Y_SIZE      20
#define MAX_LEVEL_ENTITY_FILL 10
#define MAX_LEVEL_SIZE        MAX_LEVEL_X_SIZE * MAX_LEVEL_Y_SIZE

enum direction { RIGHT, LEFT, UP, DOWN, NONE };
// TODO: move it accrdingly
enum state { STANDING, WALKING, JUMPING, FALLING_DOWN, NOTHING, MAX_STATE };
// TODO: move it accrdingly
enum segtype { HOR, VER, UNKNOWN };
// TODO: move it accrdingly

enum layers { LAYER_LIGHT, LAYER_TILE, LAYER_SPRITE, SCALED_IMAGE, LAYER_ALL };

enum buffers { DEFAULT_FRAMEBUFFER, FIRST_BUFFER };

#endif
