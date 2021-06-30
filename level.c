#include "gfx.h"
#include "tile.h"


tiles_list_t * LVL_read_level()
{
    tiles_list_t * tiles = NULL;

    //dummy
    TILE_push(&tiles, 10, 256, 60, 506, 30, 73, 102, 255);

    TILE_push(&tiles, 150, 306, 200, 506, 30, 73, 102, 255);
    TILE_push(&tiles, 50, 456, 150, 506, 30, 73, 102, 255);

    TILE_push(&tiles, 150, 256, 492, 306, 30, 73, 102, 255);
    TILE_push(&tiles, 50, 50, 100, 100, 30, 73, 102, 255);
    TILE_push(&tiles, 330, 50, 380, 100, 30, 73, 102, 255);
    TILE_push(&tiles, 300, 100, 492, 150, 30, 73, 102, 255);
    TILE_push(&tiles, 442, 150, 492, 256, 30, 73, 102, 255);

    // TILE_push(&tiles, 100, 400, 200, 450, 30, 73, 102, 255);
    // TILE_push(&tiles, 250, 400, 400, 450, 30, 73, 102, 255);

    // border
    TILE_push(&tiles, 1, 1, 511, 511, 12, 12, 12, 0);

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
