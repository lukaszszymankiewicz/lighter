#include "sprites.h"

void ANIM_free(
    animation_sheet_t* animation
) {
    animation->texture = NULL;
    free(animation);
    animation = NULL;
}
