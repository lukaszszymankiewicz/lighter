#include "global.h"
#include "game.h"
#include "segment.h"
#include "geometry.h"
#include "gfx.h"
#include "light.h"
#include "vertex.h"
#include "sprites.h"
#include "primitives.h"
#include "point.h"

#define RAY_THRESHOLD 2

// COLLECTIONS
lightsource_t* lightsources[ALL_AVAILABLE_LIGHTSOURCES];
texture_t*     gradient_textures[ALL_AVAILABLE_GRADIENT_TEXTURES];
wobble_t*      light_wobbles_patterns[ALL_AVAILABLE_WOBBLES_PATTERNS];

// this will be handled fully by native file-type. Just by now, all things related to this object is
// put into single function which it will be easy to put one file.
lightsource_t* LIG_init_lantern() {
    lightsource_t* lantern = NULL;
    lantern                = (lightsource_t*)malloc(sizeof(lightsource_t));

    lantern->width = 0.0;
    lantern->n_poly = 9;
    lantern->penetrating_power = 7;

    lantern->wobble[STANDING] = light_wobbles_patterns[WOBBLE_NO];
    lantern->wobble[WALKING] = light_wobbles_patterns[WOBBLE_NO];

    lantern->light_polygons = (lightpolygon_t*)malloc(sizeof(lightpolygon_t) * lantern->n_poly);

    lantern->light_polygons[0] = (lightpolygon_t){-10, -10, DEFAULT_LIGHT_R, DEFAULT_LIGHT_G, DEFAULT_LIGHT_B, 10, 0};
    lantern->light_polygons[1] = (lightpolygon_t){ 10, -10, DEFAULT_LIGHT_R, DEFAULT_LIGHT_G, DEFAULT_LIGHT_B, 10, 0};
    lantern->light_polygons[2] = (lightpolygon_t){-10, -10, DEFAULT_LIGHT_R, DEFAULT_LIGHT_G, DEFAULT_LIGHT_B, 10, 0};
    lantern->light_polygons[3] = (lightpolygon_t){-10,  10, DEFAULT_LIGHT_R, DEFAULT_LIGHT_G, DEFAULT_LIGHT_B, 10, 0};
    lantern->light_polygons[4] = (lightpolygon_t){-5 ,  -5, DEFAULT_LIGHT_R, DEFAULT_LIGHT_G, DEFAULT_LIGHT_B, 30, 0};
    lantern->light_polygons[5] = (lightpolygon_t){ 5 ,  -5, DEFAULT_LIGHT_R, DEFAULT_LIGHT_G, DEFAULT_LIGHT_B, 30, 0};
    lantern->light_polygons[6] = (lightpolygon_t){-5 ,  -5, DEFAULT_LIGHT_R, DEFAULT_LIGHT_G, DEFAULT_LIGHT_B, 30, 0};
    lantern->light_polygons[7] = (lightpolygon_t){-5 ,   5, DEFAULT_LIGHT_R, DEFAULT_LIGHT_G, DEFAULT_LIGHT_B, 30, 0};
    lantern->light_polygons[8] = (lightpolygon_t){ 0 ,   0, DEFAULT_LIGHT_R, DEFAULT_LIGHT_G, DEFAULT_LIGHT_B, 50, 0};

    lantern->gradient_texture = gradient_textures[GRADIENT_CIRCULAR];

    return lantern;
};

// put into single function which it will be easy to put one file.
lightsource_t* LIG_init_lighter() {
    lightsource_t* lighter = NULL;
    lighter = (lightsource_t*)malloc(sizeof(lightsource_t));

    lighter->width = PI / 7;
    lighter->n_poly = 3;

    lighter->wobble[STANDING] = light_wobbles_patterns[WOBBLE_STABLE];
    lighter->wobble[WALKING]  = light_wobbles_patterns[WOBBLE_WALK];

    lighter->penetrating_power = 7;

    lighter->light_polygons = (lightpolygon_t*)malloc(sizeof(lightpolygon_t) * lighter->n_poly);
    lighter->light_polygons[0] = (lightpolygon_t){0,  0, DEFAULT_LIGHT_R, DEFAULT_LIGHT_G, DEFAULT_LIGHT_B, 50, 0};
    lighter->light_polygons[1] = (lightpolygon_t){0,  0, DEFAULT_LIGHT_R, DEFAULT_LIGHT_G, DEFAULT_LIGHT_B, 30, 36};
    lighter->light_polygons[2] = (lightpolygon_t){0,  0, DEFAULT_LIGHT_R, DEFAULT_LIGHT_G, DEFAULT_LIGHT_B, 20, 72};

    lighter->gradient_texture = gradient_textures[GRADIENT_CIRCULAR];

    return lighter;
};

