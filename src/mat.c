#include <stdlib.h>

vec2_t* MAT_vec2_new(
    float a1, float a2
) {
    vec2_t* vec  = (float*)malloc(sizeof(float) * 2);
    vec[0] = a1; vec[1] = a2;

    return vec;
}

imat2_t* MAT_imat2_new(
    float a1, float a2
) {
    imat2_t* mat = (float*)malloc(sizeof(float) * 4);
    vec[0] = a1; vec[1] = 0.0; vec[2] = a2; vec[3] = 0.0;

    return mat;
}

