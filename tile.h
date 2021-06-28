#ifndef COMMON_H
#define COMMON_H
#define DEBUG 1

typedef struct tile 
{
    short int x1; short int y1;  // rect pos
    short int x2; short int y2;  // rect post
    short int r; short int b; short int g; short int a;  // color
} tile_t;

typedef struct tiles_list 
{
    tile_t * tile;
    struct tiles_list * next;
} tiles_list_t;

void TILE_push(
    tiles_list_t** list,
    short int x1, short int y1,
    short int x2, short int y2,
    short int r, short int g, short int b, short int a
);

void TILE_free(tiles_list_t* list);

tile_t * TILE_init(
    short int x1, short int y1,
    short int x2, short int y2,
    short int r, short int g,
    short int b, short int a
);

#endif

