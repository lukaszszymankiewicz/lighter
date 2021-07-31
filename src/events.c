#include "def.h"
#include "events.h"

#define SATURATE(a, b) ((b) ? (a=a+b) : (0))

static Uint8 *keys_delay;
static Uint8 *keys_punish;
static int n_keyboard_keys;

void EVNT_init()
{
    static SDL_Scancode legal_keys[3]={SDL_SCANCODE_SPACE, SDL_SCANCODE_RIGHT, SDL_SCANCODE_LEFT};

    // this will get the number of keys properly
    SDL_GetKeyboardState(&n_keyboard_keys);

    keys_delay = (Uint8*)malloc(sizeof(Uint8) * n_keyboard_keys);
    keys_punish = (Uint8*)malloc(sizeof(Uint8) * n_keyboard_keys);

    // values other than zero means that you cannot spam keypresses. 
    // values other than zero means for how long (in game-frames) button should be pressed to be
    // handled. Please note that this value should be ather low to be anyhow responsive.
    keys_delay[SDL_SCANCODE_SPACE] = 2;
    keys_delay[SDL_SCANCODE_RIGHT] = 0;
    keys_delay[SDL_SCANCODE_LEFT] = 0;

    // this value is cooldown (in game-fps) which user needs to wait until another same keypress
    // should be handled
    keys_punish[SDL_SCANCODE_SPACE] = -1;
    keys_punish[SDL_SCANCODE_RIGHT] = 0;
    keys_punish[SDL_SCANCODE_LEFT] = 0;
}

// handles input from player and manipulating game window (resize, exit etc)
void EVNT_handle_SDL_events(game_t* game)
{
    while(SDL_PollEvent(&(game->event)) != 0) 
    {
        if(game->event.type == SDL_QUIT) 
        {
            game->loop = false;
        }
    }
}


void EVNT_handle_keypress(
    game_t* game
)
{
    if (game->keys_cooldown[SDL_SCANCODE_SPACE] >= keys_delay[SDL_SCANCODE_SPACE])
    {
        LIG_change_source(game->light);
        game->keys_cooldown[SDL_SCANCODE_SPACE] = 0;
    }

    if (game->keys_state[SDL_SCANCODE_RIGHT])
    {

        HERO_move(game->hero, RIGHT);
        game->light->angle = RIGHT_RAD;
    }

    if (game->keys_state[SDL_SCANCODE_LEFT])
    {
        HERO_move(game->hero, LEFT);
        game->light->angle = LEFT_RAD;
    }
}

void EVNT_handle_events(game_t *game)
{
    // firstly SDL events are handled
    EVNT_handle_SDL_events(game);

    // secondly keyboard events are handled
    // new keystate are read
    game->keys_state = SDL_GetKeyboardState(NULL);
    
    // cooldown is calculated
    for (int i=0; i<n_keyboard_keys; i++)
    {
        SATURATE(game->keys_cooldown[i], game->keys_state[i]);
    }

    // keyboard events are handled
    EVNT_handle_keypress(game);
}
