#include "global.h"
#include "vertex.h"

#ifndef GEOMETRY_H
#define GEOMETRY_H

float GEO_angle_2pt(int ax, int ay, int bx, int by);
float GEO_intersection_with_y(int y, int x1, int y1, int x2, int y2);
float GEO_intersection_with_x(int x, int x1, int y1, int x2, int y2);
float GEO_x_intersection_with_slope(int y, int x1, int y1, float slope);
float GEO_calc_slope(int x1, int y1, int x2, int y2);
float GEO_line_segment_len(int x1, int y1, int x2, int y2);

int GEO_vertical_segment_intersects_rect(int x1, int y1, int x2, int y2, int r_x1, int r_y1, int r_x2, int r_y2);
int GEO_horizontal_segment_intersects_rect(int x1, int y1, int x2, int y2, int r_x1, int r_y1, int r_x2, int r_y2);
int GEO_seg_in_rect(int s_x1, int s_y1, int s_x2, int s_y2, int r_x1, int r_y1, int r_x2, int r_y2);

bool GEO_value_between_range(int value, int first, int second);
bool GEO_intersection(int x1, int y1, int x2, int y2, int r_x1, int r_y1, int r_x2, int r_y2);
bool GEO_pt_in_polygon(vertex_t* polygon, int x, int y);
bool GEO_pt_same_side (int r_x1, int r_y1, int r_x2, int r_y2, int o_x1, int o_y1, int o_x2, int o_y2);
bool GEO_collienar_segs_have_common_pt( int a1, int a2, int b1, int b2);
bool GEO_pt_in_triangle(int pt_x, int pt_y, int t1_x, int t1_y, int t2_x, int t2_y, int t3_x, int t3_y);
bool GEO_pt_in_rect(int x, int y, int x1, int y1, int x2, int y2);

#endif
