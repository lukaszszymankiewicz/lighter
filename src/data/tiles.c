#include <stdbool.h>

#include "library.h"

tile_blueprint_t *tiles_library[TILE_ALL] = {NULL};

tile_blueprint_t tile_a_lu = {
    TILE_A_LU, TILESET_SEWER, 0, 0, OBSTACLE_FALSE
};

tile_blueprint_t tile_a_mu = {
    TILE_A_MU, TILESET_SEWER, 1, 0, OBSTACLE_FALSE
};

tile_blueprint_t tile_a_ru = {
    TILE_A_RU, TILESET_SEWER, 2, 0, OBSTACLE_FALSE 
};

tile_blueprint_t tile_a_lm = {
    TILE_A_LM, TILESET_SEWER, 0, 1, OBSTACLE_FALSE
};

tile_blueprint_t tile_a_mm = {
    TILE_A_MM, TILESET_SEWER, 1, 1, OBSTACLE_TRUE
};

tile_blueprint_t tile_a_rm = {
    TILE_A_RM, TILESET_SEWER, 2, 1, OBSTACLE_FALSE 
};

tile_blueprint_t tile_a_ld = {
    TILE_A_LD, TILESET_SEWER, 0, 2, OBSTACLE_FALSE
};

tile_blueprint_t tile_a_md = {
    TILE_A_MD, TILESET_SEWER, 1, 2, OBSTACLE_FALSE
};

tile_blueprint_t tile_a_rd = {
    TILE_A_RD, TILESET_SEWER, 2, 2, OBSTACLE_FALSE 
};

tile_blueprint_t tile_b_lu = {
    TILE_B_LU, TILESET_SEWER, 0, 3, OBSTACLE_FALSE
};

tile_blueprint_t tile_b_mu = {
    TILE_B_MU, TILESET_SEWER, 1, 3, OBSTACLE_FALSE
};

tile_blueprint_t tile_b_ru = {
    TILE_B_RU, TILESET_SEWER, 2, 3, OBSTACLE_FALSE 
};

tile_blueprint_t tile_b_lm = {
    TILE_B_LM, TILESET_SEWER, 0, 4, OBSTACLE_FALSE
};

tile_blueprint_t tile_b_mm = {
    TILE_B_MM, TILESET_SEWER, 1, 4, OBSTACLE_FALSE
};

tile_blueprint_t tile_b_rm = {
    TILE_B_RM, TILESET_SEWER, 2, 4, OBSTACLE_FALSE 
};

tile_blueprint_t tile_b_ld = {
    TILE_B_LD, TILESET_SEWER, 0, 5, OBSTACLE_FALSE
};

tile_blueprint_t tile_b_md = {
    TILE_B_MD, TILESET_SEWER, 1, 5, OBSTACLE_FALSE
};

tile_blueprint_t tile_b_rd = {
    TILE_B_RD, TILESET_SEWER, 2, 5, OBSTACLE_FALSE 
};

tile_blueprint_t tile_c_l = {
    TILE_C_L, TILESET_SEWER, 3, 4, OBSTACLE_FALSE
};

tile_blueprint_t tile_c_m = {
    TILE_C_M, TILESET_SEWER, 4, 4, OBSTACLE_FALSE
};

tile_blueprint_t tile_c_r = {
    TILE_C_R, TILESET_SEWER, 5, 4, OBSTACLE_FALSE 
};

tile_blueprint_t tile_sewer = {
    TILE_SEWER, TILESET_SEWER, 4, 4, OBSTACLE_FALSE 
};

void LIB_read_tiles_blueprint(
    tile_blueprint_t* blueprint
) {
    if (blueprint) {
        printf("TILE BLUEPRINT ID = %d read\n", blueprint->id);
    }
    tiles_library[blueprint->id]      = blueprint;
};

void LIB_create_tile_library(
) {
    LIB_read_tiles_blueprint(&tile_a_lu);
    LIB_read_tiles_blueprint(&tile_a_mu);
    LIB_read_tiles_blueprint(&tile_a_ru);
    LIB_read_tiles_blueprint(&tile_a_lm);
    LIB_read_tiles_blueprint(&tile_a_mm);
    LIB_read_tiles_blueprint(&tile_a_rm);
    LIB_read_tiles_blueprint(&tile_a_ld);
    LIB_read_tiles_blueprint(&tile_a_md);
    LIB_read_tiles_blueprint(&tile_a_rd);
    LIB_read_tiles_blueprint(&tile_b_lu);
    LIB_read_tiles_blueprint(&tile_b_mu);
    LIB_read_tiles_blueprint(&tile_b_ru);
    LIB_read_tiles_blueprint(&tile_b_lm);
    LIB_read_tiles_blueprint(&tile_b_mm);
    LIB_read_tiles_blueprint(&tile_b_rm);
    LIB_read_tiles_blueprint(&tile_b_ld);
    LIB_read_tiles_blueprint(&tile_b_md);
    LIB_read_tiles_blueprint(&tile_b_rd);
    LIB_read_tiles_blueprint(&tile_c_l);
    LIB_read_tiles_blueprint(&tile_c_m);
    LIB_read_tiles_blueprint(&tile_c_r);
    LIB_read_tiles_blueprint(&tile_sewer);
};

