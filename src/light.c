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


texture_t *circular_gradient = NULL;

// yeah, this should be in some file
wobble_t no_wobble = {
    .len = 1,
    .coefs = {0.0}
};

// yeah, this should be in some file
wobble_t stable_wobble = {
    .len = 82,
    .coefs = {
        0.006, 0.006, 0.009, 0.009, 0.012, 0.012, 0.017, 0.017, 0.022, 0.022,
        0.027, 0.027, 0.033, 0.033, 0.038, 0.038, 0.042, 0.042, 0.045, 0.045,
        0.046, 0.046, 0.045, 0.045, 0.042, 0.042, 0.038, 0.038, 0.033, 0.033,
        0.027, 0.027, 0.022, 0.022, 0.017, 0.017, 0.012, 0.012, 0.009, 0.009,
        0.0, 0.0,
        -0.006, -0.006, -0.009, -0.009, -0.012, -0.012, -0.017, -0.017, -0.022, -0.022,
        -0.027, -0.027, -0.033, -0.033, -0.038, -0.038, -0.042, -0.042, -0.045, -0.045,
        -0.046, -0.046, -0.045, -0.045, -0.042, -0.042, -0.038, -0.038, -0.033, -0.033,
        -0.027, -0.027, -0.022, -0.022, -0.017, -0.017, -0.012, -0.012, -0.009, -0.009
    }
};

// yeah, this should be in some file
wobble_t walk_wobble = {
    .len = 15,
    .coefs = {
        0.012, 0.054, 0.082, 0.1, 0.082, 
        0.054, 0.012,
        0.0,
        -0.012, -0.054, -0.082, -0.1, -0.082,
        -0.054, -0.012, }
};

// yeah, this should be in some file
lightsource_t lantern = {
    .width = 0.0,
    .n_poly = 9,
    .wobble = {&no_wobble, &no_wobble},
    .penetrating_power = 7,
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
    .gradient_texture = NULL
};

// yeah, this should be in some file
lightsource_t lighter = {
    .width = PI / 7,
    .n_poly = 3,
    .wobble = {&stable_wobble, &walk_wobble},
    .penetrating_power = 7,
    .polys = {
        {  0,  0, DEFAULT_LIGHT_R, DEFAULT_LIGHT_G, DEFAULT_LIGHT_B, 50, 0  },
        // {  4,  0, DEFAULT_LIGHT_R, DEFAULT_LIGHT_G, DEFAULT_LIGHT_B, 50, 0  },
        // { -4,  0, DEFAULT_LIGHT_R, DEFAULT_LIGHT_G, DEFAULT_LIGHT_B, 50, 0  },
        // {  0,  4, DEFAULT_LIGHT_R, DEFAULT_LIGHT_G, DEFAULT_LIGHT_B, 30, 0  },
        // {  0, -4, DEFAULT_LIGHT_R, DEFAULT_LIGHT_G, DEFAULT_LIGHT_B, 30, 0  },
        {  0,  0, DEFAULT_LIGHT_R, DEFAULT_LIGHT_G, DEFAULT_LIGHT_B, 30, 36 },
        {  0,  0, DEFAULT_LIGHT_R, DEFAULT_LIGHT_G, DEFAULT_LIGHT_B, 20, 72 },
    },
    .gradient_texture = NULL
};

lightsource_t* lightsources[] = {&lighter, &lantern};

// changes done to light angle if looking in different direction
float lightpos_up_down_corr[2][5] = {
  // LEFT  RIGHT  UP               DOWN             NONE
     {0,   0,     RIGHT_RAD+DEG30, RIGHT_RAD-DEG30, RIGHT_RAD},     // RIGHT 
     {0,   0,     LEFT_RAD-DEG30,  LEFT_RAD+DEG30,  LEFT_RAD}       // LEFT 
};

void LIG_init_gradients(
) {
    // circular_gradient = GFX_read_texture("gradients/circular_gradient.png");
    circular_gradient = GFX_read_texture("gradients/circular.png");
}

