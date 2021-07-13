#include <check.h>
#include <stdlib.h>
#include "../src/sprites.h"


START_TEST (TXTR_init_texture_check)
{
    // GIVEN 
    // TODO: make it automatic to scan all sprites in folder, now it is lame as it can get
    int len = 2;
    char * filepaths[len];
    
    filepaths[0] = "../sprites/gradient.png";
    filepaths[1] = "../sprites/hero.png";

    // WHEN
    for (int i=0; i<len; i++)
    {
        // THEN
        Texture * cur_texture = NULL;
        cur_texture = TXTR_init_texture(filepaths[i]);
        ck_assert_ptr_nonnull(cur_texture);

        // CLEANUP
        TXTR_free(cur_texture);
    }

}
END_TEST

Suite * sprites_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("sprites");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, TXTR_init_texture_check);
    suite_add_tcase(s, tc_core);

    return s;
}
