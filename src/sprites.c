#include "gfx.h"
#include "sprites.h"

animation_sheet_t* TXTR_init_animation_sheet (
    char *filepath,     // path to animation sheet image
    int   n_animations  // number of animation to created from sheet
) {
    animation_sheet_t *new_sheet = NULL;
    new_sheet                    = malloc(sizeof(animation_sheet_t));

    new_sheet->texture      = GFX_read_texture(filepath);
    new_sheet->n_animations = n_animations;

    return new_sheet;
}

// creates animations (set of sprites with given delay and length). Each sprite from animation is
// just SDL_Rect indicating area to be cut from sprite sheet.
animation_t* TXTR_init_animation (
    int rect[][4],        // animation frames trimmed from animation sheet rects
    int hit_boxes[][4],   // hit boxes of frames n_hit_box * n_frames
    int delay,            // delay (in game-frames) before next frame animations occures
    int n_frames,         // animation lengths (number of sprites creating given animation)
    int n_hit_box[]       // hit boxes per frame
) {
    animation_t *new_animation = NULL;
    new_animation              = malloc(sizeof(animation_t));

    new_animation->len   = n_frames;
    new_animation->delay = delay;
    int z                = 0;

    for (int i=0; i<n_frames; i++) {
        new_animation->frames[i].rect = (SDL_Rect) {
            rect[i][FRAME_X],
            rect[i][FRAME_Y],
            rect[i][FRAME_WID],
            rect[i][FRAME_HS]
        };
        new_animation->frames[i].n_hit_box = n_hit_box[i];

        for (int j=0; j<n_hit_box[i]; j++) {
            new_animation->frames[i].hit_boxes[j] = (SDL_Rect) {
                hit_boxes[z][FRAME_X],
                hit_boxes[z][FRAME_Y],
                hit_boxes[z][FRAME_WID],
                hit_boxes[z][FRAME_HS]
            };
            z++;
        }
    }

    return new_animation;
}

// cuts animation sheet according to inputted rects and change it into animation struct
void TXTR_push_animation (
    animation_sheet_t *sheet,      // animation sheet stuct holding image to be cut
    int index,                     // index of animation (every animation should have unique index value)
    int rects[][4],                // animation frames trimmed from animation sheet rects
    int hit_boxes[][4],            // hit boxes of frames n_hit_box * n_frames
    int delay,                     // delay (in game-frames) before next frame animations occures
    int n_frames,                  // animation lengths (number of sprites creating given animation)
    int n_hit_box[]                // hit boxes per frame
) {
    animation_t *new_animation = NULL;
    new_animation              = TXTR_init_animation(rects, hit_boxes, delay, n_frames, n_hit_box);

    sheet->animations[index] = new_animation;
}

void TXTR_free_animation_sheet(animation_sheet_t *sheet) {
    for (int i=0; i<sheet->n_animations; i++) { 
        free(sheet->animations[i]);
    }
    GFX_free_texture(sheet->texture);
    free(sheet);
}
