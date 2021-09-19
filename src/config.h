#ifndef CONFIG_H
#define CONFIG_H

#define DEBUG                  1                  // set to zero to get rid of all DEBUG graphics
#define SCREEN_FPS             20                 // Frame per second
#define SCREEN_TICKS_PER_FRAME 1000 / SCREEN_FPS  // ticks per frame in ms
#define PI                     3.1415             // best approximation of pi known to mankind
#define LEFT_RAD               PI/2               // angle on which hero looks left
#define RIGHT_RAD              (3 * PI) / 2       // angle on which hero looks right
#define DEG30                  PI/6               // shortcut
#define DEG15                  PI/12              // shortcut
#define SCREEN_WIDTH           320                // screen width in pixels
#define SCREEN_HEIGHT          200                // screen height in pixels
#define MAX_KEYS               SDL_NUM_SCANCODES  // SDL max keys supported

#define DEFAULT_DARK_R         0
#define DEFAULT_DARK_G         0
#define DEFAULT_DARK_B         0
#define DEFAULT_DARK_A         100

#define DEFAULT_LIGHT_R        169
#define DEFAULT_LIGHT_G        169
#define DEFAULT_LIGHT_B        169
#define DEFAULT_LIGHT_A        20

#define GAME_NAME              "Lighter"

typedef short int direction_t;
enum direction { RIGHT, LEFT, UP, DOWN, NONE };

#endif
