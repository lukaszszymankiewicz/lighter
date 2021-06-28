#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"

// point are sorted by y increasingly, we could use full more sofisticated sorting, but here it is
// so basic that this task handled easily by f-conditions. Points should be sorted in a way that y1
// < y2 < y3 (x-coords are not taken into sorting).
void LVL_fill_triangle(int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b, int a){
    int swapx;
    int swapy;
    
    if (y1 > y3) {swapx=x3; swapy=y3; x3=x1; y3=y1; x1=swapx; y1=swapy;}
    if (y2 > y3) {swapx=x3; swapy=y3; x3=x2; y3=y2; x2=swapx; y2=swapy;}
    if (y1 > y2) {swapx=x2, swapy=y2; x2=x1; y2=y1; x1=swapx; y1=swapy;}
    
    for (int y=y1; y<y2; y++) {
        float linex1 = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
        float linex2 = x1 + (x3 - x1) * (y - y1) / (y3 - y1);
        LRE_draw_colored_line(linex1, y, linex2, y, r, g, b, a);
    }
    for (int y=y2; y<y3; y++) {
        float linex1 = x2 + (x3 - x2) * (y - y2) / (y3 - y2);
        float linex2 = x1 + (x3 - x1) * (y - y1) / (y3 - y1);
        LRE_draw_colored_line(linex1, y, linex2, y, r, g, b, a);
    }
};


// void LIG_fill_lightpoly(lightpoint_t* light_poly, int st_x, int st_y)
// {
//     lightpoint_t* ptr = light_poly;
// 
//     int last_x = ptr->x; // we will save it for later on
//     int last_y = ptr->y; // we will save it for later on
// 
//     // iterating by light polygon points and drawing triangle by triangle
//     while (ptr) {
//         int first_x = ptr->x; 
//         int first_y = ptr->y; 
//         ptr = ptr->next;
// 
//         if (ptr==NULL) {
//             // joining last cone with first patching the whole light poly in one.
//             LRE_fill_triangle(first_x, first_y, last_x, last_y, st_x, st_y, 50, 50, 50, 255);
//             break;
//         }
//         int second_x = ptr->x; 
//         int second_y = ptr->y; 
//         LRE_fill_triangle(first_x, first_y, second_x, second_y, st_x, st_y, 50, 50, 50, 255);
//     }
// }

