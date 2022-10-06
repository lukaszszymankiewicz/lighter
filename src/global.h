#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#ifndef GLOBAL_H
#define GLOBAL_H

// version
#define MAJOR = 0
#define MINOR = 1
#define PATCH = 3

// macros
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define SIGN(a) ((a) > 0 ? (1) : (-1))
#define TRIM(low, high, number) MIN(high, MAX(low, number))

// constans
#define DEFAULT_SCREEN_WIDTH     1366               // default screen resolution
#define DEFAULT_SCREEN_HEIGHT    768                // default screen resolution
#define SCREEN_FPS               20                 // Frame per second
#define SCREEN_TICKS_PER_FRAME   1000 / SCREEN_FPS  // ticks per frame in ms
#define PI                       3.1415             // best approximation of pi known to mankind
#define LEFT_RAD                 1.193547           // angle on which hero looks left
#define RIGHT_RAD                PI/2 *3            // angle on which hero looks right
#define DEG30                    0.523583333        // shortcut
#define DEG90                    PI/2               // shortcut
#define DEG45                    PI/4               // shortcut
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
#define DEFAULT_DARK_A           0                  // default darkness ALPHA channel
#define BACKGROUND_DARK_A        69                 // default background darkness ALPHA channel
#define DEFAULT_LIGHT_R          255                // default light R color
#define DEFAULT_LIGHT_G          252                // default light G color
#define DEFAULT_LIGHT_B          187                // default light B color
#define DEFAULT_LIGHT_A          10                 // default light ALPHA channel
#define GAME_NAME                "Lighter"          // game windom nape
#define MAX_LINE_LENGHT          256                // max line length when reading level data
#define MAX_INT_LEN              5                  // max int length when reading level data
#define TILE_HEIGHT              32                 // single tile height (in px)
#define TILE_WIDTH               32                 // single tile width (in px)
#define TILE_DATA_NUM            3
#define N_COLOR_CHANNELS         4                  // number of color channels for each pixel
#define EMPTY_TILE               0                  // index of empty tile in default tileset
#define BLANK_COLOR              0x00000000         // this color is fully transparent

// yeah... full screen overall pix number is apparently higher than it should be from simple
// multiplication. This is propably due to reshaping resolution size to window size
#define FULL_SCREEN_PIX_SIZE     SCREEN_WIDTH * (SCREEN_HEIGHT+40)
#define FULL_SCREEN_BYTE_SIZE    SCREEN_WIDTH * SCREEN_HEIGHT * N_COLOR_CHANNELS
#define PIX_PER_SCREEN_ROW       SCREEN_WIDTH * N_COLOR_CHANNELS

#endif
