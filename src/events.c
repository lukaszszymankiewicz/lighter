#include "events.h"
#include "global.h"
#include "light.h"

static int keyboard_delay[MAX_KEYS];
static int legal_keys[N_LEGAL_KEYS] = {
    SDL_SCANCODE_SPACE,
    SDL_SCANCODE_D,
    SDL_SCANCODE_A,
    SDL_SCANCODE_S,
    SDL_SCANCODE_W,
    SDL_SCANCODE_Q,
};

// values other than zero means that you cannot spam keypresses. Values other than zero means for
// how long (in game-frames) button should be pressed to be handled.
void EVNT_init() {
    // actions
    keyboard_delay[SDL_SCANCODE_SPACE] = 1;
    keyboard_delay[SDL_SCANCODE_Q]     = 0;

    // walking
    keyboard_delay[SDL_SCANCODE_D] = 0;
    keyboard_delay[SDL_SCANCODE_A] = 0;
    keyboard_delay[SDL_SCANCODE_W] = 0;
    keyboard_delay[SDL_SCANCODE_S] = 0;
}

// handles input from player and manipulating game window (resize, exit etc)
void EVNT_handle_SDL_events(
    game_t *game
){

  while (SDL_PollEvent(&(game->event)) != 0) {
    if (game->event.type == SDL_QUIT){ 
        game->loop = false; 
        }
    }
}

// sets given key from keyboard cooldown to zero
void EVTN_clear_keypress(
    game_t *game,
    int     key
) {
    game->keys_cooldown[key] = 0;
}

void EVNT_handle_keypress(
    game_t *game
) {
  if KEY_PRESSED(SDL_SCANCODE_W) {
      LIG_move_lightsource(game->light, UP, game->hero->direction, game->frame);
      EVTN_clear_keypress(game, SDL_SCANCODE_W);
  }

  else if KEY_PRESSED(SDL_SCANCODE_S) {
      LIG_move_lightsource(game->light, DOWN, game->hero->direction, game->frame);
      EVTN_clear_keypress(game, SDL_SCANCODE_S);
  }

  else {
      LIG_move_lightsource(game->light, NONE, game->hero->direction, game->frame);
  }

  // SPACE
  if KEY_PRESSED(SDL_SCANCODE_SPACE) {
      HERO_jump(game->hero);
      EVTN_clear_keypress(game, SDL_SCANCODE_SPACE);
  }

  // SPACE
  if KEY_PRESSED(SDL_SCANCODE_Q) {
      debug = (debug + 1) % MAX_DEBUG;
      EVTN_clear_keypress(game, SDL_SCANCODE_Q);
  }

  // RIGHT ARROW
  if KEY_PRESSED(SDL_SCANCODE_D) {
      HERO_move(game->hero, RIGHT);
      EVTN_clear_keypress(game, SDL_SCANCODE_D);
  }

  // LEFT ARROW
  if KEY_PRESSED(SDL_SCANCODE_A) {
      HERO_move(game->hero, LEFT);
      EVTN_clear_keypress(game, SDL_SCANCODE_A);
  }

}

void EVNT_handle_events(
    game_t *game
) {
  // firstly SDL events are handled
  EVNT_handle_SDL_events(game);

  // secondly keyboard events are handled
  // new keystate are read
  game->keys_state = SDL_GetKeyboardState(NULL);

  // cooldown is calculated, we check only for keys that are detectable
  for (int i=0; i<N_LEGAL_KEYS; i++) {
      int key = legal_keys[i];
      SATURATE(game->keys_cooldown[key], game->keys_state[key]);
  }

  // keyboard events are handled
  EVNT_handle_keypress(game);
}
