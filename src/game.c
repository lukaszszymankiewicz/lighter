#include "global.h"
#include "files.h"
#include "controller.h"
#include "entity_manager.h"
#include "game.h"
#include "gfx.h"
#include "timer.h"
#include "source.h"
#include "level.h"
#include "import.h"
#include "light.h"
#include "sprites.h"

static char buffer[BUFFER_SIZE];

void GAME_read_all_files(
) {
    animations[ASSET_HERO_ANIMATION]           = ANIM_read_animation(FILEPATH_HERO_ANIMATION);
    animations[ASSET_NO_ANIMATION]             = NULL;
    sprites[ASSET_SPRITE_HERO]                 = GFX_read_texture(FILEPATH_SPRITE_HERO);
    sprites[ASSET_SPRITE_TEST]                 = GFX_read_texture(FILEPATH_SPRITE_HERO);
    sprites[ASSET_SPRITE_LIGHTER]              = GFX_read_texture(FILEPATH_SPRITE_LIGHTER);
    sprites[ASSET_SPRITE_WALLLIGHT]            = GFX_read_texture(FILEPATH_SPRITE_WALLLIGHT);
    wobbles[ASSET_WOBBLE_NO]                   = SRC_read_wobble(FILEPATH_WOBBLE_NO);
    wobbles[ASSET_WOBBLE_STABLE]               = SRC_read_wobble(FILEPATH_WOBBLE_STABLE);
    wobbles[ASSET_WOBBLE_WALKING]              = SRC_read_wobble(FILEPATH_WOBBLE_WALKING);
    lightsources[ASSET_LIGHTSOURCE_LIGHTER]    = SRC_read_lightsource(FILEPATH_LIGTHER_LIGHTSOURCE);
    lightsources[ASSET_LIGHTSOURCE_WALLLIGHT]  = SRC_read_lightsource(FILEPATH_WALLLIGHT_LIGHTSOURCE);
}

void GAME_update_all_files(
) {
    // TODO: it is ugly to have it here - propably some more clever place for such initialization is
    // needed
    SRC_set_wobble(lightsources[ASSET_LIGHTSOURCE_LIGHTER], wobbles[ASSET_WOBBLE_STABLE], 0);
    SRC_set_wobble(lightsources[ASSET_LIGHTSOURCE_LIGHTER], wobbles[ASSET_WOBBLE_WALKING], 1);
}

void GAME_free_all_files(
) {
    ANIM_free(animations[ASSET_HERO_ANIMATION]);
    GFX_free_texture(sprites[ASSET_SPRITE_HERO]);
    SRC_free_wobble(wobbles[ASSET_WOBBLE_NO]);
    SRC_free_wobble(wobbles[ASSET_WOBBLE_STABLE]);
    SRC_free_wobble(wobbles[ASSET_WOBBLE_WALKING]);
    SRC_free_lightsource(lightsources[ASSET_LIGHTSOURCE_LIGHTER]);
    SRC_free_lightsource(lightsources[ASSET_LIGHTSOURCE_WALLLIGHT]);
}

void GAME_handle_SDL_events(
    game_t *game
) {
  while (SDL_PollEvent(&(game->event)) != 0) {
    if (game->event.type == SDL_QUIT) { 
        game->loop = false; 
        }
    }
}

void GAME_update_events(
    game_t *game
) {
    GAME_handle_SDL_events(game);
    CON_update(keyboard);
}

void GAME_close(
    game_t *game
) {
    CON_free(keyboard);
    TIMER_free(game->cap_timer);
    TIMER_free(game->fps_timer);
    ENTMAN_free(game->entity_manager);
    LVL_free(game->level);
    GAME_free_all_files();
    free(game);
    GFX_free();
    SDL_Quit();
};

