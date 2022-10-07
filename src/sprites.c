#include "gfx.h"
#include "sprites.h"

animation_sheet_t* TXTR_init_animation_sheet (
    char *filepath
) {
    animation_sheet_t *new_sheet = NULL;
    new_sheet                    = malloc(sizeof(animation_sheet_t));
    new_sheet->texture           = GFX_read_texture(filepath);

    return new_sheet;
}
