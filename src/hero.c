#include "global.h"
#include "geometry.h"
#include "import.h"
#include "hero.h"
#include "gfx.h"
#include "primitives.h"
#include "sorted_list.h"

// do it dynamically!
int lightpos_x_corr[] = {5, 1};
int lightpos_y_corr[] = {11, 11};

int state_collisions[3][3] = {
    //  STANDING, WALKING, JUMPING
    {1,            1,         1    },
    {1,            1,         1    },
    {0,            0,         0    },
};


hero_t* HERO_init(
    int x, int y
) {
    hero_t* hero    = (hero_t*)malloc(sizeof(hero_t));
    hero->x         = x * TILE_WIDTH;        
    hero->y         = y * TILE_HEIGHT;
    hero->view_x    = SCREEN_WIDTH / 2;      // postion on viewport (middle of screen)
    hero->view_y    = 144;                   // postion on viewport (middle of screen)
    hero->state     = STANDING;
    hero->direction = LEFT;

    hero->sprites          = animations[ASSET_HERO_ANIMATION];
    hero->sprites->texture = sprites[ASSET_SPRITE_HERO];

    hero->frame   = 0;
    hero->frame_t = 0;
    hero->x_vel   = 0;
    hero->y_vel   = 0;

    return hero;
}

SDL_Rect* HERO_current_frame(
    hero_t *hero
) {
    return &(hero->sprites->animations[hero->state].frames[hero->frame].rect);
}

void HERO_debug_hitbox(hero_t *hero) {
    int n_boxes = hero->sprites->animations[hero->state].frames[hero->frame].n_hit_box;

    for (int i=0; i<n_boxes; i++) {
        SDL_Rect rect = hero->sprites->animations[hero->state].frames[hero->frame].hit_boxes[i];
        GFX_draw_rect_border(rect.x+hero->view_x, rect.y+hero->view_y, rect.w, rect.h, 255, 0, 0, 255);
    }
}

// gets x coord of light hero is holding
int HERO_light_x(
    hero_t *hero
) {
    return hero->view_x + lightpos_x_corr[hero->direction];
}

// gets y coord of light hero is holding
int HERO_light_y(
    hero_t *hero
) {
    return hero->view_y  + lightpos_y_corr[hero->direction];
}

bool HERO_check_state_collision(
    int current_state,
    int desired_state
) {
    return state_collisions[current_state][desired_state];
}

void HERO_update_friction(
    hero_t *hero
) {
    if (hero->direction == RIGHT) {
        hero->x_vel = MAX(0, hero->x_vel - X_FRICTION);
    }
    else {
        hero->x_vel = MIN(0, hero->x_vel + X_FRICTION);
    }

    if (hero->state == JUMPING || hero->state == FALLING_DOWN) {
        hero->y_vel += Y_FRICTION;
    }
}

void HERO_update_pos_due_to_velocity(
    hero_t *hero
) {
    hero->x += hero->x_vel;
    hero->y += hero->y_vel;
}

void HERO_update_state(
    hero_t *hero
) {
    if (hero->x_vel == 0 && HERO_check_state_collision(hero->state, STANDING)) {
        hero->state=STANDING; 
    }
    if (hero->y_vel > 0) {
        hero->state=FALLING_DOWN; 
    }
}

void HERO_update_sprite(
    hero_t *hero
) {
    hero->frame_t++;
    int del = hero->sprites->animations[hero->state].frames[hero->frame].delay;
    int len = hero->sprites->animations[hero->state].len;

    if (hero->frame_t >= del) {
        hero->frame_t=0;
        hero->frame++; 

        if (hero->frame >= len) {
            hero->frame=0;
        }
    }
}

