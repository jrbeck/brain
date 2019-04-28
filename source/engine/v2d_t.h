#pragma once

#include <cstdio>
#include <cmath>

#define VEC2_DATA_TYPE float

// typedefs * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
union v2d_t {
  struct {
    VEC2_DATA_TYPE x;
    VEC2_DATA_TYPE y;
  };
  VEC2_DATA_TYPE vec[2];
};

union v2di_t {
  struct {
    int x;
    int y;
  };
  int vec[2];
};

// prototypes * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void v2d_print(const char* str, v2d_t a);
void v2di_print(const char* str, v2di_t a);
int v2d_isequal(v2d_t a, v2d_t b);
v2d_t v2d_zero();
void v2d_zero(v2d_t* a);
v2d_t v2d_unit(VEC2_DATA_TYPE theta);
v2d_t v2d_v(VEC2_DATA_TYPE x, VEC2_DATA_TYPE y);
v2di_t v2di_v(int x, int y);
VEC2_DATA_TYPE v2d_dist(v2d_t a, v2d_t b);
VEC2_DATA_TYPE v2d_relative_dist(v2d_t a, v2d_t b);
VEC2_DATA_TYPE v2d_mag(v2d_t a);
v2d_t v2d_normalize(v2d_t a);
v2d_t v2d_scale(v2d_t a, VEC2_DATA_TYPE scalar);
v2d_t v2d_scale(VEC2_DATA_TYPE scalar, v2d_t a);
v2d_t v2d_add(v2d_t a, v2d_t b);
v2di_t v2di_add(v2di_t a, v2di_t b);
v2d_t v2d_sub(v2d_t a, v2d_t b);
VEC2_DATA_TYPE v2d_dot(v2d_t a, v2d_t b);
VEC2_DATA_TYPE v2d_cross(v2d_t a, v2d_t b);
v2d_t v2d_rot(v2d_t a, VEC2_DATA_TYPE theta);
v2d_t v2d_neg(v2d_t a);
v2d_t v2d_normal(v2d_t a);
VEC2_DATA_TYPE v2d_theta(v2d_t a);
v2d_t v2d_project(v2d_t a, v2d_t b);
v2d_t v2d_interpolate(v2d_t a, v2d_t b, VEC2_DATA_TYPE percent);
