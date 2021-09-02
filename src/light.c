#include "geometry.h"
#include "gfx.h"
#include "light.h"
#include "intersection.h"
#include "lightpt.h"
#include "sprites.h"
#include "primitives.h"

// light calculation and rendering implementation. Basically each lightsource creates some light
// rays which is used to calculate polygon where light can reach. Such polygon is drawn with higher
// alpha value which creates realisitc effect

#define R 1200
#define smol_angle 0.01

static int wobble_dir = 1;
static float wobble = 0;

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
    .n_poly = 1,
    .wobble_corr = 0.001,
    .wobble_change_dir_coef = 30,
    .polys = {
        {  0,      -5,       10    },
        {  0,       5,       10    },
        { -5,       0,       30    },
        {  5,       0,       30    },
        {  0,       0,       50    },
    }
};

lightsource_t* lightsources[] = {&lighter, &lantern};

light_t * LIG_init()
{
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

    return 0;
}

void LIG_move_lightsource(light_t* light_o, direction_t light_dir, direction_t hero_dir, int frame) {
    light_o->angle = lightpos_up_down_corr[hero_dir][light_dir] + LIG_wobble_angle(light_o, frame);
}

// DEBUG function
// draws rays into every cone of the lightpolygon
void LIG_debug_rays(lightpoint_t* light_poly, int st_x, int st_y, int alpha) {
    for(lightpoint_t* ptr = light_poly; ptr; ptr = ptr->next) {
        GFX_draw_colored_line(st_x, st_y, ptr->x, ptr->y, alpha, alpha, alpha, 255); 
    }
}

// draws edges of light polygon
void LIG_debug_dark_sectors(lightpoint_t* light_poly) {
    int first_x = light_poly->x;
    int first_y = light_poly->y;
    lightpoint_t* ptr = light_poly;

    for(; ptr->next; ptr = ptr->next) {
        GFX_draw_colored_line(ptr->x, ptr->y, ptr->next->x, ptr->next->y, 255, 255, 255, 255); 
    }
    GFX_draw_colored_line(ptr->x, ptr->y, first_x, first_y, 255, 255, 255, 255);
}

// checks if ray intersects with any obstacle. If so ray is shorten to the point of intersection.
int LIG_obstacle_intersects_with_ray
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
        return 0; // collinear

    bool denomPositive = denom > 0;

    dx13 = obstacle->x1 - ray->x1;
    dy13 = obstacle->y1 - ray->y1;

    s_numer = dx21 * dy13 - dy21 * dx13;
    if ((s_numer < 0) == denomPositive)
        return 0; // no collision

    t_numer = dx43 * dy13 - dy43 * dx13;
    if ((t_numer < 0) == denomPositive)
        return 0; // no collision

    if (((s_numer >= denom) == denomPositive) || ((t_numer >= denom) == denomPositive))
        return 0; // no collision

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

    return 1;
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

