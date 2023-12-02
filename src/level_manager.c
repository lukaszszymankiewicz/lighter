#include "components.h"
#include "level_manager.h"
#include "level.h"

#include "data/library.h"

void LVLMAN_init(
) {
    level_manager        = NULL;
    level_manager        = (level_manager_t*)malloc(sizeof(level_manager_t));

    level_manager->level = NULL;
    level_manager->id    = -1;
}

void LVLMAN_fill_entities(
) {
    int n_fills = LVL_n_entity_fills(level_manager->level);

    for (int i=0; i<n_fills; i++) {
        int blueprint_id = level_manager->level->blueprint_id;
        int x            = levels_library[blueprint_id]->entities[i].x;
        int y            = levels_library[blueprint_id]->entities[i].y;
        int id           = levels_library[blueprint_id]->entities[i].id;

        ENTMAN_add_entity(x, y, id);
    }
}

void LVLMAN_fill(
    int id
) {
    level_manager->id    = id;
    level_manager->level = LVL_new(id);

    LVLMAN_fill_entities();
    LVL_fill(level_manager->level);
    LVL_analyze(level_manager->level);
}

void LVLMAN_put_to_scene(
) {
    LVL_put_to_scene(level_manager->level);
}

void LVLMAN_free(
) {
    LVL_free(level_manager->level);
}

