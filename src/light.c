#include "config.h"
#include "segment.h"
#include "geometry.h"
#include "gfx.h"
#include "light.h"
#include "vertex.h"
#include "sprites.h"
#include "primitives.h"
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
    light_o->wobble_dir = WOBBLE_DOWN;
    light_o->wobble     = 0;

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

// checks if ray (defined by (x1, y1), (x2, y2) hit obstacle (segment_t). Please not that obstacle
// can be only horizontal or vertical intersection point is calculated as float to more directly
// check if it hit obstacle. Rounding is than done simply by casting it to integer.
// points with intersection is then returned (if nothing is hit the original coords are returned).
point_t* LIG_ray_hits_obstacle (
    int x1,
    int y1,
    int x2,
    int y2,
    segment_t *obstacle
) {
    point_t *base_point = PT_new(x2, y2);  // hit point is stored here
    float    new_value;

    // obstacle is vertical
    if (obstacle->x1 == obstacle->x2) {
        if (GEO_value_between_range(obstacle->x1, x1, x2)) {
            new_value = GEO_intersection_with_x(obstacle->x1, x1, y1, x2, y2);

            if (GEO_value_between_range(new_value, obstacle->y1, obstacle->y2)) {
                base_point->y = (int) new_value;
                base_point->x = obstacle->x1;
            }
        }
    }

    // obstacle is horizontal
    else {
        if (GEO_value_between_range(obstacle->y1, y1, y2)) {
            new_value = GEO_intersection_with_y(obstacle->y1, x1, y1, x2, y2);

            if (GEO_value_between_range(new_value, obstacle->x1, obstacle->x2)) {
                base_point->x = (int) new_value;
                base_point->y = obstacle->y1;
            }
        }
    }

    return base_point;

}

// Function checks for any of the possible interseciton between single ray (defined by (x1, y1),
// (x2, y2) and set of obstacles. Best (closest) intersection point is then returned.
point_t* LIG_closest_intersection_with_obstacle(
    int x1,
    int y1,
    int x2,
    int y2,
    segment_t *obstacles
)
{ 
    point_t   *best_point = NULL;
    segment_t *obstacle   = NULL;
    
    best_point = PT_new(x2, y2);    

    for(obstacle=obstacles; obstacle; obstacle=obstacle->next) {
        best_point = LIG_ray_hits_obstacle(x1, y1, best_point->x, best_point->y, obstacle);
    }

    return best_point;
}

// checks if segment hits any of the obstacle, as above function but returns only bool if so.
bool LIG_any_intersection_with_obstacle(
    int x1,
    int y1,
    int x2,
    int y2,
    segment_t *obstacles
)
{ 
    point_t    *best_point = NULL;
    segment_t *obstacle   = NULL;

    for(obstacle=obstacles; obstacle; obstacle=obstacle->next) {
        best_point = LIG_ray_hits_obstacle(x1, y1, x2, y2, obstacle);

        if (best_point->x != x2 || best_point->y != y2) {
            return true;
        }
    }

    return false;
}

// Calculates end of light rays, only those ends of obstacles which is in sight of light cone is
// filtered. If light sweeps all around all segment ends are taken into account.
point_t* LIG_calc_hit_points(
    int        x,
    int        y,
    float      angle,
    float      width,
    segment_t *obstacles
) { 
    point_t    *hit_points = NULL;
    point_t    *pt_a       = NULL;
    point_t    *pt_b       = NULL;
    segment_t  *ptr        = NULL;
    ptr                    = obstacles;

    if (width != 0.0) {

        // light cone
        int a_border_x = (int)(x - sin(angle - width) * R);
        int a_border_y = (int)(y - cos(angle - width) * R);
        int b_border_x = (int)(x - sin(angle + width) * R);
        int b_border_y = (int)(y - cos(angle + width) * R);
        
        // if light cone edges intersectis with some obstacles such points needs to be added
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

        // filtering obstacle ends which is inside light cone
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
    
    // light sweeps all around - all points of obstacles are taken
    else {
        while(ptr) {
            PT_push(&hit_points, ptr->x1, ptr->y1);
            PT_push(&hit_points, ptr->x2, ptr->y2);
            ptr=ptr->next;
        }
    }

    return hit_points;
}

// Calculates coords of light polygon vertices.
vertex_t* LIG_calc_light_polygon(
    int x,                 // x starting point (hero)
    int y,                 // y starting point (hero)
    float angle,           // light angle
    float width,           // light width
    segment_t *obstacles   // sets of obstacles
) { 
    vertex_t  *light_polygon      = NULL;   // here is points which make light polygon is stored
    point_t   *hit_points         = NULL;   // here is points to check ray hit stored

    // calculating ray ends
    hit_points         = LIG_calc_hit_points(x, y, angle, width, obstacles);
    
    // for ray is checked for collision and the two rays which shifts from this one (by a little
    // angle).
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

    // if (DEBUG){ LIG_debug_obstacle_ends(x, y, filtered_obstacles);}
    // if (DEBUG){ LIG_debug_obstacle(filtered_obstacles); }
    // if (DEBUG){ LIG_debug_points(hit_points); }
    // if (DEBUG){ LIG_debug_rays(light_polygon, x, y, 200);}
    // if (DEBUG){ LIG_debug_dark_sectors(light_polygon);}
    
    // polygon point optimization process (deleting redundant points)
    VRTX_optim(light_polygon);
    // OBS_free(obstacles);

    return light_polygon;
};

// changes hero lightsource to another one
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

void LIG_get_jiggy_with_it(
    light_t *lght,
    int frame
) {
    if ((frame % lght->src->wobble_change_dir_coef) == 0) {
        lght->wobble_dir = (-1) * lght->wobble_dir;
    }
    lght->wobble += lght->src->wobble_corr * lght->wobble_dir;
}

// Calculates and draws light polygons. Every of the light source needs several polygons to be drawn
// - every one of them is slightly moved to another which makes light looks more "natural".
// Furthermore, the polygons with bigger shift has more pale color resulting in overall effect
// looking like "gradient".
void LIG_draw_light_effect(
    int        x,
    int        y,
    int        frame,
    light_t    *lght,
    segment_t  *obstacles
) {
    int   i;                // index of current light polygon drawn
    int   red;              // color of current light polygon drawn
    int   green;            // color of current light polygon drawn
    int   blue;             // color of current light polygon drawn
    int   alpha;            // color of current light polygon drawn
    int   x_corr, y_corr;   // x and y correction values (light polygon can be shifted from its starting point)
    float width_corr;       // light width correction (some light polygons can be wider)
    
    LIG_get_jiggy_with_it(lght, frame);
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
        light_polygon = LIG_calc_light_polygon(
            x+x_corr, y+y_corr, lght->angle+lght->wobble, lght->src->width+width_corr, obstacles
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
