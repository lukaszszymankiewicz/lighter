#include "config.h"
#include "geometry.h"
#include "gfx.h"
#include "light.h"
#include "vertex.h"
#include "sprites.h"
#include "primitives.h"
#include "obstacle.h"
#include "point.h"
#include "macros.h"

lightsource_t lantern = {
    .width = 0.0,
    .n_poly = 9,
    .wobble_corr = 0.0,
    .wobble_change_dir_coef = 1,
    .polys = {
        {-10, -10, DEFAULT_LIGHT_R, DEFAULT_LIGHT_G, DEFAULT_LIGHT_B, 10, 0 },
        { 10, -10, DEFAULT_LIGHT_R, DEFAULT_LIGHT_G, DEFAULT_LIGHT_B, 10, 0 },
        {-10, -10, DEFAULT_LIGHT_R, DEFAULT_LIGHT_G, DEFAULT_LIGHT_B, 10, 0 },
        {-10,  10, DEFAULT_LIGHT_R, DEFAULT_LIGHT_G, DEFAULT_LIGHT_B, 10, 0 },
        {-5 ,  -5, DEFAULT_LIGHT_R, DEFAULT_LIGHT_G, DEFAULT_LIGHT_B, 30, 0 },
        { 5 ,  -5, DEFAULT_LIGHT_R, DEFAULT_LIGHT_G, DEFAULT_LIGHT_B, 30, 0 },
        {-5 ,  -5, DEFAULT_LIGHT_R, DEFAULT_LIGHT_G, DEFAULT_LIGHT_B, 30, 0 },
        {-5 ,   5, DEFAULT_LIGHT_R, DEFAULT_LIGHT_G, DEFAULT_LIGHT_B, 30, 0 },
        { 0 ,   0, DEFAULT_LIGHT_R, DEFAULT_LIGHT_G, DEFAULT_LIGHT_B, 50, 0 }
    },
};

lightsource_t lighter = {
    .width = PI / 7,
    .n_poly = 6,
    .wobble_corr = 0.001,
    .wobble_change_dir_coef = 30,
    .polys = {
        {  0,  0, DEFAULT_LIGHT_R, DEFAULT_LIGHT_G, DEFAULT_LIGHT_B, 90, 0  },
        {  4,  0, DEFAULT_LIGHT_R, DEFAULT_LIGHT_G, DEFAULT_LIGHT_B, 50, 0  },
        { -4,  0, DEFAULT_LIGHT_R, DEFAULT_LIGHT_G, DEFAULT_LIGHT_B, 50, 0  },
        {  0,  4, DEFAULT_LIGHT_R, DEFAULT_LIGHT_G, DEFAULT_LIGHT_B, 30, 0  },
        {  0, -4, DEFAULT_LIGHT_R, DEFAULT_LIGHT_G, DEFAULT_LIGHT_B, 30, 0  },
        {  0,  0, DEFAULT_LIGHT_R, DEFAULT_LIGHT_G, DEFAULT_LIGHT_B, 20, 36 },
    },
};

lightsource_t* lightsources[] = {&lighter, &lantern};

float lightpos_up_down_corr[2][5] = {
  // LEFT  RIGHT  UP               DOWN             NONE
     {0,   0,     RIGHT_RAD+DEG30, RIGHT_RAD-DEG30, RIGHT_RAD},     // RIGHT 
     {0,   0,     LEFT_RAD-DEG30,  LEFT_RAD+DEG30,  LEFT_RAD}       // LEFT 
};

light_t *LIG_init() {
    light_t* light_o    = (light_t*)malloc(sizeof(light_t));
    light_o->src_num    = LIGHTER;
    light_o->src        = lightsources[light_o->src_num];
    light_o->angle      = LEFT_RAD;
    light_o->wobble_dir = WOBBLE_DOWN;
    light_o->wobble     = 0;

    return light_o;
}