// this will be some kind of global file read
void LIG_init_all_lightsources() {
    lightsource_t* lighter = NULL;
    lightsource_t* lantern = NULL;

    lighter = LIG_init_lighter();
    lantern = LIG_init_lantern();

    lightsources[LIGHTER] = lighter;
    lightsources[LANTERN] = lantern;
}

wobble_t* LIG_init_no_wobble_pattern() {
    wobble_t *no_wobble = NULL;
    no_wobble = (wobble_t*)malloc(sizeof(wobble_t));

    no_wobble->len = 1;
    no_wobble->coefs = (float*)malloc(sizeof(float) * no_wobble->len);
    no_wobble->coefs[0] = 0.0;

    return no_wobble;
}

wobble_t* LIG_init_stable_wobble_pattern() {
    wobble_t *stable_wobble = NULL;
    stable_wobble = (wobble_t*)malloc(sizeof(wobble_t));

    stable_wobble->len = 82;
    stable_wobble->coefs = (float*)malloc(sizeof(float) * stable_wobble->len);
    // damn!
    stable_wobble->coefs[0] = 0.006;
    stable_wobble->coefs[1] = 0.006;
    stable_wobble->coefs[2] = 0.009;
    stable_wobble->coefs[3] = 0.009;
    stable_wobble->coefs[4] = 0.012;
    stable_wobble->coefs[5] = 0.012;
    stable_wobble->coefs[6] = 0.017;
    stable_wobble->coefs[7] = 0.017;
    stable_wobble->coefs[8] = 0.022;
    stable_wobble->coefs[9] = 0.022;
    stable_wobble->coefs[10] = 0.027;
    stable_wobble->coefs[11] = 0.027;
    stable_wobble->coefs[12] = 0.033;
    stable_wobble->coefs[13] = 0.033;
    stable_wobble->coefs[14] = 0.038;
    stable_wobble->coefs[15] = 0.038;
    stable_wobble->coefs[16] = 0.042;
    stable_wobble->coefs[17] = 0.042;
    stable_wobble->coefs[18] = 0.045;
    stable_wobble->coefs[19] = 0.046;
    stable_wobble->coefs[20] = 0.046;
    stable_wobble->coefs[21] = 0.045;
    stable_wobble->coefs[22] = 0.045;
    stable_wobble->coefs[23] = 0.042;
    stable_wobble->coefs[24] = 0.042;
    stable_wobble->coefs[25] = 0.038;
    stable_wobble->coefs[26] = 0.038;
    stable_wobble->coefs[27] = 0.033;
    stable_wobble->coefs[28] = 0.033;
    stable_wobble->coefs[29] = 0.027;
    stable_wobble->coefs[30] = 0.027;
    stable_wobble->coefs[31] = 0.022;
    stable_wobble->coefs[32] = 0.022;
    stable_wobble->coefs[33] = 0.017;
    stable_wobble->coefs[34] = 0.017;
    stable_wobble->coefs[35] = 0.012;
    stable_wobble->coefs[36] = 0.012;
    stable_wobble->coefs[37] = 0.009;
    stable_wobble->coefs[38] = 0.009;
    stable_wobble->coefs[39] = 0.0;
    stable_wobble->coefs[40] = 0.0;
    stable_wobble->coefs[41] = -0.006;
    stable_wobble->coefs[42] = -0.006;
    stable_wobble->coefs[43] = -0.009;
    stable_wobble->coefs[44] = -0.009;
    stable_wobble->coefs[45] = -0.012;
    stable_wobble->coefs[46] = -0.012;
    stable_wobble->coefs[47] = -0.017;
    stable_wobble->coefs[48] = -0.017;
    stable_wobble->coefs[49] = -0.022;
    stable_wobble->coefs[50] = -0.022;
    stable_wobble->coefs[51] = -0.027;
    stable_wobble->coefs[52] = -0.027;
    stable_wobble->coefs[53] = -0.033;
    stable_wobble->coefs[54] = -0.033;
    stable_wobble->coefs[55] = -0.038;
    stable_wobble->coefs[56] = -0.038;
    stable_wobble->coefs[57] = -0.042;
    stable_wobble->coefs[58] = -0.042;
    stable_wobble->coefs[59] = -0.045;
    stable_wobble->coefs[60] = -0.045;
    stable_wobble->coefs[61] = -0.046;
    stable_wobble->coefs[62] = -0.046;
    stable_wobble->coefs[63] = -0.045;
    stable_wobble->coefs[64] = -0.045;
    stable_wobble->coefs[65] = -0.042;
    stable_wobble->coefs[66] = -0.042;
    stable_wobble->coefs[67] = -0.038;
    stable_wobble->coefs[68] = -0.038;
    stable_wobble->coefs[69] = -0.033;
    stable_wobble->coefs[70] = -0.033;
    stable_wobble->coefs[71] = -0.027;
    stable_wobble->coefs[72] = -0.027;
    stable_wobble->coefs[73] = -0.022;
    stable_wobble->coefs[74] = -0.022;
    stable_wobble->coefs[75] = -0.017;
    stable_wobble->coefs[76] = -0.017;
    stable_wobble->coefs[77] = -0.012;
    stable_wobble->coefs[78] = -0.012;
    stable_wobble->coefs[79] = -0.009;
    stable_wobble->coefs[80] = -0.009;
    stable_wobble->coefs[81] = -0.009;

    return stable_wobble;
}