// Fills screen with light and dark lines. Inside of the light polygon is filled with bright lines
// (with high alpha channel value) and the rest (outside of the light polygon) is filled with dark
// lines (with very low alpha channel value). Function assumes that points of light polygon are
// sorted in ascending order. To sort point value of an angle which given point make with player
// position is used.
void LIG_fill_lightpoly(lightpoint_t* pts, int clr)
{
    float x_inter;

    // Scan every y for seach of colision with light polygon.
    for (int scan_y=0; scan_y<SCREEN_HEIGHT; scan_y++) 
    {
        int n=0;                                  // number of intersection on given scanline
        x_intersection_t* intersections = NULL;   // x-intersections on scanline with polygon stored here
        lightpoint_t* ptr = pts;                 
        int first_x = ptr->x;                     // will be used for last segment intersection check
        int first_y = ptr->y;                     // will be used for last segment intersection check
        int last_x_intersection = 0;              // will be used later

        for(; ptr->next; ptr=ptr->next)
        {
            if (GEO_if_seg_intersect_with_y(scan_y, ptr->y, ptr->next->y))
            {
                x_inter = GEO_seg_intersection_with_y(scan_y, ptr->x, ptr->y, ptr->next->x, ptr->next->y);
                INTSC_insert(&intersections, x_inter);
                n++;
            }
        }

        // last segment
        if (GEO_if_seg_intersect_with_y(scan_y, ptr->y, first_y))
        {
            x_inter = GEO_seg_intersection_with_y(scan_y, ptr->x, ptr->y, first_x, first_y);
            INTSC_insert(&intersections, x_inter);
            n++;
        }

        // if no intersection found this scanline should be all black 
        if (n==0) 
        { GFX_draw_colored_line(0, scan_y, SCREEN_WIDTH, scan_y, 0, 0, 0, 100); continue;}

        // dark segment from left side of screen to first intersection with scanline
        if (intersections->x > 1)
        { GFX_draw_colored_line(0, scan_y, intersections->x, scan_y, 0, 0, 0, 100);}

        // dark segment from right side of screen to last intersection with scanline
        last_x_intersection = INTSC_get_last(intersections);
        if (last_x_intersection < SCREEN_WIDTH-1) 
        { GFX_draw_colored_line(last_x_intersection, scan_y, SCREEN_WIDTH, scan_y, 0, 0, 0, 100); }

        // rest of the lines (light and dark sectors)
        if (n>2)
        {
            for (int z=1; z<n-1; z+=2)
            {
                GFX_draw_colored_line(intersections->x, scan_y, intersections->next->x, scan_y, 169, 169, 169, clr); 
                intersections=intersections->next;

                GFX_draw_colored_line(intersections->x, scan_y, intersections->next->x, scan_y, 0, 0, 0, 255); 
                intersections=intersections->next;
            }
        }

        // last of the dark sector from intesection points
        GFX_draw_colored_line(intersections->x, scan_y, intersections->next->x, scan_y, 169, 169, 169, clr); 

        INTSC_free(intersections);
    }
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
lightpoint_t* LIG_calc_light_polygon(int x, int y, float angle, float width, obstacle_t* obstacles) { 
    obstacle_t* filtered_obstacles = NULL;
    lightpoint_t* light_pts = NULL;

    if (width) {
        // If light source has any width "border" there are two rays which is closing light polygon.
        // Below code calculates such border rays.
        // If this value is 0 light is casted on whole radius, and such border rays is not needed
        ray_t ray_a = (ray_t){x, y, (int)(x - sin(angle - width) * R), (int)(y - cos(angle - width) * R)};
        ray_t ray_b = (ray_t){x, y, (int)(x - sin(angle + width) * R), (int)(y - cos(angle + width) * R)};

        // we need to filter those level obstacles for which we will cast rays - points which is
        // inside triangle made from two border rays and player position is taken.
        for(obstacle_t* s=obstacles; s; s=s->next) {
            if (GEO_pt_in_triangle(s->x1, s->y1, x, y, ray_a.x2, ray_a.y2, ray_b.x2, ray_b.y2)) {
                OBS_push(&filtered_obstacles, s->x1, s->y1, s->x2, s->y2);
            }
        }

        // check if border rays hit any obstacle - such intersection points will be added as light
        // poly cone
        LIG_find_closest_intersection_with_wall(&ray_a, obstacles);
        angle = LIGPT_calculate_angle(x, y, ray_a.x2, ray_a.y2);
        LIGPT_insert(&light_pts, ray_a.x2, ray_a.y2, angle);

        LIG_find_closest_intersection_with_wall(&ray_b, obstacles);
        angle = LIGPT_calculate_angle(x, y, ray_b.x2, ray_b.y2);
        LIGPT_insert(&light_pts, ray_b.x2, ray_b.y2, angle);

        // light polygon must have player positions as one of the conde
        LIGPT_insert(&light_pts, x, y, 0);
    }


    // if light width=0 (full radius), all obstacles are taken. No other calculations is needed
    else { filtered_obstacles = obstacles; }

    for (lightpoint_t* ptr = light_pts; ptr; ptr=ptr->next) {
        printf("%i, %i \n", ptr->x, ptr->y);
    }
    printf("\n");
    // for each of obstacle corner three rays is casted
    for(obstacle_t* s=filtered_obstacles; s; s=s->next) {
        break;
        angle = LIGPT_calculate_angle(x, y, s->x1, s->y1);

        ray_t main_ray = (ray_t){x, y, s->x1, s->y1};
        ray_t aux_ray1 = (ray_t){x, y, x - sin(angle + smol_angle) * R, y - cos(angle + smol_angle) * R};
        ray_t aux_ray2 = (ray_t){x, y, x - sin(angle - smol_angle) * R, y - cos(angle - smol_angle) * R};

        LIG_find_closest_intersection_with_wall(&main_ray, obstacles);
        // LIGPT_insert(&light_pts, main_ray.x2, main_ray.y2, angle);

        // LIG_find_closest_intersection_with_wall(&aux_ray1, obstacles);
        // LIGPT_insert(&light_pts, aux_ray1.x2, aux_ray1.y2, angle + smol_angle);

        // LIG_find_closest_intersection_with_wall(&aux_ray2, obstacles);
        // LIGPT_insert(&light_pts, aux_ray2.x2, aux_ray2.y2, angle - smol_angle);
    }

    // polygon point optimization process (deleting redundant points)
    LIGPT_optim(light_pts);

    if (DEBUG){LIG_debug_rays(light_pts, x, y, 200);}
     // if (DEBUG){LIG_debug_dark_sectors(light_pts);}

    return light_pts;
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

    lightpoint_t* light_polygon = NULL;

    for (; i < lght->src->n_poly; i++) {
        color = LIG_get_light_polygon_color(lght, i);
        x_corr = LIG_get_light_polygon_corr(lght, i, X);
        y_corr = LIG_get_light_polygon_corr(lght, i, Y);

        // calculating the light polygon shape :)
        light_polygon = LIG_calc_light_polygon(x+x_corr, y+y_corr, lght->angle, lght->src->width, obstacles);

        // drawing the light
        // LIG_fill_lightpoly(light_polygon, color);
    }
    OBS_free(obstacles);
};

void LIG_free(light_t* lght) {
    free(lght);
};
