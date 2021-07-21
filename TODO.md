 DONE:
 - [x] add hero movement,
 - [x] add walls,
 - [x] rays collission with walls,
 - [x] fill light! (and obstacles polygons if that matters!),
 - [x] make sure that point is always expressed in integer numbers (not floats!),
 - [x] fill full polygon, not set of triangles,
 - [x] cut code into segments (light/primitives/shared structs etc),
 - [x] add new lightsource (lighter),
 - [x] add alpha lighting (offset rays should make not fully light),
 - [x] add alpha lighting (for lighter light source),
 - [x] get rid of strange artifacts while endering texture (propably another another texture will do,
   the trick),
 - [x] add some frame limit, as now game use 100% of the CPU.,
 - [x] any of the grpahics of a hero,
 - [x] calculate exact place of light source (done for lighter).
 - [x] lighter in the hand of ther should shake a little bit (up and down) in slowly matter to look more
   realistic,

 BORING THINGS:
 - [ ] document the code (WIP),
 - [ ] add tests (WIP),
 - [ ] refactor ray collision (some of function could easly use integers, rather than float),
 - [ ] scale graphics to full screen - because perfect pixel-for-pixel graphics is important in
   this, project, unfortunatly not all resolutions will looks good. Some function to determine input
   resolution and scale factor is needed.,
 - [ ] cleanup lighter.c file

 COOL THINGS:
 - [ ] hero animation
 - [ ] implement radius of the light effect,
 - [ ] implement framebuffer for light effect (right now the whole light scene is drawn n times,
   where n is the number of polygon from which light effect is made. For Example lighter is using 5
   polygons),
 - [ ] gravity,
 - [ ] any of the more sofisticated controls and movements of a hero,
 - [ ] scrolling,
 - [ ] any of the gaphics of level.

 MILESTONES:
 - GUI level editor