wobble_t* LIG_init_walk_wobble_pattern() {
    wobble_t *walk_wobble = NULL;
    walk_wobble  = (wobble_t*)malloc(sizeof(wobble_t));

    walk_wobble->len = 15;
    walk_wobble->coefs = (float*)malloc(sizeof(float) * 15);
    walk_wobble->coefs[0] = 0.012;
    walk_wobble->coefs[1] = 0.054;
    walk_wobble->coefs[2] = 0.082;
    walk_wobble->coefs[3] = 0.1;
    walk_wobble->coefs[4] = 0.082;
    walk_wobble->coefs[5] = 0.054;
    walk_wobble->coefs[6] = 0.012;
    walk_wobble->coefs[7] = 0.0;
    walk_wobble->coefs[8] = -0.012;
    walk_wobble->coefs[9] = -0.054;
    walk_wobble->coefs[10] = -0.082;
    walk_wobble->coefs[11] = -0.1;
    walk_wobble->coefs[12] = -0.082;
    walk_wobble->coefs[13] = -0.054;
    walk_wobble->coefs[14] = -0.012;

    return walk_wobble;
};

void LIG_init_all_light_wobbles_patterns() {
    wobble_t* no_wobble = NULL;
    wobble_t* stable_wobble = NULL;
    wobble_t* walk_wobble = NULL;

    no_wobble     = LIG_init_no_wobble_pattern();
    stable_wobble = LIG_init_stable_wobble_pattern();
    walk_wobble   = LIG_init_walk_wobble_pattern();

    light_wobbles_patterns[WOBBLE_NO] = no_wobble;
    light_wobbles_patterns[WOBBLE_STABLE] = stable_wobble;
    light_wobbles_patterns[WOBBLE_WALK] = walk_wobble;
}

texture_t* LIG_init_circular_gradient() {
    texture_t* circular_gradient = NULL;
    circular_gradient = GFX_read_texture("gradients/circular.png");

    return circular_gradient;
}

void LIG_init_all_gradients() {
    texture_t* circular_gradient = NULL;
    circular_gradient = LIG_init_circular_gradient();

    gradient_textures[GRADIENT_CIRCULAR] = circular_gradient;
}


// changes done to light angle if looking in different direction
float lightpos_up_down_corr[2][5] = {
  // LEFT  RIGHT  UP               DOWN             NONE
     {0,   0,     RIGHT_RAD+DEG30, RIGHT_RAD-DEG30, RIGHT_RAD},     // RIGHT 
     {0,   0,     LEFT_RAD-DEG30,  LEFT_RAD+DEG30,  LEFT_RAD}       // LEFT 
};

light_t *LIG_init() {

    light_t *light_o    = (light_t*)malloc(sizeof(light_t));
    light_o->src_num    = LIGHTER;
    light_o->src        = lightsources[light_o->src_num];
    light_o->angle      = LEFT_RAD+DEG30;

    return light_o;
}

// moves light angle due to wobble and/org looking up/down
void LIG_move_lightsource(
    light_t     *light_o,
    direction_t  light_dir,
    direction_t  hero_dir,
    int          frame
) {
    light_o->angle = lightpos_up_down_corr[hero_dir][light_dir];
}

// draws rays into every vertex of the lightpolygon
void LIG_debug_rays(
    vertex_t *light_poly,
    int       st_x,
    int       st_y,
    int       alpha
) {
    for(vertex_t* ptr = light_poly; ptr; ptr = ptr->next) {
        GFX_draw_colored_line(st_x, st_y, ptr->x, ptr->y, alpha, alpha, alpha, 255); 
    }
}

// draws obstacles of the light
void LIG_debug_obstacle(
    segment_t *obstacles
) {
    segment_t *ptr = NULL;
    ptr            = obstacles;

    while(ptr) {
        GFX_draw_colored_line(ptr->x1, ptr->y1, ptr->x2, ptr->y2, 255, 0, 0, 255); 
        ptr=ptr->next;
    }
}

