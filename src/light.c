#include "geometry.h"
#include "gfx.h"
#include "light.h"
#include "intersection.h"
#include "vertex.h"
#include "sprites.h"
#include "primitives.h"

// light calculation and rendering implementation. Basically each lightsource creates some light
// rays which is used to calculate polygon where light can reach. Such polygon is drawn with higher
// alpha value which creates realisitc effect

#define R 1200
#define smol_angle 0.01
#define empty_hit 69

static int wobble_dir = 1;
static float wobble = 0;
static int n_width_rays = 3;

float lightpos_up_down_corr[2][5] = 
{
  // LEFT  RIGHT  UP               DOWN             NONE
     {0,   0,     RIGHT_RAD+DEG30, RIGHT_RAD-DEG30, RIGHT_RAD},     // RIGHT 
     {0,   0,     LEFT_RAD-DEG30,  LEFT_RAD+DEG30,  LEFT_RAD}       // LEFT 
};

lightsource_t lantern = 
{
    .width = 0.0,
    .n_poly = 9,
    .wobble_corr = 0.0,
    .wobble_change_dir_coef = 1,
    .polys = {
        {-10,     -10,       10    },
        { 10,     -10,       10    },
        {-10,     -10,       10    },
        {-10,      10,       10    },
        {-5,       -5,       30    },
        { 5,       -5,       30    },
        {-5,       -5,       30    },
        {-5,        5,       30    },
        { 0,        0,       50    },
    }
};

lightsource_t lighter = 
{
    .width = PI / 7,
    .n_poly = 2,
    .wobble_corr = 0.001,
    .wobble_change_dir_coef = 30,
    .polys = {
        {  0,       0,       50    },
        {  5,       0,       30    },
        { -5,       0,       30    },
        {  0,       5,       10    },
        {  0,      -5,       10    },
    }
};


float sin_angle_corr(float angle, int radius){
    return sin(angle) * R;
};

float cos_angle_corr(float angle, int radius){
    return cos(angle) * R;
};

float no_corr(float angle, int radius){
    return 0.0;
};

float(*width_corr_fun[3][2])(float, int) = {
    {sin_angle_corr, cos_angle_corr},
    {sin_angle_corr, cos_angle_corr},
    {no_corr,        no_corr       }
};

int width_corrs[3][2] = {
    { -1, -1 },
    {  1,  1 },
    {  1,  0 }
};

float LIG_calc_angle_corr(float angle, float width, int n_ray, int axis) {
    int width_corr = width_corrs[n_ray][0];
    int rad_corr = width_corrs[n_ray][1];

    float corrected_angle = angle + (width_corr * width);
    float corrected_radius = R * rad_corr;

    return width_corr_fun[n_ray][axis](corrected_angle, corrected_radius);
}

lightsource_t* lightsources[] = {&lighter, &lantern};

light_t * LIG_init() {
    light_t* light_o = (light_t*)malloc(sizeof(light_t));

    light_o->src_num = LANTERN;
    light_o->src = lightsources[light_o->src_num];
    light_o->angle = LEFT_RAD;

    return light_o;
}

// calculates small angle correction to make light effect be more realistic. Charachteristic
// "wobblig" effect is achieved while changing angle in small amount slowly.
float LIG_wobble_angle(light_t* lght, int frame) {
    if ((frame % lght->src->wobble_change_dir_coef) == 0) {
        wobble_dir = (-1) * wobble_dir;
    }

    wobble += lght->src->wobble_corr * wobble_dir;

    // return wobble;
    return 0;
}

void LIG_move_lightsource(light_t* light_o, direction_t light_dir, direction_t hero_dir, int frame) {
    light_o->angle = lightpos_up_down_corr[hero_dir][light_dir] + LIG_wobble_angle(light_o, frame);
}

// DEBUG function
// draws rays into every cone of the lightpolygon
void LIG_debug_rays(vertex_t* light_poly, int st_x, int st_y, int alpha) {
    for(vertex_t* ptr = light_poly; ptr; ptr = ptr->next) {
        GFX_draw_colored_line(st_x, st_y, ptr->x, ptr->y, alpha, alpha, alpha, 255); 
    }
}

// draws edges of light polygon
void LIG_debug_dark_sectors(vertex_t* light_poly) {
    int first_x = light_poly->x;
    int first_y = light_poly->y;
    vertex_t* ptr = light_poly;

    for(; ptr->next; ptr = ptr->next) {
        GFX_draw_colored_line(ptr->x, ptr->y, ptr->next->x, ptr->next->y, 255, 255, 255, 255); 
    }
    GFX_draw_colored_line(ptr->x, ptr->y, first_x, first_y, 255, 255, 255, 255);
}

// checks if ray intersects with any obstacle. If so ray is shorten to the point of intersection.
void LIG_obstacle_intersects_with_ray
(
    obstacle_t* obstacle,    // first
    ray_t* ray               // second
)
{
    float s_numer, t_numer, denom, t;
    float dx13, dy13;
    float dx21, dx43, dy21, dy43;
    float new_dist, old_dist;
    int new_x, new_y;

    dx21 = obstacle->x2 - obstacle->x1;
    dy21 = obstacle->y2 - obstacle->y1;
    dx43 = ray->x2 - ray->x1;
    dy43 = ray->y2 - ray->y1;

    denom = dx21 * dy43 - dx43 * dy21;

    if (denom == 0)
        return; // collinear

    bool denomPositive = denom > 0;

    dx13 = obstacle->x1 - ray->x1;
    dy13 = obstacle->y1 - ray->y1;

    s_numer = dx21 * dy13 - dy21 * dx13;
    if ((s_numer < 0) == denomPositive)
        return; // no collision

    t_numer = dx43 * dy13 - dy43 * dx13;
    if ((t_numer < 0) == denomPositive)
        return; // no collision

    if (((s_numer >= denom) == denomPositive) || ((t_numer >= denom) == denomPositive))
        return; // no collision

    // collision detected
    t = t_numer / denom;

    new_x = (int) obstacle->x1 + (t * dx21);
    new_y = (int) obstacle->y1 + (t * dy21);

    new_dist = GEO_distance(ray->x1, ray->y1, new_x, new_y);
    old_dist = GEO_distance(ray->x1, ray->y1, ray->x2, ray->y2);

    if (new_dist < old_dist) {
        ray->x2 = new_x;
        ray->y2 = new_y;
    }
}

