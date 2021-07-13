#include <stdlib.h>
#include "tile.h"

tile_t * TILE_init(
    short int x1, short int y1,
    short int x2, short int y2,
    short int r, short int g,
    short int b, short int a
)
{
    tile_t * new_tile = NULL;
    new_tile = (tile_t *)malloc(sizeof(tile_t *));

    new_tile->x1 = x1;
    new_tile->y1 = y1;
    new_tile->x2 = x2;
    new_tile->y2 = y2;

    new_tile->r = r;
    new_tile->g = g;
    new_tile->b = b;
    new_tile->a = a; 

    return new_tile;
}

void TILE_push(
    tiles_list_t** list,
    short int x1, short int y1,
    short int x2, short int y2,
    short int r, short int g, short int b, short int a
)
{
    tiles_list_t * new_level_tile = NULL;
    new_level_tile = (tiles_list_t *)malloc(sizeof(tiles_list_t));

    tile_t * new_tile = TILE_init(x1, y1, x2, y2, r, g, b, a);
    new_level_tile->tile = new_tile;
    new_level_tile->next = *list;

    *list = new_level_tile;

}

void TILE_free(tiles_list_t* list) 
{
    tiles_list_t* currentRef = list;

    while (currentRef != NULL) {
        tiles_list_t * temp = currentRef->next;
        free(currentRef->tile);
        free(currentRef);
        currentRef = temp;
    }
}