// calculates small angle correction to make light effect be more realistic. Charachteristic
// "wobbling" effect is achieved while changing angle in small amount slowly.
void LIG_wobble_angle(
    light_t *lght,
    int      frame
) {
    if ((frame % lght->src->wobble_change_dir_coef) == 0) {
        lght->wobble_dir = (-1) * lght->wobble_dir;
    }

    lght->wobble += lght->src->wobble_corr * lght->wobble_dir;
}

void LIG_move_lightsource(
    light_t     *light_o,
    direction_t  light_dir,
    direction_t  hero_dir,
    int          frame
) {
    LIG_wobble_angle(light_o, frame);
    light_o->angle = lightpos_up_down_corr[hero_dir][light_dir] + light_o->wobble;
}

// DEBUG function
// draws rays into every cone of the lightpolygon
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

// DEBUG function
// draws rays into every cone of the lightpolygon
void LIG_debug_obstacle_ends(
    int       st_x,
    int       st_y,
    obstacle_t *obstacles
) {
    obstacle_t* ptr = obstacles;
    
    while(ptr) {
        GFX_draw_colored_line(st_x, st_y, ptr->x1, ptr->y1, 69, 69, 69, 255); 
        GFX_draw_colored_line(st_x, st_y, ptr->x2, ptr->y2, 150, 150, 150, 255); 
        ptr = ptr->next;
    }
}

// DEBUG function
// draws rays into every cone of the lightpolygon
void LIG_debug_obstacle(
    obstacle_t *obstacles
) {
    obstacle_t* ptr = NULL;
    ptr = obstacles;

    while(ptr) {
        GFX_draw_colored_line(ptr->x1, ptr->y1, ptr->x2, ptr->y2, 255, 0, 0, 255); 
        ptr=ptr->next;
    }
}

// draws edges of light polygon
void LIG_debug_dark_sectors(
    vertex_t *light_poly
) {
    int first_x   = light_poly->x;
    int first_y   = light_poly->y;
    vertex_t* ptr = light_poly;

    for(; ptr->next; ptr = ptr->next) {
        GFX_draw_colored_line(ptr->x, ptr->y, ptr->next->x, ptr->next->y, 255, 255, 255, 255); 
    }
    GFX_draw_colored_line(ptr->x, ptr->y, first_x, first_y, 255, 255, 255, 255);
}

point_t* LIG_ray_hits_obstacle (
    int x1,
    int y1,
    int x2,
    int y2,
    obstacle_t *obstacle
) {
    point_t *base_point = PT_new(x2, y2);
    int      new_value;

    // obstacle is vertical
    if (obstacle->x1 == obstacle->x2) {
        if (GEO_value_between_range(obstacle->x1, x1, x2)) {
            new_value = GEO_intersection_with_x(obstacle->x1, x1, y1, x2, y2);

            if (GEO_value_between_range(new_value, obstacle->y1, obstacle->y2)) {
                base_point->y = new_value;
                base_point->x = obstacle->x1;
            }
        }
    }

    // obstacle is horizontal
    else {
        if (GEO_value_between_range(obstacle->y1, y1, y2)) {
            new_value = GEO_intersection_with_y(obstacle->y1, x1, y1, x2, y2);

            if (GEO_value_between_range(new_value, obstacle->x1, obstacle->x2)) {
                base_point->x = new_value;
                base_point->y = obstacle->y1;
            }
        }
    }

    return base_point;

}

// Function checks for any of the possible interseciton between single ray and set of segments
// (obstacles). Best (closest) intersection is then stored in ray segment end coordinates.
point_t* LIG_closest_intersection_with_obstacle(
    int x1,
    int y1,
    int x2,
    int y2,
    obstacle_t *obstacles
)
{ 
    point_t    *best_point = NULL;
    obstacle_t *obstacle   = NULL;
    
    best_point = PT_new(x2, y2);    

    for(obstacle=obstacles; obstacle; obstacle=obstacle->next) {
        best_point = LIG_ray_hits_obstacle(x1, y1, best_point->x, best_point->y, obstacle);
    }

    return best_point;
}

