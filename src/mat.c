#include <stdlib.h>

float* MAT_scalar_new(
    float a1
) {
    float* scalar  = (float*)malloc(sizeof(float) * 1);
    scalar[0]      = a1;

    return scalar;
}

float* MAT_vec2_new(
    float a1, float a2
) {
    float* vec  = (float*)malloc(sizeof(float) * 2);
    vec[0] = a1; vec[1] = a2;

    return vec;
}

float* MAT_imat2_new(
    float a1, float a2
) {
    float* mat = (float*)malloc(sizeof(float) * 4);
    mat[0] = a1; mat[1] = 0.0; mat[2] = 0.0; mat[3] = a2;

    return mat;
}
