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

render_coord_t ANIM_texture_coord_full(
    animation_sheet_t animation
) {
    return (render_coord_t) { 0.0, 0.0, 1.0, 1.0 }; 
}
