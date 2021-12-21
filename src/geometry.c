#include "global.h"


// aux for GEO_pt_in_triangle function.
float GEO_sign (
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
    return (float)x1 + ((float)((x2 - x1) * (y - y1)) / (y2 - y1));
}

// Checks where x-line intersect with given segment. As y coord of such point is known from the
// begginig - function only return x-coord of such intersection point.
float GEO_intersection_with_x
(
    int x,                   // y-line coord
    int x1,      int y1,     // segment begginig
    int x2,      int y2      // segment end
) {
    return (float)y1 + ((float)(y2 - y1) * (x - x1)) / (x2 - x1);
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

// Checks if y-line intersects with given segment. Please note that to do that only y-coord of
// segment is needed to be known. Function will work no matter what is the order of the segment
// points. Note that there is not strict condition from one side of segment end:
// (y1 < scan_y rather than y1 <= scan_y)
bool GEO_value_between_range (
    int value,
    int first,
    int second,
    float acc
) {
    return ((first-acc <= value && second+acc >= value) || (second-acc <= value && first+acc >= value));
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
        GEO_value_between_range(x1, r_x1, r_x2, STRICT_ACCURACY) && 
        (
            GEO_value_between_range(r_y1, y1, y2, STRICT_ACCURACY) ||
            GEO_value_between_range(r_y2, y1, y2, STRICT_ACCURACY))
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
        GEO_value_between_range(y1, r_y1, r_y2, STRICT_ACCURACY) && 
        (
            GEO_value_between_range(r_x1, x1, x2, STRICT_ACCURACY) ||
            GEO_value_between_range(r_x2, x1, x2, STRICT_ACCURACY))
        ) {
        return y1;
        }

    return -1;
}
