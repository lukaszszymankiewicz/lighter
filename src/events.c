#include "events.h"
#include "light.h"
#include "def.h"

#define SATURATE(a, b) ((b) ? (a = a + b) : (0))
#define KEY_PRESSED(a) (game->keys_cooldown[a] > keyboard_delay[a])
#define N_LEGAL_KEYS 5

static int keyboard_delay[MAX_KEYS];

static int legal_keys[N_LEGAL_KEYS] = 
{
    SDL_SCANCODE_SPACE,
    SDL_SCANCODE_RIGHT,
    SDL_SCANCODE_LEFT,
    SDL_SCANCODE_DOWN,
    SDL_SCANCODE_UP
};


void EVNT_init() {
  // values other than zero means that you cannot spam keypresses.  values other than zero means for
  // how long (in game-frames) button should be pressed to be handled. Please note that this value
  // should be rather low to be anyhow responsive.
  keyboard_delay[SDL_SCANCODE_SPACE] = 1;
  keyboard_delay[SDL_SCANCODE_RIGHT] = 0;
  keyboard_delay[SDL_SCANCODE_LEFT] = 0;
  keyboard_delay[SDL_SCANCODE_UP] = 0;
  keyboard_delay[SDL_SCANCODE_DOWN] = 0;
}

// handles input from player and manipulating game window (resize, exit etc)
void EVNT_handle_SDL_events(game_t *game) {

  while (SDL_PollEvent(&(game->event)) != 0) 
  {
    if (game->event.type == SDL_QUIT) 
    { game->loop = false; }
  }
}

void EVTN_clear_keypress(game_t * game, int key)
{
  game->keys_cooldown[key] = 0;
}

void EVNT_handle_keypress(game_t *game) {
  // UP && DOWN ARROW
  if KEY_PRESSED(SDL_SCANCODE_UP)
  {
    LIG_move_lightsource(game->light, UP, game->hero->direction, game->frame);
    EVTN_clear_keypress(game, SDL_SCANCODE_UP);
  }
  else if KEY_PRESSED(SDL_SCANCODE_DOWN) 
  {
    LIG_move_lightsource(game->light, DOWN, game->hero->direction, game->frame);
    EVTN_clear_keypress(game, SDL_SCANCODE_DOWN);
  }
  else
  {
    LIG_move_lightsource(game->light, NONE, game->hero->direction, game->frame);
  }

  // SPACE
  if KEY_PRESSED(SDL_SCANCODE_SPACE)
  {
    LIG_change_source(game->light);
    EVTN_clear_keypress(game, SDL_SCANCODE_SPACE);
  }

  // RIGHT ARROW
  if KEY_PRESSED(SDL_SCANCODE_RIGHT)
  {
    HERO_move(game->hero, RIGHT);
    EVTN_clear_keypress(game, SDL_SCANCODE_RIGHT);
  }

  // LEFT ARROW
  if KEY_PRESSED(SDL_SCANCODE_LEFT)
  {
    HERO_move(game->hero, LEFT);
    EVTN_clear_keypress(game, SDL_SCANCODE_LEFT);
  }

}

void EVNT_handle_events(game_t *game) {
  // firstly SDL events are handled
  EVNT_handle_SDL_events(game);

  // secondly keyboard events are handled
  // new keystate are read
  game->keys_state = SDL_GetKeyboardState(NULL);

  // cooldown is calculated, we check only for keys that are detectable
  for (int i=0; i<N_LEGAL_KEYS; i++) 
  {
      int key = legal_keys[i];
      SATURATE(game->keys_cooldown[key], game->keys_state[key]);
  }

  // keyboard events are handled
  EVNT_handle_keypress(game);
}
