#include "def.h"

// aux for GEO_pt_in_triangle function.
float GEO_sign (
    int x1, int y1,
    int x2, int y2,
    int x3, int y3
)

{
    return (x1 - x3) * (y2- y3) - (x2 - x3) * (y1 - y3);
}

// checks if given point is inside of the triangle
bool GEO_pt_in_triangle (
    int pt_x, int pt_y,    // point we`re looking for
    int t1_x, int t1_y,    // triangle
    int t2_x, int t2_y,    // triangle
    int t3_x, int t3_y     // triangle
)
{
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
float GEO_seg_intersection_with_y
(
    int y,                   // y-line coord
    int x1,      int y1,     // segment begginig
    int x2,      int y2      // segment end
)
{
    return x1 + (x2 - x1) * (y - y1) / (y2 - y1);
}

// Checks if y-line intersects with given segment. Please note that to do that only y-coord of
// segment is needed to be known. Function will work no matter what is the order of the segment
// points. Note that there is not strict condition from one side of segment end:
// (y1 < scan_y rather than y1 <= scan_y)
bool GEO_if_seg_intersect_with_y
(
    int scan_y,             // y-line coord
    int y1,                 // segment beg y-coords
    int y2                  // segment end y-coord
)
{
    return ((y1 < scan_y && y2 >= scan_y) || (y2 < scan_y && y1 >= scan_y));
}

// Calculates distance between two points. Simple equations is used, can be optimised for something
// better as this is only use to distinguish which point is closer to (0, 0) point.
float GEO_distance(
    int x1, int y1,         // first point
    int x2, int y2          // second point
)
{
    int dx = x1-x2;
    int dy = y1-y2;

    return sqrt(dx*dx + dy*dy);
}
