#ifndef MAT_H
#define MAT_H

typedef struct array {
    float* values;
    int    rows;
    int    cols;
    int    cap;
} array_t;

array_t MAT_new(int rows, int cols);
array_t MAT_vec2_new(float a1, float a2);
array_t MAT_imat2_new(float a1, float a2);
array_t MAT_scalar_new(float a1);
array_t MAT_vec4_new(float a1, float a2, float a3, float a4);

void MAT_join(array_t *arr1, array_t *arr2);
void MAT_append(array_t *arr1, array_t *arr2);
void MAT_free(array_t arr);
void MAT_debug(array_t arr);

#endif