// Function checks for any of the possible interseciton between single ray and set of segments
// (obstacles). Best (closest) intersection is then stored in ray segment end coordinates.
void LIG_find_closest_intersection_with_wall(
    ray_t* ray,
    obstacle_t* obstacles
)
{ 
    obstacle_t* obstacle = NULL;
    
    for(obstacle=obstacles; obstacle; obstacle=obstacle->next) {
        LIG_obstacle_intersects_with_ray(obstacle, ray);
    }
}

// we need to filter those level obstacles for which we will cast rays - points which is
// inside triangle made from two border rays and player position is taken.
obstacle_t* LIG_get_visible_obstacles(
    obstacle_t* obstacles,
    int x, int y,
    float angle, float width 
){
    obstacle_t* filtered_obstacles = NULL;

    for(obstacle_t* s=obstacles; s; s=s->next) {
        if (GEO_pt_in_triangle(
                s->x1, s->y1,
                x, y,
                x - LIG_calc_angle_corr(angle, width, 0, X), y - LIG_calc_angle_corr(angle, width, 0, Y),
                x - LIG_calc_angle_corr(angle, width, 1, X), y - LIG_calc_angle_corr(angle, width, 1, Y)
            )) {
            OBS_push(&filtered_obstacles, s->x1, s->y1, s->x2, s->y2);
        }
    }

    return filtered_obstacles;
}

void LIG_find_light_vertex_from_ray(obstacle_t* obstacles, ray_t ray, vertex_t** light_polygon){
    LIG_find_closest_intersection_with_wall(&ray, obstacles);
    VRTX_insert(light_polygon, &ray);
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
vertex_t* LIG_calc_light_polygon(int x, int y, float angle, float width, obstacle_t* obstacles) { 
    int ray_i;
    float x_corr, y_corr;

    obstacle_t* filtered_obstacles = NULL;
    vertex_t* light_polygon = NULL;

    if (width) {
        filtered_obstacles = LIG_get_visible_obstacles(obstacles, x, y, angle, width);

        // check if border rays hit any obstacle - such intersection points will be added as light
        // poly vertex
        for (ray_i=0; ray_i<n_width_rays; ray_i++) {
            x_corr = LIG_calc_angle_corr(angle, width, ray_i, X);
            y_corr = LIG_calc_angle_corr(angle, width, ray_i, Y);

            ray_t ray = (ray_t){x, y, x-x_corr, y-y_corr};

            LIG_find_light_vertex_from_ray(obstacles, ray, &light_polygon);
        }
    }

    else { filtered_obstacles = obstacles; }

    // for each of vertex n_width_rays rays are casted
    for(obstacle_t* s=filtered_obstacles; s; s=s->next) {
        angle = GEO_calculate_angle(x, y, s->x1, s->y1);

        for (ray_i=0; ray_i<n_width_rays; ray_i++) {
            x_corr = LIG_calc_angle_corr(angle, smol_angle, ray_i, X);
            y_corr = LIG_calc_angle_corr(angle, smol_angle, ray_i, Y);

            ray_t ray = (ray_t){x, y, s->x1-x_corr, s->y1-y_corr};

            LIG_find_light_vertex_from_ray(obstacles, ray, &light_polygon);
        }
    }

    // polygon point optimization process (deleting redundant points)
    VRTX_optim(light_polygon);

    // if (DEBUG){LIG_debug_rays(light_polygon, x, y, 200);}
    // if (DEBUG){LIG_debug_dark_sectors(light_polygon);}

    return light_polygon;
};

void LIG_change_source(light_t* lght) {
    lght->src_num = ((lght->src_num) + 1) % ALL;
    lght->src = lightsources[lght->src_num];
}

int LIG_get_light_polygon_color(light_t* lght, int i){
    return lght->src->polys[i][COLOR];
}

int LIG_get_light_polygon_corr(light_t* lght, int i, int axis){
    return lght->src->polys[i][axis];
}

// calculates and draws light polygons. Every of the light source needs several polygons to be drawn
// - every one of them is slightly moved to another which makes light looks more "natural".
// Furthermore, the polygons with bigger shift has more pale color resulting in overall effect
// looking like "gradient". Number of the polygons and shift coords are defined by its lightsource.
void LIG_draw_light_effect(int x, int y, light_t* lght, obstacle_t* obstacles) {
    int i=0;            // index of current light polygon drawn
    int color;          // color of current light polygon drawn
    int x_corr, y_corr; // x and y correction values (light polygon can be shifted from its starting point)

    vertex_t* light_polygon = NULL;

    for (; i < lght->src->n_poly; i++) {
        color = LIG_get_light_polygon_color(lght, i);
        x_corr = LIG_get_light_polygon_corr(lght, i, X);
        y_corr = LIG_get_light_polygon_corr(lght, i, Y);

        // calculating the light polygon shape :)
        light_polygon = LIG_calc_light_polygon(x+x_corr, y+y_corr, lght->angle, lght->src->width, obstacles);

        // drawing the light
        GFX_draw_polygon(light_polygon, color);
    }
    OBS_free(obstacles);
};

void LIG_free(light_t* lght) {
    free(lght);
};
