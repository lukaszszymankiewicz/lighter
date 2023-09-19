#ifndef MAT_H
#define MAT_H

typedef struct vec2 {
  float[2]  val;
} vec2_t;

typedef struct imat2 {
  float[2][2]  val;
} imat2_t;

vec2_t* MAT_vec2_new(float a1, float a2);
imat2_t* MAT_imat2_new(float a1, float a2);

#endif