void GAME_fill_level(
    game_t* game,
    FILE*   file
) {
    int tiles_counter = 0;
    int layer_counter = 0;
    int layer_read    = 0;
    int cur_tile_idx  = 0;
    int cur_tile_id   = 0;
    int state         = 0;
    int coords_per_single_tile_per_layer;
    int coords_per_single_tile;
    int x_size, y_size;
    int x_tile, y_tile;

    state++;

    // TODO (LG-15): all comments should will part of "DEBUG READ LEVEL" mode
    // FILE *f = fopen("level_read_log.txt", "w");

    while((fread(buffer, DOUBLE_BYTE, PROPER_PACK_COUNT, file) == PROPER_PACK_COUNT)) {
      int dec_value = IMP_cast_val_to_dec(buffer);

      switch (state) {

        case READ_LEVEL_PREAMBULE_FIRST_HALF:
            if(dec_value != LEVEL_PREAMBULE[FIRST_HALF]) { return; }
            state++; break;

        case READ_LEVEL_PREAMBULE_SECOND_HALF:
            if(dec_value != LEVEL_PREAMBULE[SECOND_HALF]) { return; }
            state++; break;

        case READ_LEVEL_X_SIZE:
            x_size = dec_value;
            state++; break;

        case READ_LEVEL_Y_SIZE:
            y_size = dec_value;
            LVL_set_size(game->level, x_size, y_size);
            state++; break;

        case READ_LEVEL_TILES_SUM:
            // TODO(LG-15): all comments should will part of "DEBUG READ LEVEL" mode
            // fprintf(f, "detected tile per layer %d, n=%d \n", layer_read, (int)dec_value/2);
            coords_per_single_tile_per_layer = dec_value;
            state++;
            break;

        case READ_LEVEL_TILES_ID:
            cur_tile_id = dec_value;
            // TODO (LG-15): all comments should will part of "DEBUG READ LEVEL" mode
            // fprintf(f, "detected tile with index id=%d\n", cur_tile_id);
            state++;
            break;

        case READ_LEVEL_TILE_SUM:
            // TODO (LG-15): all comments should will part of "DEBUG READ LEVEL" mode
            // fprintf(f, "    n=%d per single tile\n", (int)dec_value/2);
            coords_per_single_tile = dec_value;
            state++;
            break;

        case READ_LEVEL_TILE_X:
            x_tile = dec_value;
            tiles_counter++;
            layer_counter++;
            state++;
            break;

        case READ_LEVEL_TILE_Y:
            y_tile = dec_value;
            tiles_counter++;
            layer_counter++;
            
            // both coords are read - Tile is ready to be placed somewhere on level
            if (layer_read == 0) {
                // TODO (LG-15): all comments should will part of "DEBUG READ LEVEL" mode
                // fprintf(f, "        tile put on x=%d y=%d \n",  x_tile, y_tile);
                LVL_fill_structure(game->level, x_tile, y_tile, cur_tile_idx);
            } else if (layer_read == 1) {
                // TODO (LG-15): this is temporary - it will be valid only for one type of obstacle
                // this 1 is means obstacle type
                // TODO (LG-15): all comments should will part of "DEBUG READ LEVEL" mode
                // fprintf(f, "        obstacle with index: 1 put on %d %d \n", x_tile, y_tile);
                LVL_fill_obstacle(game->level, x_tile, y_tile, 1);

            } else {
                // TODO (LG-15): all comments should will part of "DEBUG READ LEVEL" mode
                // fprintf(f, "        entity with index: %d put on %d %d on \n", cur_tile_id, x_tile, y_tile);
                ENTMAN_add_entity(game->entity_manager, x_tile, y_tile, cur_tile_id);
            }

            if (layer_counter == coords_per_single_tile_per_layer) {
                // current tile on this layer is done - proceed to next tile
                layer_read++;

                if (layer_read == 3) {
                    // eveything is read - move on 
                    state = READ_LEVEL_ALL_TILES_READ;
                    break;
                }

                state = READ_LEVEL_TILES_SUM;
                layer_counter = 0;
                cur_tile_idx  = 0;
                cur_tile_id   = 0;
                tiles_counter = 0;
                break;
            } else if (tiles_counter == coords_per_single_tile) {
                cur_tile_idx++;
                state = READ_LEVEL_TILES_ID;
                tiles_counter = 0;
                break;
            // current tile is not yet read
            } else {
                state = READ_LEVEL_TILE_X;
                break;
            }
        case READ_LEVEL_ALL_TILES_READ:
            break;
      }
    }
    // TODO (LG-15): all comments should will part of "DEBUG READ LEVEL" mode
    // fclose(f);
}

void GAME_read_level_tileset(
    game_t*    game,
    texture_t* tileset
) {
    game->level->tileset = tileset;
}

