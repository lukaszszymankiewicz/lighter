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
