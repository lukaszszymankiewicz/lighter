#include <stdlib.h>
#include <stdio.h>

#include "mat.h"

array_t MAT_new(
    int rows, 
    int cols
) {
    array_t arr;
    arr.values = NULL;
    arr.values = (float*)malloc(sizeof(float) * rows * cols);

    for (int c=0; c<cols; c++) { 
        for (int r=0; r<rows; r++) { 
            arr.values[c*rows + r] = 0.0;
        }
    }

    arr.rows = rows;
    arr.cols = cols;
    arr.cap  = rows * cols;

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
    array_t *arr1,
    array_t *arr2
) {
    int new_rows = arr1->rows;
    int new_cols = arr1->cols + arr2->cols;

    float* values = NULL;
    values        = (float*)malloc(sizeof(float) * new_rows * new_cols);

    int i=0; 

    for (int r=0; r<new_rows; r++) {
        for (int c=0; c<arr1->cols; c++) {
            values[i++] = arr1->values[r*arr1->cols + c];
        }

        for (int c=0; c<arr2->cols; c++) {
            values[i++] = arr2->values[r*arr2->cols + c];
        }
    }
    free(arr1->values);
    free(arr2->values);

    arr1->values = NULL;
    arr2->values = NULL;

    arr1->rows   = new_rows;
    arr1->cols   = new_cols;
    arr1->values = values;
    arr1->cap    = new_cols * new_rows;
}

void MAT_append(
    array_t *arr1,
    array_t *arr2
) {
    int new_rows = arr1->rows + arr2->rows;
    int new_cols = arr1->cols;
    
    if (new_rows * new_cols > arr1->cap) {
        int new_cap  = (new_rows * new_cols) * 2;
        arr1->cap    = new_cap;
        arr1->values = (float*)realloc(arr1->values, sizeof(float) * new_cap);
    }

    int j=0; 

    for (int i=arr1->rows * arr1->cols; i<new_rows*new_cols; i++) {
        arr1->values[i] = arr2->values[j++];
    }

    arr1->rows   = new_rows;
    arr1->cols   = new_cols;
    
    // TODO: free here?
    // free(arr2->values);
    // arr2->values = NULL;
}

void MAT_empty(
    array_t arr
) {
    for (int r=0; r<arr.rows; r++) {
        for (int c=0; c<arr.cols; c++) {
            printf("%f ", arr.values[r*arr.cols + c]);
        }
        printf("\n");
    }
}
void MAT_debug(
    array_t arr
) {
    for (int r=0; r<arr.rows; r++) {
        for (int c=0; c<arr.cols; c++) {
            printf("%f ", arr.values[r*arr.cols + c]);
        }
        printf("\n");
    }
}
