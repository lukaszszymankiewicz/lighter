#include "animation.h"

void ANIM_free(
    animation_sheet_t* animation
) {
    free(animation);
    animation          = NULL;
}

int ANIM_get_texture_width(
    animation_sheet_t animation
) {
    return animation.width;
}

int ANIM_get_texture_height(
    animation_sheet_t animation
) {
    return animation.height;
}