// checks if segment hits any of the obstacle
bool LIG_any_intersection_with_obstacle(
    int x1,
    int y1,
    int x2,
    int y2,
    obstacle_t *obstacles
)
{ 
    point_t    *best_point = NULL;
    obstacle_t *obstacle   = NULL;

    for(obstacle=obstacles; obstacle; obstacle=obstacle->next) {
        best_point = LIG_ray_hits_obstacle(x1, y1, x2, y2, obstacle);

        if (best_point->x != x2 || best_point->y != y2) {
            return true;
        }
    }

    return false;
}

// we need to filter those level obstacles for which we will cast rays - points which is
// inside triangle made from two border rays and player position is taken.
obstacle_t* LIG_get_visible_obstacles(
    obstacle_t *obstacles,
    int         y,
    int         x,
    float       angle,
    float       width 
) {
    obstacle_t* filtered_obstacles = NULL;
    obstacle_t* s = NULL;
    s = obstacles;

    int a_border_x = (int)x - sin(angle - width) * R;
    int a_border_y = (int)y - cos(angle - width) * R;
    int b_border_x = (int)x - sin(angle + width) * R;
    int b_border_y = (int)y - cos(angle + width) * R;

    while(s) { 
        if (GEO_pt_in_triangle(
                s->x1,      s->y1,
                x,          y,
                a_border_x, a_border_y,
                b_border_x, b_border_y
            )
            ||
            GEO_pt_in_triangle(
                s->x2,      s->y2,
                x,          y,
                a_border_x, a_border_y,
                b_border_x, b_border_y
            )) {

            OBS_push(&filtered_obstacles, s->x1, s->y1, s->x2, s->y2);
        }
        s = s->next;
    }

    return filtered_obstacles;
}

obstacle_t* LIG_filter_obstacles_by_sight(
    int x,
    int y,
    float angle,
    float width,
    obstacle_t *obstacles
) {
    obstacle_t* filtered_obstacles = NULL;

    if (width != 0.0) {
        filtered_obstacles = LIG_get_visible_obstacles(obstacles, x, y, angle, width);
    }

    else {
        filtered_obstacles = obstacles; 
    }

    return filtered_obstacles;
}

point_t* LIG_calc_hit_points(
    int         x,
    int         y,
    float       angle,
    float       width,
    obstacle_t *obstacles
) { 
    point_t    *hit_points = NULL;
    point_t    *pt_a       = NULL;
    point_t    *pt_b       = NULL;
    obstacle_t *ptr        = NULL;
    ptr                    = obstacles;

    if (width != 0.0) {
        int a_border_x = (int)(x - sin(angle - width) * R);
        int a_border_y = (int)(y - cos(angle - width) * R);
        int b_border_x = (int)(x - sin(angle + width) * R);
        int b_border_y = (int)(y - cos(angle + width) * R);
        
        pt_a = LIG_closest_intersection_with_obstacle(x, y, a_border_x, a_border_y, obstacles);
        pt_b = LIG_closest_intersection_with_obstacle(x, y, b_border_x, b_border_y, obstacles);
        
        PT_push(
                &hit_points,
                pt_a->x,
                pt_a->y
            );
        PT_push(
                &hit_points,
                pt_b->x,
                pt_b->y
            );

        // DEBUG
        // GFX_draw_colored_line(x, y, pt_a->x, pt_a->y, 200, 200, 200, 200);
        // GFX_draw_colored_line(x, y, pt_b->x, pt_b->y, 200, 200, 200, 200);

        while(ptr) { 
            if (GEO_pt_in_triangle(ptr->x1, ptr->y1, x, y, a_border_x, a_border_y, b_border_x, b_border_y)) {
                PT_push(&hit_points, ptr->x1, ptr->y1);
            }
            if (GEO_pt_in_triangle(ptr->x2, ptr->y2, x, y, a_border_x, a_border_y, b_border_x, b_border_y)) {
                PT_push(&hit_points, ptr->x2, ptr->y2);
            }
            ptr = ptr->next;
        }
        free(pt_a);
        free(pt_b);
    }
    
    else {
        while(ptr) {
            PT_push(&hit_points, ptr->x1, ptr->y1);
            PT_push(&hit_points, ptr->x2, ptr->y2);
            ptr=ptr->next;
        }
    }

    return hit_points;
}

