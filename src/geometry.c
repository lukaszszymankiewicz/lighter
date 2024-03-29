#include <math.h>
#include <stdlib.h>

#include "global.h"
#include "point.h"
#include "segment.h"
#include "sorted_list.h"

float GEO_angle_2pt(
    int ax,
    int ay,
    int bx,
    int by
) {
    return atan2(ax - bx, ay - by);
}

int GEO_sign (
    int x1, int y1,
    int x2, int y2,
    int x3, int y3
) {
    return (x1 - x3) * (y2- y3) - (x2 - x3) * (y1 - y3);
}

int GEO_int_sign(
    int a,
    int b
) {
    int res = a-b;

    if (res < 0) {
        return -1;
    } else if (res >0) {
        return 1;
    } else {
         return 0; 
    }
}

// checks if given point is inside of the triangle
bool GEO_pt_in_triangle (
    int pt_x, int pt_y,    // point we`re looking for
    int t1_x, int t1_y,    // triangle
    int t2_x, int t2_y,    // triangle
    int t3_x, int t3_y     // triangle
) {
    float d1, d2, d3;
    bool has_neg, has_pos;

    d1 = GEO_sign(pt_x, pt_y, t1_x, t1_y, t2_x, t2_y);
    d2 = GEO_sign(pt_x, pt_y, t2_x, t2_y, t3_x, t3_y);
    d3 = GEO_sign(pt_x, pt_y, t3_x, t3_y, t1_x, t1_y);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

// Checks where y-line intersect with given segment. As y coord of such point is known from the
// begginig - function only return x-coord of such intersection point.
float GEO_intersection_with_y
(
    int y,                   // y-line coord
    int x1,      int y1,     // segment begginig
    int x2,      int y2      // segment end
) {
    float meter = (x2 - x1) * (y - y1);
    float denominator = y2 - y1;

    if (denominator == 0) {
        return (float)x1;
    }

    return (float)x1 + (meter / denominator);
}

// Checks where x-line intersect with given segment. As y coord of such point is known from the
// begginig - function only return x-coord of such intersection point.
float GEO_intersection_with_x
(
    int x,                   // x-line coord
    int x1,      int y1,     // segment begginig
    int x2,      int y2      // segment end
) {

    float meter = (float)((y2 - y1) * (x - x1));
    float denominator = x2 - x1;

    if (denominator == 0) {
        return (float)x1;
    }

    return (float)y1 + (meter / denominator);
}

// Checks where y-line intersect with given segment. As y coord of such point is known from the
// begginig - function only return x-coord of such intersection point.
float GEO_x_intersection_with_slope
(
    int   y,
    int   x1,      
    int   y1,
    float slope
) {
    return x1 + (y - y1) * slope;
}

// Calculates slope of given segment
float GEO_calc_slope(
    int x1, int y1,
    int x2, int y2
) {
    if (x1 == x2) {
        return 0.0;
    }

    return (float)(x2 - x1) / (float)(y2 - y1);
}

bool GEO_value_between_range (
    int value,
    int first,
    int second
) {
    return ((first <= value && second >= value) || (second <= value && first >= value));
}

bool GEO_pt_in_rect(
    int x, int y,
    int x1, int y1,
    int x2, int y2
) {
    return (GEO_value_between_range (x, x1, x2) && GEO_value_between_range (y, y1, y2));
}

// returns -1 if there is not collision or x-value of the collision 
int GEO_vertical_segment_intersects_rect (
    int x1,     // segment
    int y1,     // segment
    int x2,     // segment
    int y2,     // segment
    int r_x1,   // rect
    int r_y1,   // rect
    int r_x2,   // rect
    int r_y2    // rect
) {
    if (
        GEO_value_between_range(x1, r_x1, r_x2) && 
        (
            GEO_value_between_range(r_y1, y1, y2) ||
            GEO_value_between_range(r_y2, y1, y2))
        ) {
        return x1;
    }

    return -1;
}

// returns -1 if there is not collision or y-value of the collision 
int GEO_horizontal_segment_intersects_rect (
    int x1,     // segment
    int y1,     // segment
    int x2,     // segment
    int y2,     // segment
    int r_x1,   // rect
    int r_y1,   // rect
    int r_x2,   // rect
    int r_y2    // rect
) {

    if (
        GEO_value_between_range(y1, r_y1, r_y2) && 
        (
            GEO_value_between_range(r_x1, x1, x2) ||
            GEO_value_between_range(r_x2, x1, x2))
        ) {
        return y1;
        }

    return -1;
}

// checks if segment has its end on different side than its begginig (border is defined by segment)
// if lines are collinear function always returns false (such case should be handled elsewhere)
bool GEO_pt_same_side (
    int r_x1, int r_y1,    // beg pt
    int r_x2, int r_y2,    // end pt
    int o_x1, int o_y1,    // segment plane
    int o_x2, int o_y2     // segment plane
) {
    int first, second;

    first = GEO_sign(o_x1, o_y1, r_x1, r_y1, r_x2, r_y2);
    second = GEO_sign(o_x2, o_y2, r_x1, r_y1, r_x2, r_y2);

    // normal case
    if (first && second) {
        return (first ^ second) >= 0;
    }

    // corner case - ray hits end of obstacle and collinear
    return false;
}

float GEO_line_segment_len( 
    int x1, int y1,
    int x2, int y2
) {
    int dx = x2-x1;
    int dy = y2-y1;

    return sqrt(dy*dy + dx*dx);
}

bool GEO_collienar_segs_have_common_pt(
    int a1, int a2, // first segment value
    int b1, int b2 // second segment value
) {
    
    bool first = ((GEO_value_between_range(a1, b1, b2)) || (GEO_value_between_range(a2, b1, b2)));
    bool second = ((GEO_value_between_range(b1, a1, a2)) || (GEO_value_between_range(b1, a1, a2)));

    return first || second;
}

bool GEO_collienar_segs_have_common_start(
    int a1, int a2, // first segment value
    int b1, int b2 // second segment value
) {
    return (a1 == b1 || a2 == b1 || a2 == b2);
}

int GEO_seg_in_rect(
    int s_x1, int s_y1,
    int s_x2, int s_y2,
    int r_x1, int r_y1,
    int r_x2, int r_y2
) {
    if (s_x1 == s_x2) {
        return GEO_vertical_segment_intersects_rect(
           s_x1, s_y1,
           s_x2, s_y2,
           r_x1, r_y1,
           r_x2, r_y2
        );
    } else {
        return GEO_horizontal_segment_intersects_rect(
           s_x1, s_y1,
           s_x2, s_y2,
           r_x1, r_y1,
           r_x2, r_y2
        );
    }
}

bool GEO_pt_in_hor_seg(
    int x, int y,
    int x1, int y1,
    int x2, int y2
) {

    if ((x1 == x && y1 == y) || (x2 == x && y2 == y)) {
        return true; 
    }

    if (y1 == y2 && y1 == y) {
        return GEO_value_between_range(x, x1, x2);
    }

    return false;
}

sorted_list_t* GEO_fill_intersection(
    sorted_list_t* intersections,
    int x,  int y,
    int x1, int y1,
    int x2, int y2
) {
    int inter;
    int sign;

    if (GEO_value_between_range(y, y1, y2)) {
        inter = (int)GEO_intersection_with_y(y, x1, y1, x2, y2);
        sign = GEO_int_sign(y2, y1);
        SRTLST_insert(&intersections, inter, sign);
    }

    return intersections;
}

// checks if segment intersects with another segment
bool GEO_segment_intersect(
    int o_x1,  // obstacle
    int o_y1,  // obstacle
    int o_x2,  // obstacle
    int o_y2,  // obstacle
    int r_x1,  // ray
    int r_y1,  // ray
    int r_x2,  // ray
    int r_y2   // ray
) {
    // check for obstacle is collinear with ray - special case here needs to be applied
    if (o_x1 == o_x2) {
        // collinear
        if ((r_x1 == r_x2) && (r_x1 == o_x1)) {
            return GEO_collienar_segs_have_common_pt(o_y1, o_y2, r_y1, r_y2);
        }
        // typical case
        return (GEO_value_between_range(o_x1, r_x1, r_x2)) &&
            !GEO_pt_same_side(r_x1, r_y1, r_x2, r_y2, o_x1, o_y1, o_x2, o_y2);

    } else {
        // collinear
        if ((r_y1 == r_y2) && (r_y1 == o_y1)) {
            return GEO_collienar_segs_have_common_pt(o_x1, o_x2, r_x1, r_x2);
        }
        // typical case
        return (GEO_value_between_range(o_y1, r_y1, r_y2) && 
         !GEO_pt_same_side(r_x1, r_y1, r_x2, r_y2, o_x1, o_y1, o_x2, o_y2));
    }
}

int GEO_find_common_point(
    int a1, int a2,
    int b1, int b2
) {
    if (a1 == b1) { return a1; }
    else if (a1 == b2) { return a1; }
    else if (a2 == b1) { return a2; }
    else return a2;
}

point_t* GEO_intersection_with_segment(
    segment_t* seg,
    int x1, int y1,
    int x2, int y2
) {
    if (!GEO_segment_intersect(seg->x1, seg->y1, seg->x2, seg->y2, x1, y1, x2, y2)) {
        return NULL;
    }

    // VERTICAL
    if (seg->type == VER) {

        // collinear
        if (seg->x1 == x1 && seg->x2 == x2) {

            if (!GEO_collienar_segs_have_common_start(seg->y1, seg->y2, y1, y2)) {
                return PT_new(seg->x1, y1);
            }
            int common_y = GEO_find_common_point(seg->y1, seg->y2, y1, y2);

            return PT_new(x1, common_y);
        }

        int inter_y = (int) GEO_intersection_with_x (seg->x1, x1, y1, x2, y2);
        return PT_new(seg->x1, inter_y);

    // HORIZONTAL
    } else {
        // collinear
        if (seg->y1 == y1 && seg->y2 == y2) {

            if (!GEO_collienar_segs_have_common_start(seg->x1, seg->x2, x1, x2)) {
                return PT_new(x1, seg->y1);
            }

            int common_x = GEO_find_common_point(seg->x1, seg->x2, x1, x2);
            return PT_new(common_x, y1);
        }

        int inter_x = (int) GEO_intersection_with_y (seg->y1, x1, y1, x2, y2);
        return PT_new(inter_x, seg->y1);
    }
}
