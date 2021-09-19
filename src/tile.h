#include "primitives.h"
#include "obstacle.h"

#ifndef TILE_H
#define TILE_H

typedef struct tile {
  short int x1;
  short int y1; // rect pos
  short int x2;
  short int y2; // rect post
  short int r;
  short int b;
  short int g;
  short int a; // color
} tile_t;

typedef struct tiles_list {
  tile_t *tile;
  struct tiles_list *next;
} tiles_list_t;

tile_t *TILE_init(short int x1, short int y1, short int x2, short int y2,
                  short int r, short int g, short int b, short int a);
void TILE_push(tiles_list_t **list, short int x1, short int y1, short int x2,
               short int y2, short int r, short int g, short int b,
               short int a);
obstacle_t* TILE_calculate_ray_obstacles(tiles_list_t * tiles);
void TILE_free(tiles_list_t *list);

#endif