// Light sweeps ray all around (full circles). Some of the lightsources cuts this circle leaving
// only part of the circle left (eg.: lighter). Light collides with each wall - point where rays are
// colliding needs to be calculated. For each of obstacle three rays are casted (one directly into
// obstacle point, one a little bit shifted to left, and last one shifted a little bit to right).
// This allows to lower number of rays to only these necessary. Each ray is than checked for
// collision, and shortest intersection is used for drawing such ray. Each point of intersection
// found is than put in sorted order (by angle it makes with player position) - such operations
// automatically creates points of polygon which is never intersecting with itself (which is totally
// fine, as we`re drawing light source originated in one point, such polygon cannot have any
// intersecting segments). After calculating all of these points, some of than can be eleminated, as
// it is redundant (all segments of such light polygon is created from segments of level, there is
// no any "new" line drawn at all). If there is more than two points hitting one segment next to
// each other all middle points can be ommited, we do this before drawing to save some computing
// power. 
vertex_t* LIG_calc_light_polygon(
    int x,
    int y,
    float angle,
    float width,
    obstacle_t *obstacles
) { 
    obstacle_t *filtered_obstacles = NULL;   // not every level obstacle is taken into account
    vertex_t   *light_polygon      = NULL;   // here is points which make light polygon is stored
    point_t    *hit_points         = NULL;   // here is points to check ray hit stored
    // point_t    *new_point          = NULL;

    hit_points         = LIG_calc_hit_points(x, y, angle, width, obstacles);
    filtered_obstacles = LIG_filter_obstacles_by_sight(x, y, angle, width, obstacles);
    
    printf("hit points: ");

    point_t* ptrptr = NULL;
    ptrptr = hit_points;
    
    while (ptrptr) {
        printf("%i, %i \n", ptrptr->x, ptrptr->y);
        ptrptr=ptrptr->next;
    }
    printf("\n\n");

    // for each of point to hit n_width_rays rays are casted
    for(point_t* pt=hit_points; pt; pt=pt->next) {
        point_t    *new_point_a        = NULL;
        point_t    *new_point_b        = NULL;

        angle = VRTX_calculate_angle(x, y, pt->x, pt->y);

        // direct ray (if direct ray hits anything, it should not be pushed into light vertex)
        if (!LIG_any_intersection_with_obstacle(x, y, pt->x, pt->y, obstacles)) {
            VRTX_add_point(&light_polygon, pt->x, pt->y, angle);
        }

        // first shifted ray
        new_point_a = LIG_closest_intersection_with_obstacle(
            x,
            y,
            pt->x - sin(angle + smol_angle) * R,
            pt->y - cos(angle + smol_angle) * R,
            obstacles
        );
        VRTX_add_point(&light_polygon, new_point_a->x, new_point_a->y, angle+smol_angle);

        // second shifted ray
        new_point_b = LIG_closest_intersection_with_obstacle(
            x,
            y,
            pt->x - sin(angle - smol_angle) * R,
            pt->y - cos(angle - smol_angle) * R,
            obstacles
        );
        VRTX_add_point(&light_polygon, new_point_b->x, new_point_b->y, angle-smol_angle);
    }

    VRTX_add_point(&light_polygon, x, y, 0);

    // polygon point optimization process (deleting redundant points)
    // VRTX_optim(light_polygon);

    // if (DEBUG){ LIG_debug_obstacle_ends(x, y, filtered_obstacles);}
    // if (DEBUG){ LIG_debug_obstacle(obstacles);}
    // if (DEBUG){ LIG_debug_rays(light_polygon, x, y, 200);}
    // if (DEBUG){ LIG_debug_dark_sectors(light_polygon);}
    
    obstacle_t* ptr = NULL;
    ptr = obstacles;

    printf("hero pos: %i %i \n", x, y);
    printf("obstacles: \n");
    while (ptr) {
        printf("%i %i, %i %i\n", ptr->x1, ptr->y1, ptr->x2, ptr->y2);
        ptr=ptr->next;
    }
    printf("width %f\n", width);
    printf("angle %f\n", angle);
    printf("\n\n");

    OBS_free(filtered_obstacles);
    // OBS_free(obstacles);

    return light_polygon;
};