// checks if ray intersects with obstacle
bool LIG_ray_intersect(
    int o_x1,  // obstacle
    int o_y1,  // obstacle
    int o_x2,  // obstacle
    int o_y2,  // obstacle
    int r_x1,  // ray
    int r_y1,  // ray
    int r_x2,  // ray
    int r_y2   // ray
) {
    // check for obstacle is collinear with ray - special case here needs to be applied
    if (o_x1 == o_x2) {
        // collinear
        if ((r_x1 == r_x2) && (r_x1 == o_x1)) {
            return GEO_collienar_segs_have_common_pt(o_y1, o_y2, r_y1, r_y2);
        }
        // typical case
        return (GEO_value_between_range(o_x1, r_x1, r_x2)) &&
            !GEO_pt_same_side(r_x1, r_y1, r_x2, r_y2, o_x1, o_y1, o_x2, o_y2);

    } else {
        // collinear
        if ((r_y1 == r_y2) && (r_y1 == o_y1)) {
            return GEO_collienar_segs_have_common_pt(o_x1, o_x2, r_x1, r_x2);
        }
        // typical case
        return (GEO_value_between_range(o_y1, r_y1, r_y2) && 
         !GEO_pt_same_side(r_x1, r_y1, r_x2, r_y2, o_x1, o_y1, o_x2, o_y2));
    }
}

// calculates all segments which ray hits
segment_t* LIG_ray_intersects_multiple(
    int r_x1,
    int r_y1,
    int r_x2,
    int r_y2,
    segment_t *obstacles
) { 
    segment_t  *seg           = NULL;
    segment_t  *inter         = NULL;

    for(seg=obstacles; seg; seg=seg->next) {
        if(LIG_ray_intersect(seg->x1, seg->y1, seg->x2, seg->y2, r_x1, r_y1, r_x2, r_y2)) {
            SEG_push(&inter, seg->x1, seg->y1, seg->x2, seg->y2);
        }
    }
    
    return inter;
}

// find specific segment type (horizontal or vertical) in corner (two segments)
segment_t* LIG_find_type_segment_in_corner(
    segment_t* first,
    segment_t* second,
    int type
) {
    if (first->type == type) {
        return first;
    } else if (second->type == type) {
        return second;
    } else {
        return NULL;
    }
}

// when casted ray "slips" of the corner of some obstacle new ray, slightly shifted from the main
// one needs to be casted (to see where and how light slips of such corner).
point_t* LIG_generate_slipover_hit_point(
    int x1,  // ray begginig
    int y1,  // ray begginig
    int x2,  // ray end 
    int y2   // ray end 
) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    
    // rays are perpendicular to screen axes
    if (dx==0) {
        return PT_new( x2, SIGN(y2-y1)*SCREEN_HEIGHT+1);
    } else if (dy==0) {
        return PT_new(SIGN(x2-x1)*SCREEN_WIDTH+1, y2);
    }

    // slipped ray, by now, ends only few pixels from slipped corner. Its end is simply transposed
    // in such a way, that it is always behind the screen border. This makes sure that such ray will
    // be checked for every obstacle in a way.
    // To calculate transposition 'nx' and 'ny' is calculated which is just a number of times ray
    // needs to be repeated to fall behind the level border.
    int nx = (int)(SCREEN_WIDTH / abs(dx)) + 1;
    int ny = (int)(SCREEN_HEIGHT / abs(dy)) + 1;

    int bigger = MAX(nx, ny);

    return PT_new(x1+bigger*dx, y1+bigger*dy);
}

// this function calculate if ray should "slip" if casted into corner.
point_t* LIG_generate_aux_hit_points(
    int x1,           // ray begginig 
    int y1,           // ray begginig
    int x2,           // ray end
    int y2,           // ray end
    segment_t *inter  // corner made from two obstacles (or one or four!)
) {
    segment_t* first  = NULL;
    segment_t* second = NULL;
    segment_t* ver    = NULL;
    segment_t* hor    = NULL;

    first = SEG_get(inter, 0);
    second = SEG_get(inter, 1);

    ver = LIG_find_type_segment_in_corner(first, second, VER);
    hor = LIG_find_type_segment_in_corner(first, second, HOR);

    // common point of corner to which ray is casted
    int common_x = SEG_common_x(first, second); 
    int common_y = SEG_common_y(first, second); 

    // corner range
    int max_x = (hor->x1 + hor->x2) - common_x;
    int max_y = (ver->y1 + ver->y2) - common_y;

    // checks in which direction slipopver ray should be casted. If upper is equal to lefter corner
    // is concave or conver from hero point of view, so no slip rays should be even considered
    bool upper = SIGN(common_y-max_y) == SIGN(y1-common_y);
    bool lefter = SIGN(common_x-max_x) == SIGN(x1-common_x);
    
    // TODO: in case this slipover rays will look fake, corr coef should be dynamicaly calculated,
    // not be a constant RAY_THRESHOLD value

    // slip in y axis
    if (lefter > upper) {
        int corr_x = SIGN(x1-x2) * RAY_THRESHOLD + x2;
        return LIG_generate_slipover_hit_point(x1, y1, corr_x, y2);
    }
    // slip in x axis
    else if (upper > lefter) {
        int corr_y = SIGN(y1-y2) * RAY_THRESHOLD + y2;
        return LIG_generate_slipover_hit_point(x1, y1, x2, corr_y);
    }

    // edge cases
    if (x1 - common_x == 0) {
        int corr_x = SIGN(common_x - max_x) * RAY_THRESHOLD + common_x;
        return LIG_generate_slipover_hit_point(x1, y1, corr_x, y2);
    } else if (y1 - common_y == 0) {
        int corr_y = SIGN(common_y - max_y) * RAY_THRESHOLD + common_y;
        return LIG_generate_slipover_hit_point(x1, y1, x2, corr_y);
    }

    // no aux points are generated
    return NULL;
}

