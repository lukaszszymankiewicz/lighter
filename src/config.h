#ifndef CONFIG_H
#define CONFIG_H

#define DEBUG 0                                   // set to zero to get rid of all DEBUG graphics
#define SCREEN_FPS 20
#define SCREEN_TICKS_PER_FRAME 1000 / SCREEN_FPS
#define PI 3.1415                                 // best approximation of pi known to mankind

#define LEFT_RAD PI/2;                            // angle on which hero looks left
#define RIGHT_RAD (3 * PI) / 2;                   // angle on which hero looks right

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

#endif
