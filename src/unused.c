// some unused junk in here

// Fills polygon with solid color. Polygon is cut into traingles, and each traingle is filled
// separatly.
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


// draws "inverse" of polygon to black. Here, not he polygon itself is filled rather than everything
// bout it
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