segment_t* LIG_find_closest_hit_segment_ver(
    segment_t* inter,
    int r_x1,
    int r_y1,
    int r_x2,
    int r_y2
) {
    segment_t* ptr      = NULL;
    segment_t* best_seg = NULL;

    int best_dist = 9999;
    int dull_ray_end;
    int dist;
    
    for(ptr=inter; ptr; ptr=ptr->next) {
        if (ptr->type == VER) {
            dist = abs(r_x1-ptr->x1);

            // if distance between ray and obstacle is 0 it means that ray is either inside obstacle
            // or collinear with obstacle - some additional attention is needed
            if (dist == 0) {
                dull_ray_end = LIG_calculate_dull_ray_end(r_y1, r_y2, ptr->y1, ptr->y2);
                dist = r_y1 - dull_ray_end;
            }

            if (dist < best_dist) {
                best_dist = dist;
                best_seg = ptr;
            }
        }
    }

    return best_seg;
}

// if ray begins inside of obstacle or it is collinear to obstacle special care is taken to proerly
// calculate distance between such obstacle to such ray
int LIG_calculate_dull_ray_end(
    int r_beg,
    int r_end,
    int o_beg,
    int o_end
) {
    // ray begins inside of obstacle
    if (GEO_value_between_range(r_beg, o_beg, o_end)) {
        return r_beg;
    }
    // if distance between ray and obstacle is still zero it means that they are collinear - one of
    // the end of obstacle is and hit point
    int dir = SIGN(r_beg - r_end);

    if (dir == -1) {
        return MIN(o_beg, o_end);
    } else {
        return MAX(o_beg, o_end);
    }
}

segment_t* LIG_find_closest_hit_segment_hor(
    segment_t* inter,
    int r_x1,
    int r_y1,
    int r_x2,
    int r_y2
) {
    segment_t* ptr      = NULL;
    segment_t* best_seg = NULL;

    int best_dist = 9999;
    int dist;
    int dull_ray_end;

    for(ptr=inter; ptr; ptr=ptr->next) {
        if (ptr->type == HOR) {
            dist = abs(r_y1-ptr->y1);
            
            // if distance between ray and obstacle is 0 it means that ray is either inside obstacle
            // or collinear with obstacle - some additional attention is needed
            if (dist == 0) {
                dull_ray_end = LIG_calculate_dull_ray_end(r_x1, r_x2, ptr->x1, ptr->x2);
                dist = r_x1 - dull_ray_end;
            }

            if (dist < best_dist) {
                best_dist = dist;
                best_seg = ptr;
            }
        }
    }

    return best_seg;
}

vertex_t* LIG_calculate_ray_hit_vertex (
    int r_x1, int r_y1,
    int r_x2, int r_y2,
    segment_t* obs
) {
    int dist;
    int hit_x;
    int hit_y;
    float angle;

    vertex_t* v = NULL;

    if (obs->type == HOR) {
        dist = abs(r_y1 - obs->y1);
        hit_y = obs->y1;

        if (dist == 0) {
            int dull_ray_end = LIG_calculate_dull_ray_end(r_x1, r_x2, obs->x1, obs->x2);
            dist = abs(r_x1 - dull_ray_end);
            hit_x = dull_ray_end;
        } else {
            // TODO: make round check here!
            hit_x = GEO_intersection_with_y(obs->y1, r_x1, r_y1, r_x2, r_y2);
            dist = (int)GEO_line_segment_len(r_x1, r_y1, hit_x, hit_y);
        }
    } else {
        dist = abs(r_x1 - obs->x1);
        hit_x = obs->x1;

        if (dist == 0) {
            int dull_ray_end = LIG_calculate_dull_ray_end(r_y1, r_y2, obs->y1, obs->y2);
            dist = abs(r_y1 - dull_ray_end);
            hit_y = dull_ray_end;
        } else {

            // TODO: make round check here!
            hit_y = GEO_intersection_with_x(obs->x1, r_x1, r_y1, r_x2, r_y2);
            dist = (int)GEO_line_segment_len(r_x1, r_y1, hit_x, hit_y);
        }
    }

    angle = GEO_angle_2pt(r_x1, r_y1, r_x2, r_y2);
    v = VRTX_new(hit_x, hit_y, angle);
    v->dist = dist;

    return v;
}

