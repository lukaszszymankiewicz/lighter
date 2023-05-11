#include "img.h"
#include "texture.h"

render_img_t IMG_new(
    texture_t *texture,   // texture from which tile is clipped
    int x1,               // x1 of texture clip (in pixels)
    int y1,               // y1 of texture clip (in pixels)
    int x2,               // x2 of texture clip (in pixels)
    int y2                // y2 of texture clip (in pixels)
) {

    float texW = (float)TXTR_width(texture);
    float texH = (float)TXTR_height(texture);

    render_img_t new_img;

    new_img.x1       = (float)x1/(float)texW;
    new_img.y1       = (float)y1/(float)texH;
    new_img.x2       = (float)x2/(float)texW;
    new_img.y2       = (float)y2/(float)texH;

    return new_img;
}

render_coord_t IMG_COORD_new(
    float x1,                // x1 of render
    float y1,                // y1 of render
    float x2,                // x2 of render
    float y2                 // y2 of render
) {
    render_coord_t new_coords;

    new_coords.x1 = x1;
    new_coords.y1 = y1;
    new_coords.x2 = x2;
    new_coords.y2 = y2;

    return new_coords;
}
