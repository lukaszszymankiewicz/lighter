#include <stdbool.h>
#include <math.h>
#include "config.h"
#include "gfx.h"
#include "light.h"
#include "intersection.h"
#include "lightpt.h"
#include "segment.h"
#include "sprites.h"

#define PI 3.1415
#define MAX(a,b) ((a) > (b) ? (a) : (b))  // yeah, good old ANSI C tricks here
#define MIN(a,b) ((a) < (b) ? (a) : (b)) 

enum light_sources {
    LANTERN = 0,
    FLASHLIGHT = 1,
    ALL = 2,
};

light_t * LIG_init()
{
    light_t* light_o = (light_t*)malloc(sizeof(light_t));

    light_o->source = LANTERN;
    light_o->sprite = TXTR_init_texture("sprites/gradient.png");
    light_o->angle = RIGHT_RAD;

    return light_o;
}

int LIG_lines_intersects(
    int x1,   int y1,     // first line
    int x2,   int y2,     // first line
    int x3,   int y3,     // second line
    int x4,   int y4,     // second line
    int *i_x, int *i_y    // intersection points
)
{
    float s_numer, t_numer, denom, t;
    float dx13, dy13;
    float dx21, dx43, dy21, dy43;

    dx21 = x2 - x1; dy21 = y2 - y1; dx43 = x4 - x3; dy43 = y4 - y3;
    denom = dx21 * dy43 - dx43 * dy21;

    if (denom == 0)
        return 0; // collinear

    bool denomPositive = denom > 0;

    dx13 = x1 - x3; dy13 = y1 - y3;
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
    if (i_x != NULL)
        *i_x = (int)x1 + (t * dx21);
    if (i_y != NULL)
        *i_y = (int)y1 + (t * dy21);

    return 1;
}

// rays are always cast from hero standing point so ray_x1 and ray_y1 wil be equal to hero pos
// ray_x2 and ray_y2 are corrds of current polygon point (for main ray)
// ray_x2 and ray_y2 are corrds offseted points behind the level (for aux rays), offeseted points
// are calculated from polar coordinates (we take big radius and calculate appripriate angle)
void LIG_find_shortest_ray(
    int ray_x1,                   int ray_y1,
    int ray_x2,                   int ray_y2,
    int * best_x,                 int * best_y,
    segment_t * obstacles,        int * obstacle_id
)
{ 
        segment_t * hit_obstacle = NULL;
        hit_obstacle = obstacles;

        // For first iteration we assume that base ray end is closest to ay begginig.
        int shortest_x = ray_x2;
        int shortest_y = ray_y2;
        int cur_x = shortest_x; 
        int cur_y = shortest_y;

        // If resulted best ray will have wall id equal to zero it means that no intersection,
        // occures. Please note that such event is very unlikely as four corners of the level are
        // also taken into account when chacking light collision. This value serves mostly as debug
        // value.
        int id = 0; 

        for(; hit_obstacle; hit_obstacle=hit_obstacle->next) 
        {
            id++;
            if (LIG_lines_intersects(
                ray_x1,           ray_y1,           shortest_x,       shortest_y,        // ray coords (end, beg)
                hit_obstacle->x1, hit_obstacle->y1, hit_obstacle->x2, hit_obstacle->y2,  // wall coords (end, beg)
                &cur_x,           &cur_y                                               ) // closest point coords 
            )
            {
                // check if newly found intersection point is better than current one, and by better
                // mean closer to ray begginig. As we found points which is always in one line, full
                // distance equations is redundant - we don`t need to check full disance as diff in
                // x coors is enough.
                if (abs(ray_x1 - cur_x) <= abs(ray_x1 - shortest_x)) 
                {
                    shortest_x = cur_x; shortest_y = cur_y; *obstacle_id = id; 
                }
            }
        }

    // clip to screen edges
    *best_x = MIN(MAX(0, (int)shortest_x), SCREEN_HEIGHT);
    *best_y = MIN(MAX(0, (int)shortest_y), SCREEN_WIDTH);
}

// scanline is always parallel to y-axis, so simplified equations can be applied. Thus we know that
// in that point scanline is actually intersect with (x1, y1), (x2, y2) segment. No further checks
// is needed.
float LIG_scanline_intersection
(
    int scan_y, 
    int x1,      int y1,
    int x2,      int y2
)
{
    return x1 + (x2 - x1) * (scan_y - y1) / (y2 - y1);
}

int LIG_segment_intersetcs_with_scanline
(
    int scan_y, int y1, int y2
)
{
    return ((y1 < scan_y && y2 >= scan_y) || (y2 < scan_y && y1 >= scan_y));
}