vertex_t* LIG_find_closest_hit(
    segment_t* inter,
    int r_x1, int r_y1,
    int r_x2, int r_y2
) {
    segment_t* best_ver = NULL;
    segment_t* best_hor = NULL;
    vertex_t* v_hor     = NULL;
    vertex_t* v_ver     = NULL;

    // find vertical and horizontal segments which is closest to ray
    best_hor = LIG_find_closest_hit_segment_hor(inter, r_x1, r_y1, r_x2, r_y2);
    best_ver = LIG_find_closest_hit_segment_ver(inter, r_x1, r_y1, r_x2, r_y2);
    
    // no hit is found, it is unexpected as inter paramter should contain only hitted segments and
    // should not be NULL, nevertheless better to have such check
    if (!best_ver && !best_hor) {
        return NULL;
    }

    // there is only one hitted segment, it could be either vertical or horizontal one
    if (!best_ver) {
        return LIG_calculate_ray_hit_vertex(r_x1, r_y1, r_x2, r_y2, best_hor);
    }
    if (!best_hor) {
        return LIG_calculate_ray_hit_vertex(r_x1, r_y1, r_x2, r_y2, best_ver);
    }

    // there are both vertical and horizontal segments hitted - best of each of these group is taken
    // and check id done to determine which is hit closer
    v_hor = LIG_calculate_ray_hit_vertex(r_x1, r_y1, r_x2, r_y2, best_hor);
    v_ver = LIG_calculate_ray_hit_vertex(r_x1, r_y1, r_x2, r_y2, best_ver);
    
    // return vertical hitted segment
    if (v_ver->dist > v_hor->dist) {
        VRTX_free(v_ver);
        return v_hor;
    // return horizontal hitted segment
    } else {
        VRTX_free(v_hor);
        return v_ver;
    }
}

// Calculates coords of light polygon vertices.
vertex_t* LIG_get_base_light_polygon(
    int x,                 // x starting point
    int y,                 // y starting point
    segment_t *obstacles,  // light obstacles
    point_t* hit_points    // hit points to be checked
) { 
    vertex_t   *light_polygon     = NULL;
    vertex_t   *v                 = NULL;
    segment_t  *inter             = NULL;
    point_t    *ptr               = NULL;
    point_t    *aux_pts           = NULL;  
    point_t    *aux_pt            = NULL;

    float       pt_angle;

    for(ptr=hit_points; ptr; ptr=ptr->next) {
        inter = LIG_ray_intersects_multiple(x, y, ptr->x, ptr->y, obstacles);
        
        // ray does not hit anything, only check if light can slip through corner which this segment
        // make
        if (SEG_contains(inter, ptr->x, ptr->y)) {
            // slip rays are calculatd
            aux_pt = LIG_generate_aux_hit_points(x, y, ptr->x, ptr->y, inter);
            PT_merge(&aux_pts, aux_pt);

            // ray end if not interrupted should be added to light polygon
            pt_angle = GEO_angle_2pt(x, y, ptr->x, ptr->y);
            VRTX_add_point(&light_polygon, ptr->x, ptr->y, pt_angle);

        // ray hits SOMETHING - nothing happens. Calculating hit point is needed only for slip rays
        } else { }
    }

    // slip rays MUST hit something, its end is generated to always range outisde level. Because
    // level is rounded with dummy obstacles lining the border of screen, at list hit of it needs to
    // be calculated.
    ptr = NULL;
    v = NULL;

    for (ptr=aux_pts; ptr; ptr=ptr->next) {
        inter = NULL;
        inter = LIG_ray_intersects_multiple(x, y, ptr->x, ptr->y, obstacles);

        v = LIG_find_closest_hit(inter, x, y, ptr->x, ptr->y);
        VRTX_push(&light_polygon, v);
    }

    SEG_free(inter);

    return light_polygon;
};

// adds initial point of light to light polygon
vertex_t* LIG_initial_point_of_light(
    int x,                   // hero position 
    int y,                   // hero position
    float width              // width (in radians) of light cone
) {
    if (width == 0.0) {
        return NULL;
    }

    return VRTX_new(x, y, 0.0);
}

// adds light border to light cone (if light sweeps all around function is not used)
point_t* LIG_add_border_light_vertices(
    int x,                   // hero position 
    int y,                   // hero position
    segment_t* obstacles,    // light obstacles
    float angle,             // angle in which light is pointed
    float width              // width (in radians) of light cone
) {
    if (width == 0.0) {
        return NULL;
    }

    int r = 10;

    point_t  *aux_pt_a            = NULL;  
    point_t  *aux_pt_b            = NULL;  
    point_t  *hit_points          = NULL;  

    // edges of light light cone needs to checked for collision
    int x_a = (int)(x - sin(angle - width) * r);
    int y_a = (int)(y - cos(angle - width) * r);
    int x_b = (int)(x - sin(angle + width) * r);
    int y_b = (int)(y - cos(angle + width) * r);
    
    aux_pt_a = LIG_generate_slipover_hit_point(x, y, x_a, y_a);
    aux_pt_b = LIG_generate_slipover_hit_point(x, y, x_b, y_b);

    PT_merge(&hit_points, aux_pt_a);
    PT_merge(&hit_points, aux_pt_b);

    return hit_points;
}

