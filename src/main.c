#include "game.h"

int graphic_option    = GRAPHIC_ON;
int defult_level      = 0;
int defult_max_frames = -1;

const char* GRAPHIC_OFF_ARG = "gfx_off";
const char* RUN_ONE_FRAME   = "one_frame";

int main(
    int argc,
    char* args[]
) {
    if ((argc > 1) && (strcmp(args[1], GRAPHIC_OFF_ARG) == 0)) {
        graphic_option = 0;
    }

    if ((argc > 2) && (strcmp(args[2], RUN_ONE_FRAME) == 0)) {
        defult_max_frames = 1;
    }

    GAME_run(defult_level, graphic_option, defult_max_frames);
}