void LIG_draw_dark_sectors(lightpoint_t* pts)
{
    float x_inter;

    // Scan every y for seach of colision with light polygon.
    for (int scan_y=0; scan_y<SCREEN_HEIGHT; scan_y++) 
    {
        x_intersection_t* intersections = NULL;
        lightpoint_t* ptr = pts;
        int n=0;
        int first_x = ptr->x;
        int first_y = ptr->y;

        for(; ptr->next; ptr = ptr->next)
        {
            if (LIG_segment_intersetcs_with_scanline(scan_y, ptr->y, ptr->next->y))
            {
                x_inter = LIG_scanline_intersection(scan_y, ptr->x, ptr->y, ptr->next->x, ptr->next->y);
                INTSC_insert(&intersections, x_inter);
                n++;
            }
        }

        // last segment
        if (LIG_segment_intersetcs_with_scanline(scan_y, ptr->y, first_y))
        {
            x_inter = LIG_scanline_intersection(scan_y, ptr->x, ptr->y, first_x, first_y);
            INTSC_insert(&intersections, x_inter);
            n++;
        }

        // if no intersection found this scanline should be all black (abort drawing)
        if (n==0)
        { GFX_draw_colored_line(0, scan_y, SCREEN_WIDTH, scan_y, 0, 0, 0, 255); continue;}

        int last_x_intersection = INTSC_get_last(intersections);

        // dark segment from left side of screen to first intersection with scanline
        if (intersections->x > 1)
        { GFX_draw_colored_line(0, scan_y, intersections->x, scan_y, 0, 0, 0, 255);}

        // dark segment from right side of screen to last intersection with scanline
        if (last_x_intersection < SCREEN_WIDTH-1) 
        { GFX_draw_colored_line(last_x_intersection, scan_y, SCREEN_WIDTH, scan_y, 0, 0, 0, 255); }

        // rest of dark segments
        if (n>2)
        {
            intersections=intersections->next;

            // ugly, but we ommit using crypic next->next->next
            for (int z=1; z<n-1; z+=2)
            {
                GFX_draw_colored_line(intersections->x, scan_y, intersections->next->x, scan_y, 0, 0, 0, 255); 
                intersections=intersections->next->next;
            }
        }
        INTSC_free(intersections);
    }
}

void LIG_debug_rays(lightpoint_t* light_poly, int st_x, int st_y)
{
    for(lightpoint_t* ptr = light_poly; ptr; ptr = ptr->next)
    { GFX_draw_colored_line(st_x, st_y, ptr->x, ptr->y, 255, 255, 255, 255); }
}

void LIG_debug_dark_sectors(lightpoint_t* light_poly)
{
    int first_x = light_poly->x;
    int first_y = light_poly->y;
    lightpoint_t* ptr = light_poly;

    for(; ptr->next; ptr = ptr->next)
    { GFX_draw_colored_line(ptr->x, ptr->y, ptr->next->x, ptr->next->y, 255, 255, 255, 255); }
    GFX_draw_colored_line(ptr->x, ptr->y, first_x, first_y, 255, 255, 255, 255);
}

segment_t * LIG_calculate_ray_obstacles(tiles_list_t * tiles)
{
    segment_t * segments = NULL;
    tiles_list_t * ptr = NULL;
    ptr = tiles;

    while (ptr)
    {
        SEG_push(&segments, ptr->tile->x1, ptr->tile->y1, ptr->tile->x1, ptr->tile->y2);
        SEG_push(&segments, ptr->tile->x1, ptr->tile->y2, ptr->tile->x2, ptr->tile->y2);
        SEG_push(&segments, ptr->tile->x2, ptr->tile->y2, ptr->tile->x2, ptr->tile->y1);
        SEG_push(&segments, ptr->tile->x2, ptr->tile->y1, ptr->tile->x1, ptr->tile->y1);

        ptr=ptr->next;
    }
    return segments;
}

void LIG_fill_lightpoly(lightpoint_t* light_poly, int st_x, int st_y)
{
    lightpoint_t* ptr = light_poly;

    int last_x = ptr->x; // we will save it for later on
    int last_y = ptr->y; // we will save it for later on

    // iterating by light polygon points and drawing triangle by triangle
    while (ptr) {
        int first_x = ptr->x; 
        int first_y = ptr->y; 
        ptr = ptr->next;

        if (ptr==NULL) {
            // joining last cone with first patching the whole light poly in one.
            GFX_fill_triangle(first_x, first_y, last_x, last_y, st_x, st_y, 50, 50, 50, 255);
            break;
        }
        int second_x = ptr->x; 
        int second_y = ptr->y; 
        GFX_fill_triangle(first_x, first_y, second_x, second_y, st_x, st_y, 50, 50, 50, 255);
    }
}

