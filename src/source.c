#include "global.h"
#include "import.h"
#include "source.h"
#include "primitives.h"

static char buffer[BUFFER_SIZE];

lightsource_t     *lightsources[ASSET_LIGHTSOURCE_ALL];
wobble_t          *wobbles[ASSET_WOBBLE_ALL];

// changes done to light angle if looking in different direction
float lightpos_up_down_corr[2][5] = {
  // LEFT  RIGHT  UP               DOWN             NONE
     {0,   0,     RIGHT_RAD+DEG30, RIGHT_RAD-DEG30, RIGHT_RAD },      // RIGHT 
     {0,   0,     LEFT_RAD-DEG30,  LEFT_RAD+DEG30,  LEFT_RAD  }       // LEFT 
};

void SRC_move_lightsource(
    lightsource_t     *source,
    direction_t        light_dir,
    direction_t        entity_dir
) {
    source->angle = lightpos_up_down_corr[entity_dir][light_dir];
}

int SRC_get_light_penetrating_power(
    lightsource_t *source
) {
    return source->penetrating_power;
}

int SRC_get_light_polygon_x_corr(
    lightsource_t *source,
    int      i
) {
    return sin(source->angle) * source->light_polygons[i].x + cos(source->angle) * source->light_polygons[i].y;
}

int SRC_get_light_polygon_y_corr(
    lightsource_t *source,
    int            i 
) {
    return sin(source->angle) * source->light_polygons[i].y + cos(source->angle) * source->light_polygons[i].x;
}

float SRC_get_light_polygon_width_corr(
    lightsource_t *source,
    int            i
) {
    int coef = source->light_polygons[i].width;

    if (coef){
        return PI / coef;
    }
    return 0.0;
}

void SRC_change_wobble(
    lightsource_t *source,
    int            wobble_index
) { 
    source->curent_wobble = wobble_index;
}

void SRC_set_wobble(
    lightsource_t *source,
    wobble_t      *wobble,
    int            n
) { 
    source->wobble[n] = wobble;
    source->n_wobbles++;
}

float SRC_get_wobble_angle_coef(
    lightsource_t *source
) {
    if (source->n_wobbles==0) {return 0.0;}

    int len = source->wobble[source->curent_wobble]->len;

    if (source->frame == 0) {
        return source->wobble[source->curent_wobble]->coefs[0];
    }
    return source->wobble[source->curent_wobble]->coefs[source->frame%len];
}

