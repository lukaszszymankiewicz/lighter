#include "gfx.h"
#include "tile.h"


tiles_list_t * LVL_read_level()
{
    tiles_list_t * tiles = NULL;

    //dummy
    TILE_push(&tiles, 450, 450, 500, 500, 30, 73, 102, 255);
    TILE_push(&tiles, 50, 50, 100, 100, 30, 73, 102, 255);
    TILE_push(&tiles, 30, 170, 80, 230, 30, 73, 102, 255);
    TILE_push(&tiles, 300, 20, 350, 70, 30, 73, 102, 255);
    TILE_push(&tiles, 350, 120, 400, 170, 30, 73, 102, 255);
    TILE_push(&tiles, 100, 400, 200, 450, 30, 73, 102, 255);
    TILE_push(&tiles, 250, 400, 400, 450, 30, 73, 102, 255);
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
