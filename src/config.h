#ifndef CONFIG_H
#define CONFIG_H

#define DEBUG                    1                  // set to zero to get rid of all DEBUG graphics
#define SCREEN_FPS               20                 // Frame per second
#define SCREEN_TICKS_PER_FRAME   1000 / SCREEN_FPS  // ticks per frame in ms
#define PI                       3.1415             // best approximation of pi known to mankind
#define LEFT_RAD                 PI/2               // angle on which hero looks left
#define RIGHT_RAD                (3 * PI) / 2       // angle on which hero looks right
#define DEG30                    0.523583333        // shortcut
#define DEG15                    PI/12              // shortcut
#define DEG10                    PI/18              // shortcut
#define DEG5                     PI/36              // shortcut
#define DEG1                     PI/(36*5)          // shortcut
#define SCREEN_WIDTH             320                // screen width in pixels
#define SCREEN_HEIGHT            200                // screen height in pixels
#define MAX_KEYS                 SDL_NUM_SCANCODES  // SDL max keys supported

#define DEFAULT_DARK_R           0                  // default darkness R color
#define DEFAULT_DARK_G           0                  // default darkness G color
#define DEFAULT_DARK_B           0                  // default darkness B color
#define DEFAULT_DARK_A           42                 // default darkness ALPHA channel

#define DEFAULT_LIGHT_R          255                // default light R color
#define DEFAULT_LIGHT_G          255                // default light G color
#define DEFAULT_LIGHT_B          255                // default light B color
#define DEFAULT_LIGHT_A          20                 // default light ALPHA channel

#define GAME_NAME                "Lighter"          // game windom nape

#endif
