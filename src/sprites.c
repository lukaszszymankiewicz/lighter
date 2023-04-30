#include "assets.h"
#include "global.h"
#include "primitives.h"
#include "sprites.h"
#include "import.h"

static char buffer[BUFFER_SIZE];
animation_sheet_t *animations[ASSET_ANIMATION_ALL];

void ANIM_free(
    animation_sheet_t* animation
) {
    animation->texture = NULL;
    free(animation);
    animation = NULL;
}

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

animation_sheet_t* ANIM_init(
    animation_sheet_t* animation,
    texture_t* texture
) {
    if (animation != NULL && texture != NULL) {
        animation->texture = texture;
    } else if (animation == NULL && texture != NULL) {
        animation                          = (animation_sheet_t*)malloc(sizeof(animation_sheet_t));
        animation->texture                 = texture;
        animation->n_animations            = 0;
    }

    return animation;
}

animation_sheet_t* ANIM_read_animation(
    const char *filepath
) {
    animation_sheet_t *sheet      = NULL;

    sheet                         = malloc(sizeof(animation_sheet_t));

    int state                     = READ_ANIMATION_PREAMBULE_IDLE;
    FILE *file                    = NULL;
    file                          = fopen(filepath, LEVEL_READ_MODE);
    
    int coords[COORDS_PER_RECT];
    int idx                       = 0; // temp container
    int frame_idx                 = 0; // temp container
    int hitbox_idx                = 0; // temp container
    int rect_coord_idx            = 0; // temp container
    int cur_animation             = 0; // temp container

    state++;

    if (!file) {return NULL;}

    while((fread(buffer, DOUBLE_BYTE, PROPER_PACK_COUNT, file) == PROPER_PACK_COUNT)) {

    switch (state) 
    {
        // preambule
        case READ_ANIMATION_PREAMBULE_FIRST_HALF:
            if(IMP_cast_val_to_dec(buffer) != ANIMATION_PREAMBULE[FIRST_HALF]) { return NULL; }
            state++; break;
             
        case READ_ANIMATION_PREAMBULE_SECOND_HALF:
            if(IMP_cast_val_to_dec(buffer) != ANIMATION_PREAMBULE[SECOND_HALF]) { return NULL; }
            state++; break;
        
        // animation sheet
        case READ_ANIMATIONS_NUMBER:
            sheet->n_animations = IMP_cast_val_to_dec(buffer);
            state++; break;
        
        // single animation
        case READ_ANIMATION_IDX:
            idx = IMP_cast_val_to_dec(buffer);
            state++; break;

       case READ_ANIMATION_N_FRAMES:
            sheet->animations[idx].len = IMP_cast_val_to_dec(buffer);
            state++; break;

       // single frame
       case READ_ANIMATION_DELAY:
            sheet->animations[idx].frames[frame_idx].delay = IMP_cast_val_to_dec(buffer);
            state++; break;
       
       case READ_ANIMATION_HANDLE_X:
            sheet->animations[idx].frames[frame_idx].handle_x = IMP_cast_val_to_dec(buffer);
            state++; break;

       case READ_ANIMATION_HANDLE_Y:
            sheet->animations[idx].frames[frame_idx].handle_y = IMP_cast_val_to_dec(buffer);
            state++; break;

       case READ_ANIMATION_RECT:
            coords[rect_coord_idx] = IMP_cast_val_to_dec(buffer);

            if (++rect_coord_idx == COORDS_PER_RECT) {

                sheet->animations[idx].frames[frame_idx].rect.x = coords[0];
                sheet->animations[idx].frames[frame_idx].rect.y = coords[1];
                sheet->animations[idx].frames[frame_idx].rect.w = coords[2];
                sheet->animations[idx].frames[frame_idx].rect.h = coords[3];

                rect_coord_idx = 0; state++; 
            }


            break;

       case READ_ANIMATION_HITBOX_PER_FRAME:
            sheet->animations[idx].frames[frame_idx].n_hit_box = IMP_cast_val_to_dec(buffer);
            state++; break;

       case READ_ANIMATION_HITBOX_RECT:
            coords[rect_coord_idx] = IMP_cast_val_to_dec(buffer);

            if (rect_coord_idx++ == COORDS_PER_RECT-1) {
                sheet->animations[idx].frames[frame_idx].hit_boxes[hitbox_idx].x = coords[0];
                sheet->animations[idx].frames[frame_idx].hit_boxes[hitbox_idx].y = coords[1];
                sheet->animations[idx].frames[frame_idx].hit_boxes[hitbox_idx].w = coords[2];
                sheet->animations[idx].frames[frame_idx].hit_boxes[hitbox_idx].h = coords[3];

                rect_coord_idx = 0;

                if (++hitbox_idx == sheet->animations[idx].frames[frame_idx].n_hit_box) {

                    if (++frame_idx < sheet->animations[idx].len) {
                        state = READ_ANIMATION_DELAY;
                        hitbox_idx = 0;
                        rect_coord_idx = 0;
                    } else if (++cur_animation < sheet->n_animations) {
                        hitbox_idx = 0;
                        rect_coord_idx = 0;
                        frame_idx = 0;
                        state = READ_ANIMATION_IDX;
                    } else {
                        state++;
                    }
                }
            }
            break;
        }
    }
    return sheet;
}
