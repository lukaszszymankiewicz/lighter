#include "components.h"

// maybe the window also shouldb e there?
// TODO: introduce window_t struct holding all window needed data
controller_t     *keyboard;
entity_manager_t *entity_manager;
game_timer_t     *cap_timer;
game_timer_t     *fps_timer;
level_manager_t  *level_manager;
scene_t          *scene;

void COMPONENTS_init(
) {
    CON_init();
    ENTMAN_init();
    LVLMAN_init();
    TIMER_init_fps_timer();
    TIMER_init_cap_timer();
    SCENE_init();
}

