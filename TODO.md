 SMALL THINGS:
 - [ ] remove all printf
 - [ ] add cool background
 - [x] better detection if vertex is on the same line (VRTX_optim)
 - [ ] add filtered obstacles
 - [ ] repair all tests
 - [ ] check obstacle calculations for horizontal lines (something does not work)
 - [ ] upgrade layers of graphics (now tiles are above the light)

 BORING THINGS:
 - [ ] refactor Makefile
 - [x] refactor ray collision (some of function could easly use integers, rather than float),
 - [ ] scale graphics to full screen - because perfect pixel-for-pixel graphics is important in
   this, project, unfortunatly not all resolutions will looks good. Some function to determine input
   resolution and scale factor is needed.,
 - [ ] Investigate segfaults

 COOL THINGS:
 - [x] any of the more sofisticated controls and movements of a hero (looking up/down with lighter),
 - [x] any working level mechanism for placing/drawing tiles,
 - [x] scrolling (works anyhow by now),
 - [ ] add light on the vertex of the lighted wall,
 - [ ] implement radius of the light effect,
 - [ ] implement framebuffer for light effect (right now the whole light scene is drawn n times,
   where n is the number of polygon from which light effect is made. For Example lighter is using 5
   polygons),
 - [ ] gravity.

 MILESTONES:
 - [ ] GUI level editor,
 - [ ] prepare something what will make stand-alone distribution of game for linux,
 - [ ] prepare something what will make stand-alone distribution of game for Windows.

 VERY HARD THINGS:
 - [ ] try implement OpenGL shade for light effect (it is possible but require some research and
   learning)