// Calculates polygon of shadow which will be rendered on walls (light penetating walls).
vertex_t* LIG_calc_light_wall_shadow(
    vertex_t *light_polygon,      // light polygon shape
    int       penetrating_power,  // range of light penetating wall (in px)
    int       x,                  // x starting point (hero)
    int       y                   // y starting point (hero)
) { 
    vertex_t *shadow_polygon = NULL;  
    vertex_t *ptr            = NULL;
    ptr                      = light_polygon;

    while(ptr) {
        if ((ptr->x == x) && (ptr->y == y)) {
            // starting point (hero position) should not be transfomred. This allows to avoid
            // checking if light has any width (if so this point will occur, if not won`t).
            VRTX_add_point(&shadow_polygon, x, y, 0);
        }
        else {
            VRTX_add_point(
                &shadow_polygon,
                ptr->x - sin(ptr->angle) * penetrating_power,
                ptr->y - cos(ptr->angle) * penetrating_power,
                ptr->angle
            );
        }
        ptr=ptr->next;
    }
    return shadow_polygon;
};

// changes hero lightsource to another one
void LIG_change_source(
    light_t* lght
) {
    lght->src_num = ((lght->src_num) + 1) % ALL;
    lght->src = lightsources[lght->src_num];
}

int LIG_get_light_penetrating_power(
    light_t *lght
) {
    return lght->src->penetrating_power;
}

int LIG_get_light_polygon_x_corr(
    light_t *lght,
    int      i
) {
    return sin(lght->angle) * lght->src->light_polygons[i].x + cos(lght->angle) * lght->src->light_polygons[i].y;
}

int LIG_get_light_polygon_y_corr(
    light_t *lght,
    int      i 
) {
    return sin(lght->angle) * lght->src->light_polygons[i].y + cos(lght->angle) * lght->src->light_polygons[i].x;
}

float LIG_get_light_polygon_width_corr(
    light_t *lght,
    int      i
) {
    int coef = lght->src->light_polygons[i].width;

    if (coef){
        return PI / coef;
    }
    return 0.0;
}

float LIG_get_wobble_angle_coef(
    light_t *lght,
    int      x_vel,
    int      frame
) {
    int state = 0;

    // TODO: state checking should be in separate function
    if (x_vel != 0) {
        state=1;
    }
    wobble_t *current_wobble = lght->src->wobble[state];
    return current_wobble->coefs[frame%current_wobble->len];
}

// generate list of points which needs to be check for ray casting
point_t* LIG_generate_hit_points(
    int x,
    int y,
    float width,
    float angle,
    segment_t* obstacles
) {
    segment_t *ptr        = NULL;
    point_t *hit_points   = NULL;
    ptr                   = obstacles;

    // if light source has no width, every point from obstacles is taken
    if (width == 0.0) {
        while(ptr) {
            if (!(PT_contains(hit_points, ptr->x1, ptr->y1))) { 
                    PT_push(&hit_points, ptr->x1, ptr->y1);
            }
            if (!(PT_contains(hit_points, ptr->x2, ptr->y2))) { 
                    PT_push(&hit_points, ptr->x2, ptr->y2);
            } 
            ptr = ptr->next;
        }
        return hit_points;
    }

    int r = 2000;

    // edges of light light cone needs to checked for collision
    int x_a = (int)(x - sin(angle - width) * r);
    int y_a = (int)(y - cos(angle - width) * r);
    int x_b = (int)(x - sin(angle + width) * r);
    int y_b = (int)(y - cos(angle + width) * r);
    
    bool res;

    // end each of the obstacle end is checked whether it occurs in light cone.
    while(ptr) {
        res = GEO_pt_in_triangle(ptr->x1, ptr->y1, x, y, x_a, y_a, x_b, y_b);

        if (res && !(PT_contains(hit_points, ptr->x1, ptr->y1))) { 
            PT_push(&hit_points, ptr->x1, ptr->y1);
        }

        res = GEO_pt_in_triangle(ptr->x2, ptr->y2, x, y, x_a, y_a, x_b, y_b);

        if (res && !(PT_contains(hit_points, ptr->x2, ptr->y2))) { 
            PT_push(&hit_points, ptr->x2, ptr->y2);
        }

        ptr = ptr->next;
    }

    return hit_points;
}

