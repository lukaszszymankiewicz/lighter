#include "gfx.h"
#include "tile.h"


tiles_list_t * LVL_read_level()
{
    tiles_list_t * tiles = NULL;

    //dummy
    TILE_push(&tiles, 10, 10, 74, 42, 30, 73, 102, 255);
    TILE_push(&tiles, 10, 10, 106, 42, 30, 73, 102, 255);
    TILE_push(&tiles, 10, 42, 42, 189, 30, 73, 102, 255);
    TILE_push(&tiles, 42, 157, 309, 189, 30, 73, 102, 255);
    TILE_push(&tiles, 267, 93, 309, 157, 30, 73, 102, 255);
    TILE_push(&tiles, 267, 10, 309, 42, 30, 73, 102, 255);
    TILE_push(&tiles, 74, 74, 106, 128, 30, 73, 102, 255);

    // border
    TILE_push(&tiles, 1, 1, 319, 199, 12, 12, 12, 0);

    return tiles;
}

void LVL_draw(tiles_list_t * tiles)
{
    tiles_list_t * ptr = NULL;
    ptr = tiles;

    while(ptr)
    {
        SDL_SetRenderDrawColor(renderer, ptr->tile->r, ptr->tile->g, ptr->tile->b, ptr->tile->a);
        SDL_Rect tile_rect = {
            ptr->tile->x1,
            ptr->tile->y1,
            ptr->tile->x2 - ptr->tile->x1,
            ptr->tile->y2 - ptr->tile->y1,
        };
        SDL_RenderFillRect(renderer, &tile_rect);

        ptr = ptr->next;
    }
}
