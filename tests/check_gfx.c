#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <check.h>
#include "../src/gfx.h"
#include "../src/segment.h"


START_TEST (GFX_init_window_check)
{
    // GIVEN
    SDL_Window* window = NULL;

    // WHEN
    window = GFX_init_window(100, 100);

    // THEN
    ck_assert_ptr_nonnull(window); // window is created sucessfully!

    // CLEANUP
    SDL_DestroyWindow(window);
}
END_TEST

START_TEST (GFX_init_renderer_check)
{
    // GIVEN
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    // WHEN
    window = GFX_init_window(100, 100);
    renderer = GFX_init_renderer(window);

    // THEN
    ck_assert_ptr_nonnull(renderer); // renderer is created sucessfully!

    // CLEANUP
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

}
END_TEST

START_TEST (GFX_init_graphics_check)
{
    // GIVEN
    int result;

    // WHEN
    result = GFX_init_graphics(100, 100);

    // THEN
    ck_assert_int_eq(result, 1);
    
    // CLEANUP
    GFX_free();
}
END_TEST
 

START_TEST (GFX_calc_intersections_in_scanline_check)
{
    // GIVEN
    int result;
    int y = 0;
    int n;
    int len = 0;
    int expected_len = 2;

    segment_t* segments = NULL;
    x_intersection_t* points = NULL;
    x_intersection_t* ptr = NULL;

    SEG_push(&segments, 287, 0, 256, 36);
    SEG_push(&segments, 320, 0, 287, 0);
    SEG_push(&segments, 320, 145, 320, 0);

    // WHEN
    result = GFX_init_graphics(100, 100);
    points = GFX_calc_intersections_in_scanline(segments, y, &n);
    
    ptr = points;

    while(ptr) {
        len++;
        ptr=ptr->next;
    }

    // THEN
    ck_assert_int_eq(len, expected_len);
    
    // CLEANUP
    GFX_free();
}
END_TEST

Suite *gfx_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("gfx");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, GFX_init_window_check);
    tcase_add_test(tc_core, GFX_init_renderer_check);
    tcase_add_test(tc_core, GFX_init_graphics_check);
    tcase_add_test(tc_core, GFX_calc_intersections_in_scanline_check);

    suite_add_tcase(s, tc_core);

    return s;
}
