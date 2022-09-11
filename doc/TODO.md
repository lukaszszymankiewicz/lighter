MILESTONE 1
codename: make light pretty
 - [x] wobble should be stronger in air jumping,
 - [x] implement radius of the light effect,
 - [x] implement framebuffer for light effect,
 - [x] light should penetrate walls, but weaker.

MILESTONE 2
codename: make dev easy - create tools for level editing
developing tools will be stored in such [repo](https://github.com/lukaszszymankiewicz/ltools)
 - [x] basic level editor
 - [x] level format coherent between editor && game

 MILESTONE 3
 codename: make gameplay cool
 - [ ] add some items on level (use editor),
 - [ ] add some enemy (use editor for aniemation sheets) - behavior of each enemy will propably be
   too different from each other so behavior editor will be propably be useless,
 - [ ] add cutscene system (text + face, scripted hero movement etc).

 KEWL FEATURES:
 - [ ] drawing gradient of light blit whole texture on a screen. Only part of it (shaped by light
   polygon) should be blitted. To obtain so, SDL_RenderCopy needs to be replaced by some custom
   function and custom pix_draw_fun.
 - [ ] other way od drawing light penetrating wall - by now it is drawn pixel by pixel, maybe some
   simple tile will do the work?
 - [ ] script for stand-alone distribution of game for linux,
 - [ ] scipt for stand-alone distribution of game for Windows,
 - [ ] Put SDL library directly into game files.
 - [ ] check for unused code
 - [ ] filetype which will store Hero/Object/light data,
