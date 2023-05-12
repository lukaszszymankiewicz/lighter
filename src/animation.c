#include "animation.h"

void ANIM_free(
    animation_sheet_t* animation
) {
    animation->texture = NULL;
    free(animation);
    animation          = NULL;
}

// TODO: add width and height to animation_sheet_t struct
// TODO: divide this method into two separate parts (width and height), do not
// fight with SDL_Rect
SDL_Rect* ANIM_get_whole_texture_size(
    animation_sheet_t* animation
) {
    SDL_Rect* clip = NULL;
    clip           = (SDL_Rect*)malloc(sizeof(SDL_Rect));

    clip->x = 0;
    clip->y = 0;

    clip->w = TXTR_width(animation->texture);
    clip->h = TXTR_height(animation->texture);

    return clip;
}

render_img_t ANIM_texture_coord(
    animation_t *animation,
    int          idx,
    int          n_frame
) {
    return animation[idx].frames[n_frame].img;
}

render_img_t ANIM_texture_coord_full(
    animation_sheet_t* animation
) {
    render_img_t coord;

    coord.x1 = 0.0;
    coord.y1 = 0.0;
    coord.x2 = (float)TXTR_width(animation->texture);
    coord.y2 = (float)TXTR_height(animation->texture);

    return coord;
}
