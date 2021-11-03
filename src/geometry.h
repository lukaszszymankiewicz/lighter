#include "def.h"

#ifndef GEOMETRY_H
#define GEOMETRY_H

bool GEO_pt_in_triangle(int pt_x, int pt_y, int t1_x, int t1_y, int t2_x, int t2_y, int t3_x, int t3_y);
bool GEO_if_y_intersect_with_slope(int scan_y, int y1, int y2);
float GEO_intersection_with_y(int y, int x1, int y1, int x2, int y2);
float GEO_intersection_with_x(int x, int x1, int y1, int x2, int y2);
float GEO_distance(int x1, int y1, int x2, int y2);
float GEO_x_intersection_with_slope(int y, int x1, int y1, float slope);
float GEO_calc_slope(int x1, int y1, int x2, int y2);
bool GEO_value_between_range(int value, int first, int second);

#endif
