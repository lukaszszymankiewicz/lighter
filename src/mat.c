#include <stdlib.h>

#include "mat.h"

array_t MAT_new(
    int rows, 
    int cols
) {
    array_t arr;
    arr.values = NULL;
    arr.values = (float*)malloc(sizeof(float) * rows * cols);

    for (int c=0; c<cols; cols++) { 
        for (int r=0; r<rows; rows++) { 
            arr.values[c*rows + r] = 0.0;
        }
    }

    return arr;
}

array_t MAT_scalar_new(
    float a1
) {
    array_t arr   = MAT_new(1, 1);
    arr.values[0] = a1;

    return arr;
}

array_t MAT_vec2_new(
    float a1, float a2
) {
    array_t arr   = MAT_new(2, 1);
    arr.values[0] = a1;
    arr.values[1] = a2;

    return arr;
}

array_t MAT_vec4_new(
    float a1, float a2, float a3, float a4
) {
    array_t arr  = MAT_new(4, 1);
    arr.values[0] = a1;
    arr.values[1] = a2;
    arr.values[2] = a3;
    arr.values[3] = a4;

    return arr;
}

array_t MAT_imat2_new(
    float a1, float a2
) {
    array_t arr   = MAT_new(2, 2);
    arr.values[0] = a1;
    arr.values[3] = a2;

    return arr;
}

void MAT_free(
    array_t arr
) {
    if (arr.values != NULL) {
        free(arr.values);
    }
    arr.values = NULL;
}

void MAT_join(
    array_t arr1,
    array_t arr2
) {
    int new_rows = arr1.rows + arr2.rows;
    int new_cols = arr1.cols;

    arr1.values = realloc(arr1.values, sizeof(float) * new_rows * new_cols);

    int i=0; 

    for (int c=0; c<new_cols; c++) {
        for (int r=0; r<arr1.rows; r++) {
            arr1.values[i++] = arr1.values[c*arr1.rows + r];
        }

        for (int r=0; r<arr2.rows; r++) {
            arr1.values[i++] = arr2.values[c*arr2.rows + r];
        }
    }
    // TODO: free here?
}
