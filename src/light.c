#include "./data/library.h"

#include "geometry.h"
#include "global.h"
#include "light.h"
#include "point.h"
#include "segment.h"
#include "source.h"
#include "scene.h"
#include "vertex.h"

#include <stdio.h>

#define LIGHT_RENDER_COUNT        2

// TODO: replace it with function in geometry.h
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

vertex_t* LIG_add_aux_hit_points(
    int        x,
    int        y,
    point_t*   aux_hit_points,
    vertex_t*  light_polygon,
    segment_t* obstacles
) {
    point_t* ptr            = NULL;
    segment_t* inter        = NULL;
    vertex_t* aux_vertices  = NULL;

    for (ptr=aux_hit_points; ptr; ptr=ptr->next) {
        inter = NULL;
        inter = LIG_ray_intersects_multiple(x, y, ptr->x, ptr->y, obstacles);
        aux_vertices = LIG_find_closest_hit(inter, x, y, ptr->x, ptr->y);
        VRTX_merge(&light_polygon, aux_vertices);

        if (aux_vertices) {VRTX_free(aux_vertices);}
        aux_vertices = NULL;

        if (inter) {SEG_free(inter); }
        inter = NULL;
    }

    return light_polygon;
}

// Calculates coords of light polygon vertices.
vertex_t* LIG_get_base_light_polygon(
    int x,                 // x starting point
    int y,                 // y starting point
    segment_t *obstacles,  // light obstacles
    point_t* hit_points    // hit points to be checked
) { 
    vertex_t   *light_polygon     = NULL;
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
            // PT_free(aux_pt);

            // ray end if not interrupted should be added to light polygon
            pt_angle = GEO_angle_2pt(x, y, ptr->x, ptr->y);
            VRTX_add_point(&light_polygon, ptr->x, ptr->y, pt_angle);

            SEG_free(inter);
            inter = NULL;

        // ray hits SOMETHING - nothing happens. Calculating hit point is needed only for slip rays
        } else {

            SEG_free(inter);
            inter = NULL;
        }

        SEG_free(inter);
        inter = NULL;
    }

    // slip rays MUST hit something, its end is generated to always range outisde level. Because
    // level is rounded with dummy obstacles lining the border of screen, at list hit of it needs to
    // be calculated.
    LIG_add_aux_hit_points(x, y, aux_pts, light_polygon, obstacles);

    if (inter) { SEG_free(inter); }
    if (aux_pts) { PT_free(aux_pts); }

    return light_polygon;
};

// adds initial point of light to light polygon
vertex_t* LIG_initial_point_of_light(
    int       x,                 
    int       y,                
    float     width,
    vertex_t* light_polygon
) {
    vertex_t* v = NULL;

    if (width != 0.0) {
        v = VRTX_new(x, y, 0.0);
        VRTX_merge(&light_polygon, v);
        
        if (v) { VRTX_free(v); }
    }

    return light_polygon;
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

    point_t  *hit_points          = NULL;  

    // edges of light light cone needs to checked for collision
    int x_a = (int)(x - sin(angle - width) * R);
    int y_a = (int)(y - cos(angle - width) * R);
    int x_b = (int)(x - sin(angle + width) * R);
    int y_b = (int)(y - cos(angle + width) * R);
    
    PT_push(&hit_points, x_a, y_a);
    PT_push(&hit_points, x_b, y_b);

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

    // edges of light light cone needs to checked for collision
    int x_a = (int)(x - sin(angle - width) * R);
    int y_a = (int)(y - cos(angle - width) * R);
    int x_b = (int)(x - sin(angle + width) * R);
    int y_b = (int)(y - cos(angle + width) * R);
    
    bool res;

    // each of the obstacle end is checked whether it occurs in light cone.
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
    vertex_t*  light_polygon    = NULL;
    point_t*   hit_points       = NULL;
    point_t*   aux_hit_points   = NULL;
    segment_t* inter            = NULL;

    // calculating points to which light rays is casted
    hit_points = LIG_generate_hit_points(x, y, width, angle, obstacles);

    // calculating light polygon shape
    light_polygon = LIG_get_base_light_polygon(x, y, obstacles, hit_points);

    // calculating aux hitpoints of light polygon
    aux_hit_points = LIG_add_border_light_vertices(x, y, obstacles, angle, width);
    
    light_polygon = LIG_add_aux_hit_points(x, y, aux_hit_points, light_polygon, obstacles);
    light_polygon = LIG_initial_point_of_light(x, y, width, light_polygon);

    PT_free(hit_points);
    PT_free(aux_hit_points);
    SEG_free(inter);

    return light_polygon;
}

vertex_t* LIG_calc_polygon(
    int              x,
    int              y,
    int              n,
    float            angle,
    float            coef,
    lightsource_t   *light,
    segment_t       *obstacles
) {
    int   x_corr;                  // x and y correction values 
    int   y_corr;                  // x and y correction values
    float width;                   // light width
    float width_corr;              // light width correction (some light polygons can be wider)

    vertex_t* light_polygon     = NULL;
    x_corr                      = SRC_get_light_polygon_x_corr(light, angle, n);
    y_corr                      = SRC_get_light_polygon_y_corr(light, angle, n);
    width                       = light->width;
    width_corr                  = SRC_get_light_polygon_width_corr(light, n);

    light_polygon = LIG_get_light_polygon(
        x+x_corr,
        y+y_corr,
        width+width_corr,
        angle+coef,
        obstacles
    );

    return light_polygon;
};

void LIG_draw_polygon(
    int              x,
    int              y,
    int              i,
    float            angle,
    float            coef,
    lightsource_t   *light,
    segment_t       *obstacles
) {
    vertex_t *vertex   = NULL;
    float    *coords   = NULL;
    int       n_coords = 0;

    vertex       = LIG_calc_polygon(x, y, i, angle, coef, light, obstacles);
    n_coords     = VRTX_len(vertex) * LIGHT_RENDER_COUNT;
    coords       = VRTX_to_coords(vertex);

    SCENE_draw_polygon(
        coords,
        n_coords,
        light->light_polygons[i].red,
        light->light_polygons[i].green,
        light->light_polygons[i].blue,
        light->light_polygons[i].light_power
    );
}