lightsource_t* SRC_read_lightsource(
    const char *filepath
) {
    lightsource_t *lightsource = NULL;
    lightsource                = (lightsource_t*)malloc(sizeof(lightsource_t));

    lightsource->frame          = 0;
    lightsource->curent_wobble  = 0;
    lightsource->n_wobbles      = 0;
    lightsource->light_polygons = NULL;
    lightsource->gradient       = NULL;

    int state                   = READ_LIGHTSOURCE_IDLE;
    FILE *file                  = NULL;
    file                        = fopen(filepath, LEVEL_READ_MODE);
     
    int n_polygon               = 0;
    int width                   = 0;
    int x                       = 0;
    int y                       = 0;

    state++;

    if (!file) {return NULL;}

    while((fread(buffer, DOUBLE_BYTE, PROPER_PACK_COUNT, file) == PROPER_PACK_COUNT)) {
        switch (state) 
        {
            case READ_LIGHTSOURCE_PREAMBULE_FIRST_HALF:
                if(IMP_cast_val_to_dec(buffer) != LIGHTSOURCE_PREAMBULE[FIRST_HALF]) { return NULL; }
                state++; break;

            case READ_LIGHTSOURCE_PREAMBULE_SECOND_HALF:
                if(IMP_cast_val_to_dec(buffer) != LIGHTSOURCE_PREAMBULE[SECOND_HALF]) { return NULL; }
                state++; break;

            case READ_LIGHSOURCE_WIDTH:
                width = IMP_cast_val_to_dec(buffer);

                // just to avoid dividing zero
                if (width != 0) {
                    lightsource->width = PI / IMP_cast_val_to_dec(buffer);
                } else {
                    lightsource->width = 0.0;
                }

                state++; break;

            case READ_LIGHSOURCE_PENETRATING_POWER:
                lightsource->penetrating_power = IMP_cast_val_to_dec(buffer);
                state++; break;

            case READ_LIGHSOURCE_POLYGONS_NUMBER:

                lightsource->n_poly = IMP_cast_val_to_dec(buffer);
                lightsource->light_polygons = (lightpolygon_t*)malloc(sizeof(lightpolygon_t) * lightsource->n_poly);

                state++; break;

            case READ_LIGHSOURCE_POLYGON_X:
                x = IMP_cast_val_to_dec(buffer);

                // invert the sign
                if (x > 255 * 256) {
                    x -= 255 * 256;
                    x *= -1;
                }

                lightsource->light_polygons[n_polygon].x = x;
                state++; break;

            case READ_LIGHSOURCE_POLYGON_Y:
                y = IMP_cast_val_to_dec(buffer);

                // invert the sign
                if (y > 255 * 256) {
                    y -= 255 * 256;
                    y *= -1;
                }
                lightsource->light_polygons[n_polygon].y = y;
                state++; break;

            case READ_LIGHSOURCE_POLYGON_RED:
                lightsource->light_polygons[n_polygon].red = IMP_cast_val_to_dec(buffer);
                state++; break;

            case READ_LIGHSOURCE_POLYGON_GREEN:
                lightsource->light_polygons[n_polygon].green = IMP_cast_val_to_dec(buffer);
                state++; break;

            case READ_LIGHSOURCE_POLYGON_BLUE:
                lightsource->light_polygons[n_polygon].blue = IMP_cast_val_to_dec(buffer);
                state++; break;

            case READ_LIGHSOURCE_POLYGON_POWER:
                lightsource->light_polygons[n_polygon].light_power = IMP_cast_val_to_dec(buffer);
                state++; break;

            case READ_LIGHSOURCE_POLYGON_WIDTH:
                lightsource->light_polygons[n_polygon].width = IMP_cast_val_to_dec(buffer);

                if (++n_polygon == lightsource->n_poly) {
                    state++; break;
                } {
                    state = READ_LIGHSOURCE_POLYGON_X; break;
                }

            case READ_LIGHSOURCE_WOBBABLE:
                // UNUSED, sorry :C
                IMP_cast_val_to_dec(buffer);
                state++; break;
        }
    }

    // TODO: add some initialization for this one!
    lightsource->angle = LEFT_RAD;

    return lightsource;
}

wobble_t* SRC_read_wobble(
    const char *filepath
) {
    wobble_t *wobble = NULL;
    wobble           = (wobble_t*)malloc(sizeof(wobble_t));

    int state                     = READ_WOBBLE_IDLE;
    FILE *file                    = NULL;
    file                          = fopen(filepath, LEVEL_READ_MODE);
     
    int coef_n                    = 0;
    int sign                      = 0;
    int val                       = 0;

    state++;

    if (!file) {return NULL;}

    while((fread(buffer, DOUBLE_BYTE, PROPER_PACK_COUNT, file) == PROPER_PACK_COUNT)) {
        switch (state) 
        {
            case READ_WOBBLE_PREAMBULE_FIRST_HALF:
                if(IMP_cast_val_to_dec(buffer) != WOBBLE_PREAMBULE[FIRST_HALF]) { return NULL; }
                state++; break;

            case READ_WOBBLE_PREAMBULE_SECOND_HALF:
                if(IMP_cast_val_to_dec(buffer) != WOBBLE_PREAMBULE[SECOND_HALF]) { return NULL; }
                state++; break;

            case READ_WOBBLE_NUMBER:
                wobble->len = IMP_cast_val_to_dec(buffer);
                wobble->coefs = (float*)malloc(sizeof(float) * wobble->len);
                state++; break;

            case READ_WOBBLE_SIGN:
                val = IMP_cast_val_to_dec(buffer);
                sign = val==1 ? 1 : -1;
                state++; break;

            case READ_WOBBLE_COEF:
                val = IMP_cast_val_to_dec(buffer);
                wobble->coefs[coef_n] = (float)(sign * val) / 1000.0;

                if (++coef_n == wobble->len) {
                    state++; break;
                } else {
                    state = READ_WOBBLE_SIGN; break;
                }
        }
    }
    return wobble;
}

void SRC_free_wobble(
    wobble_t* wobble
) {
    free(wobble);
    wobble = NULL;
};

void SRC_free_lightsource(
    lightsource_t* source
) {
    free(source);
    source = NULL;
};

