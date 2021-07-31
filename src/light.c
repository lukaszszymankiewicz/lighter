#include "geometry.h"
#include "gfx.h"
#include "light.h"
#include "intersection.h"
#include "lightpt.h"
#include "segment.h"
#include "sprites.h"


// light calculation and rendering implementation. Basically each lightsource creates some light
// rays which is used to calculate polygon where light can reach. Such polygon is drawn with higher
// alpha value which creates realisitc effect

#define R 1200
#define smol_angle 0.01

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
    .n_poly = 5,
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
// draws rays into every cone of the lightpolygon
void LIG_debug_rays(lightpoint_t* light_poly, int st_x, int st_y, int alpha)
{
    for(lightpoint_t* ptr = light_poly; ptr; ptr = ptr->next)
    { GFX_draw_colored_line(st_x, st_y, ptr->x, ptr->y, alpha, alpha, alpha, 255); }
}

// draws edges of light polygon
void LIG_debug_dark_sectors(lightpoint_t* light_poly)
{
    int first_x = light_poly->x;
    int first_y = light_poly->y;
    lightpoint_t* ptr = light_poly;

    for(; ptr->next; ptr = ptr->next)
    { GFX_draw_colored_line(ptr->x, ptr->y, ptr->next->x, ptr->next->y, 255, 255, 255, 255); }
    GFX_draw_colored_line(ptr->x, ptr->y, first_x, first_y, 255, 255, 255, 255);
}

// Function checks for any of the possible interseciton between single ray and set of segments
// (obstacles). Best (closest) intersection is then stored in ray segment end coordinates.
void LIG_find_closest_intersection_with_wall(
    segment_t* ray,
    segment_t* obstacles
)
{ 
    segment_t* hit_obstacle = NULL;
    hit_obstacle = obstacles;
    point_t* intersection = PT_init(ray->end.x, ray->end.y);
    
    float best_dist = GEO_distance(ray->beg.x, ray->beg.y, ray->end.x, ray->end.y);
    float new_dist;

    for(; hit_obstacle; hit_obstacle=hit_obstacle->next) 
    {
        if(SEG_intersects(*ray, *hit_obstacle, intersection))
        {
            new_dist = GEO_distance(ray->beg.x, ray->beg.y, intersection->x, intersection->y);
            if (new_dist < best_dist)
            {
                ray->end.x = intersection->x;
                ray->end.y = intersection->y;
                best_dist = new_dist;
            }
        }
    }

    free(intersection);
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

// Converts tiles of level to list of segments on which ray light can hit.
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
void LIG_base_light(int x, int y, float angle, float width, int color, segment_t* obstacles)
{ 
    segment_t* filtered_segs = NULL;
    lightpoint_t* light_pts = NULL;

    if (width)
    {
        // If light source has any width "border" there are two rays which is closing light polygon.
        // Below code calculates such border rays.
        // If this value is 0 light is casted on whole radius, and such border rays is not needed
        int ray_a_x = (int) x - sin(angle - width) * R;
        int ray_a_y = (int) y - cos(angle - width) * R;
        int ray_b_x = (int) x - sin(angle + width) * R;
        int ray_b_y = (int) y - cos(angle + width) * R;

        segment_t * ray_a = NULL;
        segment_t * ray_b = NULL;

        ray_a = SEG_init(x, y, ray_a_x, ray_a_y);
        ray_b = SEG_init(x, y, ray_b_x, ray_b_y);

        // we need to filter those level obstacles for which we will cast rays - points which is
        // inside triangle made from two border rays and player position is taken.
        for(segment_t* s=obstacles; s; s=s->next)
        {
            if (GEO_pt_in_triangle(s->beg.x, s->beg.y, x, y, ray_a->end.x, ray_a->end.y, ray_b->end.x, ray_b->end.y)) 
            {
                SEG_push(&filtered_segs, s->beg.x, s->beg.y, s->end.x, s->end.y);
            }
        }

        // check if border rays hit any obstacle - such intersection points will be added as light
        // poly cone
        LIG_find_closest_intersection_with_wall(ray_a, obstacles);
        angle = LIGPT_calculate_angle(x, y, ray_a->end.x, ray_a->end.y);
        LIGPT_insert(&light_pts, ray_a->end.x, ray_a->end.y, angle);

        LIG_find_closest_intersection_with_wall(ray_b, obstacles);
        angle = LIGPT_calculate_angle(x, y, ray_b->end.x, ray_b->end.y);
        LIGPT_insert(&light_pts, ray_b->end.x, ray_b->end.y, angle);

        // light polygon must have player positions as one of the conde
        LIGPT_insert(&light_pts, x, y, 0);

        SEG_free(ray_a);
        SEG_free(ray_b);
    }

    // if light width=0 (full radius), all obstacles are taken. No other calculations is needed
    else { filtered_segs = obstacles; }

    // for each of obstacle corner three rays is casted
    for(segment_t* s=filtered_segs; s; s=s->next)
    {
        angle = LIGPT_calculate_angle(x, y, s->beg.x, s->beg.y);

        segment_t * main_ray = NULL;
        segment_t * aux_ray1 = NULL;
        segment_t * aux_ray2 = NULL;

        main_ray = SEG_init(x, y, s->beg.x, s->beg.y);
        aux_ray1 = SEG_init(x, y, x - sin(angle + smol_angle) * R, y - cos(angle + smol_angle) * R);
        aux_ray2 = SEG_init(x, y, x - sin(angle - smol_angle) * R, y - cos(angle - smol_angle) * R);

        LIG_find_closest_intersection_with_wall(main_ray, obstacles);
        LIGPT_insert(&light_pts, main_ray->end.x, main_ray->end.y, angle);

        LIG_find_closest_intersection_with_wall(aux_ray1, obstacles);
        LIGPT_insert(&light_pts, aux_ray1->end.x, aux_ray1->end.y, angle + smol_angle);

        LIG_find_closest_intersection_with_wall(aux_ray2, obstacles);
        LIGPT_insert(&light_pts, aux_ray2->end.x, aux_ray2->end.y, angle - smol_angle);

        SEG_free(main_ray);
        SEG_free(aux_ray1);
        SEG_free(aux_ray2);
    }

    // polygon point optimization process (deleting redundant points)
    LIGPT_optim(light_pts);

    // drawing the light
    LIG_fill_lightpoly(light_pts, color);

    if (DEBUG){LIG_debug_rays(light_pts, x, y, 69);}
    if (DEBUG){LIG_debug_dark_sectors(light_pts);}

    LIGPT_free(light_pts);
};

// calculates and draws light polygons. Every of the light source needs several polygons to be drawn
// - every one of them is slightly moved to another which makes light looks more "natural".
// Furthermore, the polygons with bigger shift has more pale color resulting in overall effect
// looking like "gradient". Number of the polygons and shift coords are defined by its lightsource.
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

void LIG_corr_light_angle(light_t* lght, int frame)
{
    static bool sign;

    if ((frame % lght->src->wobble_change_dir_coef) == 0) { sign = !sign; }
    if (sign) { lght->angle += lght->src->wobble_corr; }
    else { lght->angle -= lght->src->wobble_corr; }
}

void LIG_draw_light_effect(int x, int y, light_t* lght, tiles_list_t * tiles, int frame)
{
    segment_t* obstacles = LIG_calculate_ray_obstacles(tiles);
    LIG_corr_light_angle(lght, frame);
    LIG_draw_light_polygons(x, y, lght, obstacles);
};

void LIG_free(light_t* lght)
{
    free(lght);
};
