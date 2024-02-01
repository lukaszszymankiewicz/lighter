#ifndef GLOBAL_H
#define GLOBAL_H

// version
#define MAJOR = 1
#define MINOR = 0
#define PATCH = 0

// macros
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define SIGN(a) ((a) > 0 ? (1) : (-1))

// constans
#define GAME_NAME                    "Lighter"          // game windom nape
#define PI                           3.1415             // best approximation of pi known to mankind
#define SCREEN_FPS                   20                 // Frame per second
#define SCREEN_HEIGHT                240                // screen height in pixels
#define SCREEN_TICKS_PER_FRAME       1000 / SCREEN_FPS  // ticks per frame in ms
#define SCREEN_WIDTH                 320                // screen width in pixels

#endif