// Lantern light sweeps ray all around (infinitly big circle). With distance power of light is
// weakening - we`re achieveing the this effect by drawing proper gradiented texture. Light collides
// with each wall or obstacle, and point where rays are colliding needs to be calculated.  For each
// corner of every obstacle three rays are casted (one directly into corner, one a little bit to
// left, and one a little bit to right). This allows to lower number of rays (which needs a lot of
// resources) to only these necessary. Each ray is than checked for collision, and shorted from
// player point of collision is saved. Each point of intersection found is than collected in sorted
// order (by angle it makes with player position) - such operations automatically creates points of
// polygon which is never intersecting with himself (which is totally fine, as we`re drawing light
// source originated in one point, such polygon cannot have any intersecting segments).
// After calculating all of these points, some of than can be eleminated, as it is redundant (all
// segments of such light polygon is created from segments of level, there is no any "new" line
// drawn at all). If there is more than two points hitting one segment next to each other all middle
// points can be ommited, we do this before drawing to save some computing power.  Having such
// point, we draw not the polygon itself but rather its inverse (and fill it black, as it is
// shadow). Having light texture on bottom, newly created shadow fills place where light cannot be
// casted.
void LIG_draw_lanternt_light_effect(light_t * light_o, int st_x, int st_y, segment_t * obstacles)
{ 
    int obstacle_id = 0; // each intersection points has id of segement in which its hits
    int shortest_x = 0;  // end of shortest ray coords stored here
    int shortest_y = 0;  // end of shortest ray coords stored here
    float corr = 0.01;   // coef for calculating how big is the shift from main ray
    float angle = 0.0;
    int big_r = 1200;    // base length of ray big enought to be sure that it will end abroad level 
                         // range. Having ray end such far away make that we must find shorted
                         // intersection on a way.

    lightpoint_t* light_pts = NULL;

    // light gradient texture on back 
    TXTR_render_texture(light_o->sprite, NULL, st_x-256, st_y-256);

    // for each corner of each segment rays are casted
    for(segment_t * corner = obstacles; corner; corner=corner->next) {
        angle = LIGPT_calculate_angle(st_x, st_y, corner->x1, corner->y1);

        // base ray - such ray will always at least hit its base end point
        LIG_find_shortest_ray(
            st_x,                                      st_y,
            corner->x1,                                corner->y1,
            &shortest_x,                               &shortest_y,
            obstacles,                                 &obstacle_id
        );
        LIGPT_insert(&light_pts, shortest_x, shortest_y, st_x, st_y, obstacle_id);

        // shifted ray - ray moved by a little angle from base ray
        LIG_find_shortest_ray(
            st_x,                                      st_y,
            st_x - sin(angle + corr) * big_r,          st_y - cos(angle + corr) * big_r,
            &shortest_x,                               &shortest_y,
            obstacles,                                 &obstacle_id
        );
        LIGPT_insert(&light_pts, shortest_x, shortest_y, st_x, st_y, obstacle_id);

        // second shift shifted ray
        LIG_find_shortest_ray(
            st_x,                                      st_y,
            st_x - sin(angle - corr) * big_r,          st_y - cos(angle - corr) * big_r,
            &shortest_x,                               &shortest_y,
            obstacles,                                 &obstacle_id
        );
        LIGPT_insert(&light_pts, shortest_x, shortest_y, st_x, st_y, obstacle_id);
    }

    // polygon point optimization process
    LIGPT_optim(light_pts);

    // drawing the shadow (sound dark)
    LIG_draw_dark_sectors(light_pts);

    // comment debugs if not needed
    // if (DEBUG){LIG_fill_lightpoly(light_pts, st_x, st_y);}
    // if (DEBUG){LIG_debug_dark_sectors(light_pts);}
    // if (DEBUG){LIG_debug_rays(light_pts, st_x, st_y);}

    SEG_free(obstacles);
    LIGPT_free(light_pts);
};

float LIG_sign (int x1, int y1, int x2, int y2, int x3, int y3)
{
    return (x1 - x3) * (y2- y3) - (x2 - x3) * (y1 - y3);
}