// changes heros lightsource to another one
void LIG_change_source(
    light_t* lght
) {
    lght->src_num = ((lght->src_num) + 1) % ALL;
    lght->src = lightsources[lght->src_num];
}

int LIG_get_light_polygon_color(
    light_t *lght,
    int      i,
    int      color
) {
    return lght->src->polys[i][color];
}

int LIG_get_light_polygon_x_corr(
    light_t *lght,
    int      i
) {
    return sin(lght->angle) * lght->src->polys[i][X] + cos(lght->angle) * lght->src->polys[i][Y];
}

int LIG_get_light_polygon_y_corr(
    light_t *lght,
    int      i 
) {
    return sin(lght->angle) * lght->src->polys[i][Y] + cos(lght->angle) * lght->src->polys[i][X];
}

float LIG_get_light_polygon_width_corr(
    light_t *lght,
    int      i
) {
    int coef = lght->src->polys[i][WIDTH];

    if (coef){
        return PI / coef;
    }
    return 0.0;
}

// Calculates and draws light polygons. Every of the light source needs several polygons to be drawn
// - every one of them is slightly moved to another which makes light looks more "natural".
// Furthermore, the polygons with bigger shift has more pale color resulting in overall effect
// looking like "gradient".
void LIG_draw_light_effect(
    int         x,
    int         y,
    light_t    *lght,
    obstacle_t *obstacles
) {
    int   i;                // index of current light polygon drawn
    int   red;              // color of current light polygon drawn
    int   green;            // color of current light polygon drawn
    int   blue;             // color of current light polygon drawn
    int   alpha;            // color of current light polygon drawn
    int   x_corr, y_corr;   // x and y correction values (light polygon can be shifted from its starting point)
    float width_corr;       // light width correction (some light polygons can be wider)

    vertex_t* light_polygon = NULL;

    for (i=0; i < lght->src->n_poly; i++) {
        red        = LIG_get_light_polygon_color(lght, i, RED);
        green      = LIG_get_light_polygon_color(lght, i, GREEN);
        blue       = LIG_get_light_polygon_color(lght, i, BLUE);
        alpha      = LIG_get_light_polygon_color(lght, i, ALPHA);

        x_corr     = LIG_get_light_polygon_x_corr(lght, i);
        y_corr     = LIG_get_light_polygon_y_corr(lght, i);
        width_corr = LIG_get_light_polygon_width_corr(lght, i);

        // calculating the light polygon shape
        printf("corrs: %i %i \n\n", x_corr, y_corr);
        printf("start coords: %i %i \n\n", x, y);
        light_polygon = LIG_calc_light_polygon(
            x+x_corr, y+y_corr, lght->angle, lght->src->width+width_corr, obstacles
        );

        // drawing the light
        GFX_draw_light_polygon(light_polygon, red, green, blue, alpha);
    }
};

void LIG_free(
    light_t* lght
) {
    free(lght);
};
