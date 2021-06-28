#include <stdbool.h>
#include <math.h>
#include "gfx.h"
#include "config.h"
#include "light.h"
#include "segment.h"
#include "intersection.h"
#include "lightpt.h"

#define MAX(a,b) ((a) > (b) ? (a) : (b))  // yeah, good old ANSI C tricks here
#define MIN(a,b) ((a) < (b) ? (a) : (b))  // yeah, good old ANSI C tricks here

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

// for each corner of wall three ays are drawn (one directly into corner, one a little bit to left, 
// and one a little bit to right). This allows to calculate map pieces where there is high lightness
// and where there is only a little bit of it (rays which are a little bit offseted will mark it).
void LIG_draw_light_effect(int hero_x, int hero_y, tiles_list_t * tiles) { 

    // First of all tiles from level should be examined. All line segment on which ray hay must be
    // calculted
    segment_t * obstacles = NULL;
    obstacles = LIG_calculate_ray_obstacles(tiles);

    // each wall can be hit by maximum two rays (all rays in between are redundant), so array with 
    // n_wall * 2 points are ceated to store results and filled with empty data. After all point are
    // catched (and promoted if needed), list is then sort by its angle (to form a poly).
    lightpoint_t* light_pts = NULL;

    int obstacle_id = 0; 
    int shortest_x = 0;
    int shortest_y = 0;
    float corr = 0.01;
    int big_r = 1200;

    for(segment_t * corner = obstacles; corner; corner=corner->next) {

        // "base" ray - such rays will always at least hit its base end-point.
        float pl_angle = LIGPT_calculate_angle(hero_x, hero_y, corner->x1, corner->y1);

            // main ray
            LIG_find_shortest_ray(
                hero_x,                                    hero_y,
                corner->x1,                                corner->y1,
                &shortest_x,                               &shortest_y,
                obstacles,                                 &obstacle_id
            );
            LIGPT_insert(&light_pts, shortest_x, shortest_y, hero_x, hero_y, obstacle_id);

            // shifted ray - ray moved by a little angle from "base" ray - this simulates light
            // physics at low cost
            LIG_find_shortest_ray(
                hero_x,                                    hero_y,
                hero_x - sin(pl_angle + corr) * big_r,     hero_y - cos(pl_angle + corr) * big_r,
                &shortest_x,                               &shortest_y,
                obstacles,                                 &obstacle_id
            );
            LIGPT_insert(&light_pts, shortest_x, shortest_y, hero_x, hero_y, obstacle_id);

            // second shift shifted ray
            LIG_find_shortest_ray(
                hero_x,                                    hero_y,
                hero_x - sin(pl_angle - corr) * big_r,     hero_y - cos(pl_angle - corr) * big_r,
                &shortest_x,                               &shortest_y,
                obstacles,                                 &obstacle_id
            );
            LIGPT_insert(&light_pts, shortest_x, shortest_y, hero_x, hero_y, obstacle_id);

        }

    // LIGPT_optim(light_pts);
    LIG_draw_dark_sectors(light_pts);

    // LIG_fill_lightpoly(light_pts, hero_pos.x, hero_pos.y);
    // if (DEBUG){LIG_debug_dark_sectors(light_pts);}
    // if (DEBUG){LIG_debug_rays(light_pts, hero_x, hero_y);}

    SEG_free(obstacles);
    LIGPT_free(light_pts);
};

