#include <stdbool.h>
#include <math.h>
#include "config.h"
#include "geometry.h"
#include "gfx.h"
#include "light.h"
#include "intersection.h"
#include "lightpt.h"
#include "segment.h"
#include "sprites.h"

#define R 1200
#define smol_angle 0.01
#define PI 3.1415

lightsource_t lantern = 
{
    .width = 0.0,
    .n_poly = 13,
    .polys = {
        {-20,     -20,       20    },
        { 20,     -20,       20    },
        {-20,     -20,       20    },
        {-20,      20,       20    },
        {-12,     -12,       50    },
        { 12,     -12,       50    },
        {-12,     -12,       50    },
        {-12,      12,       50    },
        {-5,       -5,      100    },
        { 5,       -5,      100    },
        {-5,       -5,      100    },
        {-5,        5,      100    },
        { 0,        0,      200    },
    }
};

lightsource_t lighter = 
{
    .width = PI / 7,
    .n_poly = 7,
    .polys = {
        {-20,       0,       20    },
        { 20,       0,       20    },
        {-10,       0,       50    },
        { 10,       0,       50    },
        { -5,       0,       100   },
        {  5,       0,       100   },
        {  0,       0,       200   },
    }
};

lightsource_t* lightsources[] = {&lantern, &lighter};
light_t * LIG_init()
{
    light_t* light_o = (light_t*)malloc(sizeof(light_t));

    light_o->src_num = LANTERN;
    light_o->src = lightsources[light_o->src_num];
    light_o->angle = LEFT_RAD;

    return light_o;
}

// DEBUG function
void LIG_debug_rays(lightpoint_t* light_poly, int st_x, int st_y, int alpha)
{
    for(lightpoint_t* ptr = light_poly; ptr; ptr = ptr->next)
    { GFX_draw_colored_line(st_x, st_y, ptr->x, ptr->y, alpha, alpha, alpha, 255); }
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

// Function checks for any of the possible interseciton between rays and set of segments
// (obstacles). As a result function id of best (closest) wall to be hit by given ray. If function
// return 0 it means that ray does not hit any of the given segments.
int LIG_find_closest_intersection_with_wall(
    segment_t * ray,
    segment_t * obstacles
)
{ 
    segment_t * hit_obstacle = NULL;
    hit_obstacle = obstacles;
    point_t * intersection = PT_init(ray->end.x, ray->end.y);
    
    // If resulted best ray will have wall id equal to zero it means that no intersection,
    // occures. Please note that such event is very unlikely as four ss of the level are
    // also taken into account when checking light collision. 
    int id = 0; 
    int closest_wall_id = 0;
    float best_dist = GEO_distance(ray->beg.x, ray->beg.y, ray->end.x, ray->end.y);
    float new_dist;

    for(; hit_obstacle; hit_obstacle=hit_obstacle->next) 
    {
        id++;

        if(SEG_intersects(*ray, *hit_obstacle, intersection))
        {
            new_dist = GEO_distance(ray->beg.x, ray->beg.y, intersection->x, intersection->y);
            if (new_dist < best_dist)
            {
                ray->end.x = intersection->x;
                ray->end.y = intersection->y;
                closest_wall_id = id;
                best_dist = new_dist;
            }
        }
    }

    free(intersection);

    return closest_wall_id;
}

void LIG_fill_lightpoly(lightpoint_t* pts, int clr)
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

        // if no intersection found this scanline should be all black (abort drawing)
        if (n==0)
        { continue;}

        // if there is two intersection just draw line between them
        if (n==2)
        {
            GFX_draw_colored_line(intersections->x, scan_y, intersections->next->x, scan_y, clr, clr, clr, 255);
            continue;
        }

        // rest of dark segments
        if (n>2)
        {
            while(intersections)
            {
                GFX_draw_colored_line(intersections->x, scan_y, intersections->next->x, scan_y, clr, clr, clr, 255); 
                intersections=intersections->next->next;
            }
        }
        INTSC_free(intersections);
    }
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

void LIG_fill_lightpoly_with_triangles(
    lightpoint_t* light_poly,
    int st_x,
    int st_y,
    int alpha
)
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
            GFX_fill_triangle(first_x, first_y, last_x, last_y, st_x, st_y, alpha, alpha, alpha, 255);
            break;
        }
        int second_x = ptr->x; 
        int second_y = ptr->y; 
        GFX_fill_triangle(first_x, first_y, second_x, second_y, st_x, st_y, alpha, alpha, alpha, 255);
    }
}

