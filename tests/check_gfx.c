#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <check.h>
#include "../src/gfx.h"


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

START_TEST (GFX_clear_screen_check)
{
    // GIVEN
    int w = 10;
    int h = 10;

    SDL_Surface *surface;
    GFX_init_graphics(w, h);
    SDL_PixelFormat *fmt;
    SDL_Color *color;
    Uint8 index;
    Uint8 r, g, b;

    // WHEN
    GFX_clear_screen();

    // THEN
    SDL_LockSurface(surface);
    fmt = surface->format;
    // index = *((Uint8*)surface->pixels);

    SDL_GetRGB(0, fmt, &r, &g, &b);
    SDL_UnlockSurface(surface);
    // printf("%d\n", r);

    // for (int i=0; i<w; i++)
    // {
    //     for (int j=0; j<h; j++)
    //     {
    //         color=&fmt->palette->colors[0];
    //         ck_assert_int_eq(color->r, 0);
    //         ck_assert_int_eq(color->g, 0);
    //         ck_assert_int_eq(color->b, 0);
    //         printf("Index: %d\n", pixel);
    //     }
    // }
    
    // CLEANUP
    GFX_free();
}
END_TEST


Suite * gfx_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("gfx");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, GFX_init_window_check);
    tcase_add_test(tc_core, GFX_init_renderer_check);
    tcase_add_test(tc_core, GFX_init_graphics_check);
    tcase_add_test(tc_core, GFX_clear_screen_check);

    suite_add_tcase(s, tc_core);

    return s;
}
