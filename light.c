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

float LIG_sign (int x1, int y1, int x2, int y2, int x3, int y3)
{
    return (x1 - x3) * (y2- y3) - (x2 - x3) * (y1 - y3);
}

bool LIG_point_in_triangle (
    point_t pt,
    point_t t1,
    point_t t2,
    point_t t3
)
{
    float d1, d2, d3;
    bool has_neg, has_pos;

    d1 = LIG_sign(pt.x, pt.y, t1.x, t1.y, t2.x, t2.y);
    d2 = LIG_sign(pt.x, pt.y, t2.x, t2.y, t3.x, t3.y);
    d3 = LIG_sign(pt.x, pt.y, t3.x, t3.y, t1.x, t1.y);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
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

// Function returns id of closest to hit wall
int LIG_find_closest_intersection_with_wall(
    segment_t * ray,
    segment_t * obstacles
)
{ 
    segment_t * hit_obstacle = NULL;
    hit_obstacle = obstacles;
    point_t * shortest_intersection = PT_init(ray->end.x, ray->end.y);
    
    // If resulted best ray will have wall id equal to zero it means that no intersection,
    // occures. Please note that such event is very unlikely as four corners of the level are
    // also taken into account when checking light collision. 
    int id = 0; 
    int closest_wall_id = 0;

    for(; hit_obstacle; hit_obstacle=hit_obstacle->next) 
    {
        id++;

        if(SEG_intersects(*ray, *hit_obstacle, shortest_intersection))
        {
            // check if newly found intersection point is better than current one, and by better
            // mean closer to ray begginig. As we found points which is always in one line, full
            // distance equations is redundant - we don`t need to check full disance as diff in
            // x coors is enough.
            if (abs(ray->beg.x - ray->end.x) > abs(ray->beg.x - shortest_intersection->x)) 
            {
                ray->end.x = shortest_intersection->x;
                ray->end.y = shortest_intersection->y;
                closest_wall_id = id;
            }
        }
    }

    free(shortest_intersection);

    return closest_wall_id;
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
void LIG_draw_lanternt_light_effect(light_t * light_o, point_t st, segment_t * obstacles)
{ 
    float angle = 0.0;
    int hit_wall_id = 0;
    float corr = 0.01;
    int big_r = 1200;    
                         
    lightpoint_t* light_pts = NULL;

    // light gradient texture on back 
    TXTR_render_texture(light_o->sprite, NULL, st.x-256, st.y-256);

    // for each corner of each segment rays are casted
    for(segment_t * corner = obstacles; corner; corner=corner->next) {
        angle = LIGPT_calculate_angle(st.x, st.y, corner->beg.x, corner->beg.y);

        segment_t * main_ray = NULL;
        main_ray = SEG_init(st.x, st.y, corner->beg.x, corner->beg.y);

        segment_t * aux_ray1 = NULL;
        aux_ray1 = SEG_init(st.x, st.y, st.x - sin(angle + corr) * big_r, st.y - cos(angle + corr) * big_r);

        segment_t * aux_ray2 = NULL;
        aux_ray2 = SEG_init(st.x, st.y, st.x - sin(angle - corr) * big_r, st.y - cos(angle - corr) * big_r);

        hit_wall_id = LIG_find_closest_intersection_with_wall(main_ray, obstacles);
        LIGPT_insert(&light_pts, main_ray->end.x, main_ray->end.y, angle, hit_wall_id);

        hit_wall_id = LIG_find_closest_intersection_with_wall(aux_ray1, obstacles);
        LIGPT_insert(&light_pts, aux_ray1->end.x, aux_ray1->end.y, angle+corr, hit_wall_id);

        hit_wall_id = LIG_find_closest_intersection_with_wall(aux_ray2, obstacles);
        LIGPT_insert(&light_pts, aux_ray2->end.x, aux_ray2->end.y, angle-corr, hit_wall_id);

        SEG_free(main_ray);
        SEG_free(aux_ray1);
        SEG_free(aux_ray2);
    }

    // polygon point optimization process
    LIGPT_optim(light_pts);

    // drawing the shadow (sound dark)
    LIG_draw_dark_sectors(light_pts);

    // comment debugs if not needed
    // if (DEBUG){LIG_fill_lightpoly(light_pts, st_x, st_y);}
    // if (DEBUG){LIG_debug_dark_sectors(light_pts);}
    // if (DEBUG){LIG_debug_rays(light_pts, st.x, st.y);}

    SEG_free(obstacles);
    LIGPT_free(light_pts);
};


void LIG_draw_flashlight_light_effect(light_t * light_o, point_t pos,  segment_t * obstacles) 
{ 
    float light_width = PI / 7;
    int hit_wall_id; 
    float corr = 0.01;            // coef for calculating how big is the shift from main ray
    float angle = light_o->angle; // base angle
    int big_r = 1200;             // base length of ray big enought to be sure that it will end abroad level 

    lightpoint_t* light_pts = NULL;

    // border rays which will create triangle of light
    int ray_a_x = (int) pos.x - sin(angle - light_width) * big_r;
    int ray_a_y = (int) pos.y - cos(angle - light_width) * big_r;
    int ray_b_x = (int) pos.x - sin(angle + light_width) * big_r;
    int ray_b_y = (int) pos.y - cos(angle + light_width) * big_r;

    // light gradient texture on back 
    TXTR_render_texture(light_o->sprite, NULL, pos.x-256, pos.y-256);

    // first of all, check for intersections between two border rays.
    segment_t * ray_a = NULL;
    segment_t * ray_b = NULL;

    ray_a  = SEG_init(pos.x, pos.y, ray_a_x, ray_a_y);
    ray_b  = SEG_init(pos.x, pos.y, ray_b_x, ray_b_y);

    for(segment_t * corner = obstacles; corner; corner=corner->next)
    {
        if (LIG_point_in_triangle(corner->beg, pos, ray_a->end, ray_b->end)) 
        {
            angle = LIGPT_calculate_angle(pos.x, pos.y, corner->beg.x, corner->beg.y);

            segment_t * main_ray = NULL;
            segment_t * aux_ray1 = NULL;
            segment_t * aux_ray2 = NULL;

            main_ray = SEG_init(pos.x, pos.y, corner->beg.x, corner->beg.y);
            aux_ray1 = SEG_init(pos.x, pos.y, pos.x - sin(angle + corr) * big_r, pos.y - cos(angle + corr) * big_r);
            aux_ray2 = SEG_init(pos.x, pos.y, pos.x - sin(angle - corr) * big_r, pos.y - cos(angle - corr) * big_r);

            hit_wall_id = LIG_find_closest_intersection_with_wall(main_ray, obstacles);
            LIGPT_insert(&light_pts, main_ray->end.x, main_ray->end.y, angle, hit_wall_id);

            hit_wall_id = LIG_find_closest_intersection_with_wall(aux_ray1, obstacles);
            LIGPT_insert(&light_pts, aux_ray1->end.x, aux_ray1->end.y, angle+corr, hit_wall_id);

            hit_wall_id = LIG_find_closest_intersection_with_wall(aux_ray2, obstacles);
            LIGPT_insert(&light_pts, aux_ray2->end.x, aux_ray2->end.y, angle-corr, hit_wall_id);

            SEG_free(main_ray);
            SEG_free(aux_ray1);
            SEG_free(aux_ray2);
        }

    }

    hit_wall_id = LIG_find_closest_intersection_with_wall(ray_a, obstacles);
    angle = LIGPT_calculate_angle(pos.x, pos.y, ray_a->end.x, ray_a->end.y);
    LIGPT_insert(&light_pts, ray_a->end.x, ray_a->end.y, angle, hit_wall_id);

    hit_wall_id = LIG_find_closest_intersection_with_wall(ray_b, obstacles);
    angle = LIGPT_calculate_angle(pos.x, pos.y, ray_b->end.x, ray_b->end.y);
    LIGPT_insert(&light_pts, ray_b->end.x, ray_b->end.y, angle, hit_wall_id);

    // lighter light polygon must have initial point as one of the corners
    LIGPT_insert(&light_pts, pos.x, pos.y, 0, 0);

    // polygon point optimization process
    LIGPT_optim(light_pts);

    // drawing the shadow (sound dark)
    LIG_draw_dark_sectors(light_pts);

    // if (DEBUG){LIG_debug_rays(light_pts, pos.x, pos.y);}
    // if (DEBUG){LIG_fill_lightpoly(light_pts, pos.x, pos.y);}

    SEG_free(obstacles);
    SEG_free(ray_a);
    SEG_free(ray_b);
    LIGPT_free(light_pts);
};

void (*functions[2])() = {LIG_draw_lanternt_light_effect, LIG_draw_flashlight_light_effect};

void LIG_draw_light_effect(light_t * light_o, int hero_x, int hero_y, tiles_list_t * tiles)
{
    // tiles from level needs to be converted into list of segments into which rays can hit
    point_t pos = (point_t){hero_x, hero_y};
    segment_t * obstacles = NULL;
    obstacles = LIG_calculate_ray_obstacles(tiles);

    functions[light_o->source](light_o, pos, obstacles);    
};

void LIG_change_source(int * light_source)
{
    *light_source = ((*light_source) + 1) % ALL;
}

void LIG_free(light_t * light_o)
{
    TXTR_free(light_o->sprite);
}
