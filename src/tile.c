#include "img.h"
#include "texture.h"
#include "tile.h"
#include "gfx.h"


//tile_img_t* TILE_img_new(
//    texture_t *texture,   // texture from which tile is clipped
//    int x1,               // x1 of clip  
//    int y1,               // y1 of clip
//    int x2,               // x2 of clip
//    int y2                // y2 of licp
//) {
//
//    float texW = (float)TXTR_width(texture);
//    float texH = (float)TXTR_height(texture);
//
//    tile_img_t *new_tile   = NULL;
//    new_tile               = (tile_img_t*)malloc(sizeof(tile_img_t));
//
//    new_tile->texture      = texture;
//    new_tile->render_x1    = (float)x1/(float)texW;
//    new_tile->render_y1    = (float)y1/(float)texH;
//    new_tile->render_x2    = (float)x2/(float)texW;
//    new_tile->render_y2    = (float)y2/(float)texH;
//
//    return new_tile;
//}

tile_t* TILE_new(
    texture_t *texture,      // tile img
    int        clip_x1,      // clip of the tile from texture (in pixels)
    int        clip_y1,      // clip of the tile from texture (in pixels)
    int        clip_x2,      // clip of the tile from texture (in pixels)
    int        clip_y2,      // clip of the tile from texture (in pixels)
    int        x,            // x of tile on level (left, down coord)
    int        y             // y of tile on level (left, down coord)
) {
    tile_t *new_tile       = NULL;
    new_tile               = (tile_t*)malloc(sizeof(tile_t));

    float render_x1, render_y1, render_x2, render_y2;
        
    // TODO: this should be moved to separate function
    render_x1    = (float)x / (float)SCREEN_WIDTH;
    render_y1    = (float)y / (float)SCREEN_HEIGHT;
    render_y1   *= -1;
    render_x2    = render_x1 + WIDTH_COEF;
    render_y2    = render_y1 - HEIGHT_COEF;
    
    render_x1   *= global_x_scale;
    render_y1   *= global_y_scale;
    render_x2   *= global_x_scale;
    render_y2   *= global_y_scale;

    new_tile->coord = IMG_COORD_new(render_x1, render_y1, render_x2, render_y2);
    new_tile->img   = IMG_new(texture, clip_x1, clip_y1, clip_x2, clip_y2);

    return new_tile;


    // printf("texture: %f %f \n", w_diff * (float)TXTR_width(new_tile->img->texture), h_diff * (float)TXTR_height(new_tile->img->texture));

    // float render_y2 = (float)render_y1 - ((tex_y2 - tex_y1) * texH);
    // float render_x2 = (float)render_x1 + ((tex_x2 - tex_x1) * texW);

    // float render_y2 = (float)render_y1 - (float)abs(tex_y2 - tex_y1); // -COS
    // float render_x2 = (float)render_x1 + (float)abs(tex_x2 - tex_x1); // + COS
    //
    // new_tile->render_x1 = ((float)TILE_WIDTH * x / (float)SCREEN_WIDTH) - ((float)SCREEN_WIDTH /2.0);
    // new_tile->render_y1 = (((float)TILE_WIDTH * y / (float)SCREEN_HEIGHT) - ((float)SCREEN_HEIGHT / 2.0)) * -1;

    // new_tile->render_x1 = ((float)(TILE_WIDTH * x) - (float)SCREEN_WIDTH/2.0) / (float)SCREEN_WIDTH;
    // new_tile->render_y1 = ((float)(TILE_HEIGHT * y) - (float)SCREEN_HEIGHT/2.0) / (float)SCREEN_HEIGHT;
    // printf("new tile created %d, %d \n", x, y);
    // new_tile->render_x1 = ((float)(TILE_WIDTH * x) - (float)SCREEN_WIDTH/2.0) / (float)SCREEN_WIDTH;
    // new_tile->render_y1 = ((float)(TILE_HEIGHT * y) - (float)SCREEN_HEIGHT/2.0) / (float)SCREEN_HEIGHT;

    // new_tile->render_y1 *= -1;

    // OLD
    //
    // camera_x must knonw!
    //
    // camera_x -> hero->x
    // int camera_x = hero->x
    // int st_x           = camera_x - ENTITY_DRAW_X_RANGE;
    // int tile_pos_x = TILE_WIDTH  * x - st_x;
    // tile_pos_x -> render_x1
    
    // hero->x - ENTITY_DRAW_X_RANGE
    // tile_pos_x = TILE_WIDTH * x - COS
    // GFX
    // render_x1 -= (float)SCREEN_WIDTH / 2.0;  // -COS
    // render_y1 *= -1; (+ COS?)
    // float render_y2 = (float)render_y1 - (float)abs(tex_y2 - tex_y1); // -COS
    // float render_x2 = (float)render_x1 + (float)abs(tex_x2 - tex_x1); // + COS
    ///// 
    // render_x1 / SCREEN_WIDTH  - camera / SCREEN_WIDTH
    // render_y1 / SCREEN_HEIGHT - camera / SCREEN_HEIGHT
}

void TILE_free(
    tile_t *tile
) {
    free(tile);    
}
