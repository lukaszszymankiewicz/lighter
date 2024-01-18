#include "library.h"
#include "../source.h"

wobble_t* wobble_library[WOBBLE_ALL] = { NULL };

wobble_t wobble_no = {
    0,
    { 0.0 }
};

wobble_t wobble_stable = {
    82,
    { 
        0.006, 0.006, 0.009, 0.009, 0.012,
        0.012, 0.017, 0.017, 0.022, 0.022,
        0.027, 0.027, 0.033, 0.033, 0.038,
        0.038, 0.042, 0.042, 0.045, 0.046,
        0.046, 0.045, 0.045, 0.042, 0.042,
        0.038, 0.038, 0.033, 0.033, 0.027,
        0.027, 0.022, 0.022, 0.017, 0.017,
        0.012, 0.012, 0.009, 0.009, 0.0,
        0.0, -0.006, -0.006, -0.009, -0.009,
        -0.012, -0.012, -0.017, -0.017, -0.022,
        -0.022, -0.027, -0.027, -0.033, -0.033,
        -0.038, -0.038, -0.042, -0.042, -0.045,
        -0.045, -0.046, -0.046, -0.045, -0.045,
        -0.042, -0.042, -0.038, -0.038, -0.033,
        -0.033, -0.027, -0.027, -0.022, -0.022,
        -0.017, -0.017, -0.012, -0.012, -0.009,
        -0.009, -0.009
    }
};

wobble_t wobble_walk = {
    15,
    { 
        0.012, 0.054, 0.082, 0.1, 0.082,
        0.054, 0.012, 0.0, -0.012, -0.054,
        -0.082, -0.1, -0.082, -0.054, -0.012,
    }
};


void LIB_read_wobble_blueprint(
    int       id,
    wobble_t* wobble
) {
    if (wobble) {
        printf("WOBBLE ID = %d read\n", id);
    }
    wobble_library[id]      = wobble;
};

void LIB_create_wobble_library(
) {
    LIB_read_wobble_blueprint(WOBBLE_NO, &wobble_no);
    LIB_read_wobble_blueprint(WOBBLE_STABLE, &wobble_stable);
    LIB_read_wobble_blueprint(WOBBLE_WALKING, &wobble_walk);
}