bool LIG_point_in_triangle (
    int pt_x, int pt_y,
    int x1, int y1,
    int x2, int y2,
    int x3, int y3
)
{
    float d1, d2, d3;
    bool has_neg, has_pos;

    d1 = LIG_sign(pt_x, pt_y, x1, y1, x2, y2);
    d2 = LIG_sign(pt_x, pt_y, x2, y2, x3, y3);
    d3 = LIG_sign(pt_x, pt_y, x3, y3, x1, y1);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

void LIG_draw_flashlight_light_effect(light_t * light_o, int st_x, int st_y, segment_t * obstacles) 
{ 
    float light_width = PI / 8;
    int obstacle_id = 0; 
    int shortest_x = 0;           // end of shortest ray coords stored here
    int shortest_y = 0;           // end of shortest ray coords stored here
    float corr = 0.01;            // coef for calculating how big is the shift from main ray
    float angle = light_o->angle; // base angle
    int big_r = 1200;             // base length of ray big enought to be sure that it will end abroad level 

    lightpoint_t* light_pts = NULL;

    // border rays which will create triangle of light
    int ray_a_x = (int) st_x - sin(angle - light_width) * big_r;
    int ray_a_y = (int) st_y - cos(angle - light_width) * big_r;
    int ray_b_x = (int) st_x - sin(angle + light_width) * big_r;
    int ray_b_y = (int) st_y - cos(angle + light_width) * big_r;

    // light gradient texture on back 
    TXTR_render_texture(light_o->sprite, NULL, st_x-256, st_y-256);

    // first of all, check for intersections between two border rays.
    // first border ray
    LIG_find_shortest_ray(
        st_x,                                      st_y,
        ray_a_x,                                   ray_a_y,
        &shortest_x,                               &shortest_y,
        obstacles,                                 &obstacle_id
    );
    LIGPT_insert(&light_pts, shortest_x, shortest_y, st_x, st_y, obstacle_id);

    // second border ray
    LIG_find_shortest_ray(
        st_x,                                      st_y,
        ray_b_x,                                   ray_b_y,
        &shortest_x,                               &shortest_y,
        obstacles,                                 &obstacle_id
    );
    LIGPT_insert(&light_pts, shortest_x, shortest_y, st_x, st_y, obstacle_id);

    for(segment_t * corner = obstacles; corner; corner=corner->next)
    {
        if (LIG_point_in_triangle(
            corner->x1,                                corner->y1,
            st_x,                                      st_y,
            ray_a_x,                                   ray_a_y,
            ray_b_x,                                   ray_b_y
        )) 
        {
            angle = LIGPT_calculate_angle(st_x, st_y, corner->x1, corner->y1);
            LIG_find_shortest_ray(
                st_x,                                      st_y,
                corner->x1,                                corner->y1,
                &shortest_x,                               &shortest_y,
                obstacles,                                 &obstacle_id
            );
            LIGPT_insert(&light_pts, shortest_x, shortest_y, st_x, st_y, 0);

            // first shift shifted ray
            LIG_find_shortest_ray(
                st_x,                                      st_y,
                st_x - sin(angle + corr) * big_r,          st_y - cos(angle + corr) * big_r,
                &shortest_x,                               &shortest_y,
                obstacles,                                 &obstacle_id
            );
            LIGPT_insert(&light_pts, shortest_x, shortest_y, st_x, st_y, obstacle_id);

            // second shift shifted ray
            LIG_find_shortest_ray(
                st_x,                                      st_y,
                st_x - sin(angle - corr) * big_r,          st_y - cos(angle - corr) * big_r,
                &shortest_x,                               &shortest_y,
                obstacles,                                 &obstacle_id
            );
            LIGPT_insert(&light_pts, shortest_x, shortest_y, st_x, st_y, obstacle_id);
        }

    }

    // lighter light polygon must have initial point as one of the corners
    LIGPT_insert(&light_pts, st_x, st_y, st_x, st_y, 0);

    // polygon point optimization process
    LIGPT_optim(light_pts);

    // drawing the shadow (sound dark)
    LIG_draw_dark_sectors(light_pts);

    SEG_free(obstacles);
    LIGPT_free(light_pts);

};

void (*functions[2])() = {LIG_draw_lanternt_light_effect, LIG_draw_flashlight_light_effect};

void LIG_draw_light_effect(light_t * light_o, int hero_x, int hero_y, tiles_list_t * tiles)
{
    // tiles from level needs to be converted into list of segments into which rays can hit
    segment_t * obstacles = NULL;
    obstacles = LIG_calculate_ray_obstacles(tiles);

    functions[light_o->source](light_o, hero_x, hero_y, obstacles);    
};

void LIG_change_source(int * light_source)
{
    *light_source = ((*light_source) + 1) % ALL;
}

void LIG_free(light_t * light_o)
{
    TXTR_free(light_o->sprite);
}
