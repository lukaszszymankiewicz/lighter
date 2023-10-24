#include <stdbool.h>

#include "game.h"

#define  graphic_on         true
#define  graphic_off        false
#define  defult_level       0
#define  defult_max_frames -1
#define  GAME_COMPLETE      0

game_config_t default_config = (game_config_t) {
    defult_max_frames,
    graphic_on,
    defult_level      
};

int main(
) {
    GAME_run(default_config);

    return GAME_COMPLETE;
}
