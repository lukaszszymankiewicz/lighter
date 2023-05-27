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

// render_coord_t ANIM_texture_coord(
//     animation_t  animation,
//     int          idx,
//     int          n_frame
// ) {
//     return animation[idx].frames[n_frame].img;
// }

render_coord_t ANIM_texture_coord_full(
    animation_sheet_t animation
) {
    render_coord_t coord;

    coord.x1 = 0.0;
    coord.y1 = 0.0;
    coord.x2 = (float)animation.width;
    coord.y2 = (float)animation.height;

    return coord;
}