void HERO_check_collision(
    hero_t *hero,
    segment_t *obstacles
) {
    // TODO: add method for this calculation
    int  n_boxes     = hero->sprites->animations[hero->state].frames[hero->frame].n_hit_box;

    int  x_coef      = 0;
    int  y_coef      = 0;
    int  x_collision = -1;
    int  y_collision = -1;
    int  y_falling   = -1;
    int  new_x;
    int  new_y;

    y_coef = hero->y_vel;
    x_coef = hero->x_vel;

    sorted_list_t *x_intersections = NULL;
    sorted_list_t *y_intersections = NULL;
    sorted_list_t *falling         = NULL;

    // for each of the hit box x, y and falling collision is checked, and each calculated collision
    // value is stored. Then, depending of the direction of a hero lowest or highest value is then
    // used and final postion of hero.
    for (int i=0; i<n_boxes; i++) {
        SDL_Rect rect = hero->sprites->animations[hero->state].frames[hero->frame].hit_boxes[i];

        for (segment_t* obstacle=obstacles; obstacle; obstacle=obstacle->next) {
            if (hero->x_vel != 0) {
                x_collision = GEO_vertical_segment_intersects_rect(
                   obstacle->x1,
                   obstacle->y1,
                   obstacle->x2,
                   obstacle->y2, 
                   rect.x + hero->view_x + x_coef,
                   rect.y + hero->view_y,
                   rect.x + hero->view_x + rect.w + x_coef,
                   rect.y + hero->view_y + rect.h
                );
            }

            y_collision = GEO_horizontal_segment_intersects_rect(
               obstacle->x1,
               obstacle->y1,
               obstacle->x2,
               obstacle->y2, 
               rect.x + hero->view_x,
               rect.y + hero->view_y + y_coef,
               rect.x + hero->view_x + rect.w,
               rect.y + hero->view_y + rect.h + y_coef
            );

            y_falling = GEO_horizontal_segment_intersects_rect(
               obstacle->x1,
               obstacle->y1,
               obstacle->x2,
               obstacle->y2, 
               rect.x + hero->view_x,
               rect.y + hero->view_y + 1,
               rect.x + hero->view_x + rect.w,
               rect.y + hero->view_y + rect.h + 1
            );


            if (x_collision != -1) {
                SRTLST_insert(&x_intersections, x_collision); 
            }
            if (y_collision != -1) { SRTLST_insert(&y_intersections, y_collision); }
            if (y_falling   != -1) { SRTLST_insert(&falling, y_falling  );         }
        }
    }
    
    if (x_intersections != NULL) {

        hero->x_vel = 0;  //stop moving!

        if (hero->direction == LEFT) {
            new_x = SRTLST_get_last(x_intersections);
            hero->x -= hero->view_x - new_x;
        }
        else {
            SDL_Rect *frame = NULL;
            frame = HERO_current_frame(hero);

            new_x = x_intersections->value - frame->w;
            hero->x -= hero->view_x - new_x;
        }
    }

    if (y_intersections != NULL) {
        // moving up
        if (hero->y_vel < 0) { 
            new_y = SRTLST_get_last(y_intersections);
            hero->y += hero->view_y - new_y;
            hero->y_vel = 0;
        }

        // falling down
        else {
            SDL_Rect *frame = HERO_current_frame(hero);
            new_y = y_intersections->value - frame->h;
            hero->y -= hero->view_y - new_y;
            hero->state = STANDING;
            hero->y_vel = 0;
        }
    }

    if (falling == NULL && (hero->state == STANDING || hero->state == WALKING )) {
        // if no obstacles is below hero`s feet failing down begins
        hero->state = FALLING_DOWN;
    }

    free(x_intersections);
    free(y_intersections);
    free(falling);
}


void HERO_move(
    hero_t      *hero,
    direction_t  direction
) {
    if (hero->state == STANDING || hero->state == WALKING) {
        hero->state = WALKING;

        if (hero->direction == RIGHT) {
            hero->x_vel = MIN(MAX_VEL, hero->x_vel + MOVE_POWUH);
        }
        else {
            hero->x_vel = MAX(-1*MAX_VEL, (hero->x_vel - MOVE_POWUH));
        }

        hero->direction = direction;
    }

    else if (hero->state == JUMPING || hero->state == FALLING_DOWN) {
        if (hero->direction == RIGHT) {
            hero->x_vel = MIN(MAX_VEL, hero->x_vel + MOVE_POWUH);
        }
        else {
            hero->x_vel = MAX(-1*MAX_VEL, (hero->x_vel - MOVE_POWUH));
        }

        hero->direction = direction;
    }
}

void HERO_jump(
    hero_t      *hero
) {
    if (hero->state == STANDING || hero->state == WALKING) {
        hero->state = JUMPING;
        hero->y_vel -= JUMP_POWUH;
    }
}

void HERO_draw(
    hero_t *hero
) {
    GFX_render_texture(
        hero->sprites->texture,
        HERO_current_frame(hero),
        hero->view_x,
        hero->view_y,
        hero->direction
    );
}

void HERO_free(
    hero_t *hero
) {
    GFX_free_texture(hero->sprites->texture);
    free(hero->sprites);
}