// Lantern light sweeps ray all around (infinitly big circle). With distance power of light is
// weakening - we`re achieveing the this effect by drawing proper gradiented texture. Light collides
// with each wall or obstacle, and point where rays are colliding needs to be calculated.  For each
// s of every obstacle three rays are casted (one directly into s, one a little bit to
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
void LIG_base_light(int x, int y, float angle, float width, int color, segment_t* obstacles)
{ 
    int hit_wall_id; 

    segment_t* filtered_segs = NULL;
    lightpoint_t* light_pts = NULL;

    if (width)
    {
        // If light source has any width such ray is casted. If this value is 0, light is casted on
        // whole radius, and such border is not needed
        int ray_a_x = (int) x - sin(angle - width) * R;
        int ray_a_y = (int) y - cos(angle - width) * R;
        int ray_b_x = (int) x - sin(angle + width) * R;
        int ray_b_y = (int) y - cos(angle + width) * R;

        segment_t * ray_a = NULL;
        segment_t * ray_b = NULL;

        ray_a = SEG_init(x, y, ray_a_x, ray_a_y);
        ray_b = SEG_init(x, y, ray_b_x, ray_b_y);

        // we need to filter those level obstacles for which we will cast rays
        for(segment_t* s = obstacles; s; s=s->next)
        {
            if (GEO_pt_in_triangle(s->beg.x, s->beg.y, x, y, ray_a->end.x, ray_a->end.y, ray_b->end.x, ray_b->end.y)) 
            {
                SEG_push(&filtered_segs, s->beg.x, s->beg.y, s->end.x, s->end.y);
            }
        }

        // check if border rays hit any obstacle (such intersection points will be added as light poly
        // cone)
        hit_wall_id = LIG_find_closest_intersection_with_wall(ray_a, obstacles);
        angle = LIGPT_calculate_angle(x, y, ray_a->end.x, ray_a->end.y);
        LIGPT_insert(&light_pts, ray_a->end.x, ray_a->end.y, angle, hit_wall_id);

        hit_wall_id = LIG_find_closest_intersection_with_wall(ray_b, obstacles);
        angle = LIGPT_calculate_angle(x, y, ray_b->end.x, ray_b->end.y);
        LIGPT_insert(&light_pts, ray_b->end.x, ray_b->end.y, angle, hit_wall_id);

        // light polygon must have initial point if have any width (cone shape needs to be achieved)
        LIGPT_insert(&light_pts, x, y, 0, 0);

        SEG_free(ray_a);
        SEG_free(ray_b);
    }

    //if light is full radius we just assume that we take all obstacles)
    else { filtered_segs = obstacles; }

    for(segment_t* s=filtered_segs; s; s=s->next)
    {
        angle = LIGPT_calculate_angle(x, y, s->beg.x, s->beg.y);

        segment_t * main_ray = NULL;
        segment_t * aux_ray1 = NULL;
        segment_t * aux_ray2 = NULL;

        main_ray = SEG_init(x, y, s->beg.x, s->beg.y);
        aux_ray1 = SEG_init(x, y, x - sin(angle + smol_angle) * R, y - cos(angle + smol_angle) * R);
        aux_ray2 = SEG_init(x, y, x - sin(angle - smol_angle) * R, y - cos(angle - smol_angle) * R);

        hit_wall_id = LIG_find_closest_intersection_with_wall(main_ray, obstacles);
        LIGPT_insert(&light_pts, main_ray->end.x, main_ray->end.y, angle, hit_wall_id);

        hit_wall_id = LIG_find_closest_intersection_with_wall(aux_ray1, obstacles);
        LIGPT_insert(&light_pts, aux_ray1->end.x, aux_ray1->end.y, angle + smol_angle, hit_wall_id);

        hit_wall_id = LIG_find_closest_intersection_with_wall(aux_ray2, obstacles);
        LIGPT_insert(&light_pts, aux_ray2->end.x, aux_ray2->end.y, angle - smol_angle, hit_wall_id);

        SEG_free(main_ray);
        SEG_free(aux_ray1);
        SEG_free(aux_ray2);
    }

    // polygon point optimization process
    LIGPT_optim(light_pts);

    // drawing the light
    LIG_fill_lightpoly(light_pts, color);

    // if (DEBUG){LIG_debug_rays(light_pts, pos.x, pos.y);}
    // if (DEBUG){LIG_fill_lightpoly(light_pts, pos.x, pos.y);}

    LIGPT_free(light_pts);
};

void LIG_draw_light_polygons(int x, int y, light_t* lght, segment_t* obstacles) 
{ 
    for (int i=0; i < lght->src->n_poly; i++)
    {
        LIG_base_light(
            x + lght->src->polys[i][X],
            y + lght->src->polys[i][Y],
            lght->angle,
            lght->src->width,
            lght->src->polys[i][COLOR],
            obstacles
        );
    }
    SEG_free(obstacles);
}

void LIG_change_source(light_t* lght)
{
    lght->src_num = ((lght->src_num) + 1) % ALL;
    lght->src = lightsources[lght->src_num];
}

void LIG_draw_light_effect(int x, int y, light_t* lght, tiles_list_t * tiles)
{
    // tiles from level needs to be converted into list of segments into which rays can hit
    segment_t * obstacles = LIG_calculate_ray_obstacles(tiles);
    LIG_draw_light_polygons(x, y, lght, obstacles);
};