light_t *LIG_init() {
    // this is very ugly! storing lightsource data should be done in prettier matter!
    LIG_init_gradients();

    lighter.gradient_texture = circular_gradient;
    lantern.gradient_texture = circular_gradient;

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
        if (GEO_value_between_range(obstacle->x1, x1, x2, SLIGHLY_LOOSEN_ACCURACY)) {
            new_value = GEO_intersection_with_x(obstacle->x1, x1, y1, x2, y2);

            if (GEO_value_between_range(new_value, obstacle->y1, obstacle->y2, SLIGHLY_LOOSEN_ACCURACY)) {
                base_point->y = (int) new_value;
                base_point->x = obstacle->x1;
            }
        }
    }

    // obstacle is horizontal
    else {
        if (GEO_value_between_range(obstacle->y1, y1, y2, SLIGHLY_LOOSEN_ACCURACY)) {
            new_value = GEO_intersection_with_y(obstacle->y1, x1, y1, x2, y2);

            if (GEO_value_between_range(new_value, obstacle->x1, obstacle->x2, SLIGHLY_LOOSEN_ACCURACY)) {
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
    segment_t  *obstacle   = NULL;

    for(obstacle=obstacles; obstacle; obstacle=obstacle->next) {
        best_point = LIG_ray_hits_obstacle(x1, y1, x2, y2, obstacle);

        if (best_point->x != x2 || best_point->y != y2) {
            PT_free(best_point);
            return true;
        }
    }

    PT_free(best_point);
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
        PT_free(pt_a);
        PT_free(pt_b);
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

        PT_free(new_point_a);
        PT_free(new_point_b);
    }

    if (width != 0.0) {
        VRTX_add_point(&light_polygon, x, y, 0);
    }

    if (debug == DEBUG_OBSTACLE_LINES) { LIG_debug_obstacle(obstacles); }
    if (debug == DEBUG_LIGHT_RAYS) { LIG_debug_rays(light_polygon, x, y, 200);}
    
    // polygon point optimization process (deleting redundant points)
    VRTX_optim(light_polygon);

    // cleaning
    PT_free(hit_points);

    return light_polygon;
};

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

int LIG_get_lightsource_feature(
    light_t *lght,
    int      i,
    int      color
) {
    return lght->src->polys[i][color];
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

float LIG_get_wobble_angle_coef(
    light_t *lght,
    int      x_vel,
    int      frame
) {
    int state = 0;

    if (x_vel != 0) {
        state=1;
    }

    wobble_t *current_wobble = lght->src->wobble[state];
    return current_wobble->coefs[frame%current_wobble->len];
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
    int   i;                 // index of current light polygon drawn
    int   red;               // color of current light polygon drawn
    int   green;             // color of current light polygon drawn
    int   blue;              // color of current light polygon drawn
    int   light_power;       // power of light (brightness)
    int   penetrating_power; // ability to penetrate walls by light
    int   x_corr, y_corr;    // x and y correction values (light polygon can be shifted from its starting point)
    float width_corr;        // light width correction (some light polygons can be wider)
    float wobble_corr;       // angle correction due to wobling

    // before adding any new light to scene cleansing of lighbuffers is needed
    GFX_clean_buffers();

    vertex_t* light_polygon     = NULL;
    vertex_t* light_wall_shadow = NULL;
    wobble_corr                 = LIG_get_wobble_angle_coef(light, x_vel, frame);

    for (i=0; i < light->src->n_poly; i++) {
        red              = LIG_get_lightsource_feature(light, i, RED);
        green            = LIG_get_lightsource_feature(light, i, GREEN);
        blue             = LIG_get_lightsource_feature(light, i, BLUE);
        light_power      = LIG_get_lightsource_feature(light, i, LIGHT_POWER);

        penetrating_power = LIG_get_light_penetrating_power(light);
        x_corr           = LIG_get_light_polygon_x_corr(light, i);
        y_corr           = LIG_get_light_polygon_y_corr(light, i);
        width_corr       = LIG_get_light_polygon_width_corr(light, i);

        // calculating the light polygon shape
        light_polygon = LIG_calc_light_polygon(
            x+x_corr,
            y+y_corr,
            light->angle+wobble_corr,
            light->src->width+width_corr,
            obstacles
        );

        light_wall_shadow = LIG_calc_light_wall_shadow(
            light_polygon,
            penetrating_power,
            x,
            y
        );
        
        // fill lightbuffer with freshly calulcated light polygon
        GFX_fill_buffer_single_polygon(light_polygon, GFX_fill_lightbuffer, red, green, blue, light_power);

        // fill lightbuffer with freshly calulcated light polygon
        GFX_fill_buffer_single_polygon(light_wall_shadow, GFX_fill_mesh_shadowbuffer, red, green, blue, 20);

        // cleaning
        VRTX_free(light_polygon);
    }
};

void LIG_free(
    light_t* lght
) {
    free(lght);
};
