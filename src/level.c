#include "gfx.h"
#include <stdio.h>
#include "level.h"
#include "config.h"

#define MAX_LEN 32
#define MAX_LEVEL_PATH 32

tiles_list_t *LVL_open(int level_n) {
    tiles_list_t *tiles                 = NULL;
    FILE         *file_pointer          = NULL;
    char          level_number[MAX_LEN];

    //change level number to string
    snprintf(level_number, 10, "%d", level_n);

    char level_file_path[MAX_LEVEL_PATH] = strcat(LEVELS_FOLDER, BASE_LEVEL_NAME, level_number, LEVEL_FILE_EXTENSION);

    file_pointer = fopen(level_file_path, "r");

    if (file_pointer == NULL) {
        perror("Failed: ");
    }

    char buffer[MAX_LEN];

    // -1 to allow room for NULL terminator for really long string
    while (fgets(buffer, MAX_LEN - 1, file_pointer)) {
        if (strcmp("SIZE", buffer)) {

            // find size
        };

        // Remove trailing newline
        buffer[strcspn(buffer, "\n")] = 0;
        printf("%s\n", buffer);
    }

    fclose(file_pointer);

    return 0;
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

void LVL_draw(tiles_list_t * tiles) {
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