// calculates shape of light polygon (width of light cone is calculated on a fly)
vertex_t* LIG_get_light_polygon(
    int        x,
    int        y,
    float      width,
    float      angle,
    segment_t* obstacles
) {
    vertex_t* light_polygon     = NULL;
    vertex_t* aux_vertices      = NULL;
    vertex_t* starting_point    = NULL;

    point_t* hit_points         = NULL;
    point_t* aux_hit_points     = NULL;
    point_t* ptr                = NULL;

    segment_t* inter            = NULL;

    // calculating points to which light rays is casted
    hit_points = LIG_generate_hit_points(x, y, width, angle, obstacles);

    // calculating light polygon shape
    light_polygon = LIG_get_base_light_polygon(x, y, obstacles, hit_points);

    // calculating aux hitpoints of light polygon
    aux_hit_points = LIG_add_border_light_vertices(x, y, obstacles, angle, width);

    for (ptr=aux_hit_points; ptr; ptr=ptr->next) {
        inter = NULL;
        inter = LIG_ray_intersects_multiple(x, y, ptr->x, ptr->y, obstacles);
        aux_vertices = LIG_find_closest_hit(inter, x, y, ptr->x, ptr->y);
        VRTX_merge(&light_polygon, aux_vertices);
    }

    starting_point = LIG_initial_point_of_light(x, y, width);
    VRTX_merge(&light_polygon, starting_point);

    PT_free(hit_points);
    PT_free(aux_hit_points);

    SEG_free(inter);

    return light_polygon;
}


// Calculates and draws light polygons. Every of the light source needs several polygons to be drawn
// - every one of them is slightly moved to another which makes light looks more "natural".
// Furthermore, the polygons with bigger shift has more pale color resulting in overall effect
// looking like "gradient".
void LIG_fill_lightbuffer(
    int        x,
    int        y,
    int        frame,
    light_t   *light,
    segment_t *obstacles,
    int        x_vel
) {
    int   i;                       // index of current light polygon drawn
    int   red;                     // color of current light polygon drawn
    int   green;                   // color of current light polygon drawn
    int   blue;                    // color of current light polygon drawn
    int   light_power;             // power of light (brightness)
    int   penetrating_power;       // ability to penetrate walls by light
    int   x_corr;                  // x and y correction values 
    int   y_corr;                  // x and y correction values
    float width_corr;              // light width correction (some light polygons can be wider)
    float wobble_corr = 0.0;       // angle correction due to wobling

    // before adding any new light to scene, cleaning of lighbuffers is needed
    GFX_clean_buffers();

    vertex_t* light_polygon     = NULL;
    vertex_t* light_wall_shadow = NULL;

    wobble_corr                 = LIG_get_wobble_angle_coef(light, x_vel, frame);

    for (i=0; i < light->src->n_poly; i++) {
        red              = light->src->light_polygons[i].red;
        green            = light->src->light_polygons[i].green;
        blue             = light->src->light_polygons[i].blue;
        light_power      = light->src->light_polygons[i].light_power;

        penetrating_power = light->src->penetrating_power;
        x_corr            = LIG_get_light_polygon_x_corr(light, i);
        y_corr            = LIG_get_light_polygon_y_corr(light, i);
        width_corr        = LIG_get_light_polygon_width_corr(light, i);

        light_polygon = LIG_get_light_polygon(
            x+x_corr,
            y+y_corr,
            light->src->width+width_corr,
            light->angle+wobble_corr,
            obstacles
        );
        
        light_wall_shadow = LIG_calc_light_wall_shadow(light_polygon, penetrating_power, x, y);
        
        // fill lightbuffer with freshly calulcated light polygon
        GFX_fill_buffer_single_polygon(light_polygon, GFX_fill_lightbuffer, red, green, blue, light_power);

        // fill lightbuffer with freshly calulcated light polygon
        GFX_fill_buffer_single_polygon(light_wall_shadow, GFX_fill_mesh_shadowbuffer, red, green, blue, 20);

        // cleaning
        VRTX_free(light_polygon);
    }

    // LIG_debug_obstacle(obstacles);
    SEG_free(obstacles);
};

void LIG_read_all_files() {
    LIG_init_all_light_wobbles_patterns();
    LIG_init_all_gradients();
    LIG_init_all_lightsources();
}

void LIG_free_all_files() {
    for (int i=0; i<ALL_AVAILABLE_LIGHTSOURCES; i++) {
        free(lightsources[i]->light_polygons);
        free(lightsources[i]);
    }

    for (int i=0; i<ALL_AVAILABLE_WOBBLES_PATTERNS; i++) {
        free(light_wobbles_patterns[i]->coefs);
        free(light_wobbles_patterns[i]);
    }

    for (int i=0; i<ALL_AVAILABLE_GRADIENT_TEXTURES; i++) {
        GFX_free_texture(gradient_textures[GRADIENT_CIRCULAR]);
    }
}

void LIG_free(
    light_t* lght
) {
    free(lght);
};
