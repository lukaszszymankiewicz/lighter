#include "global.h"

// calculates angle between two points
float GEO_angle_2pt(int ax, int ay, int bx, int by) {
    return atan2(ax - bx, ay - by);
}

int GEO_sign (
    int x1, int y1,
    int x2, int y2,
    int x3, int y3
) {
    return (x1 - x3) * (y2- y3) - (x2 - x3) * (y1 - y3);
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
    if (GEO_value_between_range (x, x1, x2)) {
        return true;
    }

    if (GEO_value_between_range (y, y1, y2)) {
        return true;
    }

    return false;
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

// we dont need collinear coord in this function. Client is responsbile for checking and inputting
// proper coords to this function
bool GEO_collienar_segs_have_common_pt(
    int a1, int a2, // first segment value
    int b1, int b2 // second segment value
) {
    
    bool first = ((GEO_value_between_range(a1, b1, b2)) || (GEO_value_between_range(a2, b1, b2)));
    bool second = ((GEO_value_between_range(b1, a1, a2)) || (GEO_value_between_range(b1, a1, a2)));

    return first || second;
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