void GAME_read_level(
    game_t*     game,
    const char* filename
) {
    FILE *file         = NULL;
    char *data_path    = NULL;
    char *img_path     = NULL;
    texture_t *tileset = NULL;

    data_path = IMP_concatenate_string(filename, SEPARATOR, LEVEL_STRUCTURE_SUFFIX);
    img_path  = IMP_concatenate_string(filename, SEPARATOR, LEVEL_TILESET_SUFFIX);

    file    = fopen(data_path, LEVEL_READ_MODE);
    tileset = GFX_read_texture(img_path);

    GAME_read_level_tileset(game, tileset);
    LVL_fill_tiles(game->level);
    GAME_fill_level(game, file);

    LVL_analyze(game->level);

    free(data_path);
    free(img_path);
}

game_t* GAME_init(
) {
    game_t* game         = NULL;
    game                 = (game_t*)malloc(sizeof(game_t));

    keyboard             = CON_init();

    game->loop           = true;
    game->entity_manager = ENTMAN_new();

    game->level          = NULL;
    game->level          = LVL_new();
    GAME_read_level(game, FILEPATH_LEVEL_SAMPLE);

    game->frame          = 0;
    game->fps_timer      = TIMER_new();
    game->cap_timer      = TIMER_new();

    TIMER_start(game->fps_timer);

    return game;
};

void GAME_update_ticks(
    game_t* game
) {
    game->frame_ticks = TIMER_get_ticks(game->cap_timer);
}

void GAME_update_frame(
    game_t* game
) {
    if(game->frame_ticks < SCREEN_TICKS_PER_FRAME) {
        game->frame = (game->frame + 1);
    }
}

void GAME_delay_frame(
    game_t* game
) {
    if(game->frame_ticks < SCREEN_TICKS_PER_FRAME) {
        SDL_Delay(SCREEN_TICKS_PER_FRAME - game->frame_ticks);
    }
}

void GAME_start_cap_time(
    game_t* game
) {
    TIMER_start(game->cap_timer);
}

void GAME_clear_screen(
    game_t *game
) {
    GFX_clear_screen();
    GFX_clean_buffers();
}

void GAME_update_entities(
    game_t *game
) {
    ENTMAT_update(game->entity_manager, game->level->obstacle_segments);
}

void GAME_draw_level(
    game_t *game
) {
    LVL_draw(game->level, ENTMAN_hero_x(game->entity_manager), ENTMAN_hero_y(game->entity_manager));
}

void GAME_draw_entities(
    game_t *game
) { 
    ENTMAN_draw_entities(game->entity_manager);
}

void GAME_render(
    game_t *game
) {
    GFX_update();
}

void GAME_init_graphics(
) {
    GFX_init_graphics();
}

void GAME_calc_light(
    game_t        *game,
    light_scene_t *scene
) {
    ENTMAN_calc_light(game->entity_manager, scene, game->level->obstacle_segments);
}

void GAME_draw_light(
    game_t* game
) {
    LIG_clean_light();

    light_scene_t* scene = NULL; 
    scene                = LIG_new_light_scene();

    ENTMAN_calc_light(game->entity_manager, scene, game->level->obstacle_segments);

    LIG_compose_light_scene(scene);
    LIG_draw_light_scene(scene);
    LIG_free_light_scene(scene);
}

game_t* GAME_new(
) {
    game_t* game = NULL;

    GAME_init_graphics();
    GAME_read_all_files();
    GAME_update_all_files();

    game         = GAME_init();

    return game;
}

void GAME_start_time(
    game_t* game
) {
    GAME_start_cap_time(game);
}

void GAME_apply_logic(
    game_t* game
) {
    GAME_update_events(game);
    GAME_update_entities(game);
}

void GAME_draw(
    game_t* game
) {
    GAME_clear_screen(game);
    GAME_draw_level(game);
    GAME_draw_light(game);
    GAME_draw_entities(game);
    GAME_render(game);
}

void GAME_update_time(
    game_t* game
) {
    GAME_update_ticks(game);
    GAME_update_frame(game);
    GAME_delay_frame(game);
}

void GAME_loop(
    game_t* game
) {
    while(game->loop) {
        GAME_start_time(game);
        GAME_apply_logic(game);
        GAME_draw(game);
        GAME_update_time(game);
    }
}

void GAME_run(
) {
    game_t* game = NULL;
    game         = GAME_new();
    GAME_loop(game);
    GAME_close(game);
}
