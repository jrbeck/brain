#include "v2d_t.h"

void v2d_print(const char* str, v2d_t a) {
  printf ("%s <%.3f, %.3f>\n", str, a.x, a.y);
}

void v2di_print(const char* str, v2di_t a) {
  printf ("%s <%6d, %6d>\n", str, a.x, a.y);
}

// return 1 if equal, 0 otherwise
int v2d_isequal(v2d_t a, v2d_t b) {
  if (a.x == b.x && a.y == b.y) return 1;
  return 0;
}

// returns a vector of magnitude zero
v2d_t v2d_zero() {
  v2d_t a;
  a.x = 0.0f;
  a.y = 0.0f;
  return a;
}

// zeroes out an existing vector
void v2d_zero(v2d_t* a) {
  a->x = 0.0f;
  a->y = 0.0f;
}

// returns unit vector at angle theta
v2d_t v2d_unit(VEC2_DATA_TYPE theta) {
  v2d_t a;
  a.x = cos(theta);
  a.y = sin(theta);
  return a;
}

// returns a v2d_t
v2d_t v2d_v(VEC2_DATA_TYPE x, VEC2_DATA_TYPE y) {
  v2d_t a;
  a.x = x;
  a.y = y;
  return a;
}

// returns a v2di_t
v2di_t v2di_v(int x, int y) {
  v2di_t a;
  a.x = x;
  a.y = y;
  return a;
}

// returns the distance between two points
VEC2_DATA_TYPE v2d_dist(v2d_t a, v2d_t b) {
  a.x -= b.x;
  a.y -= b.y;
  return (sqrt ((a.x * a.x) + (a.y * a.y)));
}

// returns the relative distance between two points
VEC2_DATA_TYPE v2d_relative_dist(v2d_t a, v2d_t b) {
  a.x -= b.x;
  a.y -= b.y;
  return (a.x * a.x) + (a.y * a.y);
}

// returns the magnitude of the vector
VEC2_DATA_TYPE v2d_mag(v2d_t a) {
  return (sqrt ((a.x * a.x) + (a.y * a.y)));
}

// returns the unit vector in the same direction as a
v2d_t v2d_normalize(v2d_t a) {
  VEC2_DATA_TYPE len = sqrt ((a.x * a.x) + (a.y * a.y));

  if (len <= 0.0f) return a;

  a.x /= len;
  a.y /= len;
  return a;
}

// returns scalar * a (s is scalar, a is vector)
v2d_t v2d_scale(v2d_t a, VEC2_DATA_TYPE scalar) {
  a.x *= scalar;
  a.y *= scalar;
  return a;
}

v2d_t v2d_scale(VEC2_DATA_TYPE scalar, v2d_t a) {
  a.x *= scalar;
  a.y *= scalar;
  return a;
}

// returns a + b
v2d_t v2d_add(v2d_t a, v2d_t b) {
  a.x += b.x;
  a.y += b.y;
  return a;
}

v2di_t v2di_add(v2di_t a, v2di_t b) {
  a.x += b.x;
  a.y += b.y;
  return a;
}

// returns a - b
v2d_t v2d_sub(v2d_t a, v2d_t b) {
  a.x -= b.x;
  a.y -= b.y;
  return a;
}

// returns a dot b (|a|*|b|*cos (theta))
VEC2_DATA_TYPE v2d_dot(v2d_t a, v2d_t b) {
  return ((a.x * b.x) + (a.y * b.y));
}

// returns a cross b (sorta)
VEC2_DATA_TYPE v2d_cross(v2d_t a, v2d_t b) {
  return ((a.x * b.y) - (a.y * b.x));
}

// returns a vevtor rotated theta radians around the origin
v2d_t v2d_rot(v2d_t a, VEC2_DATA_TYPE theta) {
  v2d_t b;
  b.x = (cos (theta) * a.x) - (sin (theta) * a.y);
  b.y = (sin (theta) * a.x) + (cos (theta) * a.y);
  return b;
}

// returns the opposite vector
v2d_t v2d_neg(v2d_t a) {
  a.x = -a.x;
  a.y = -a.y;
  return a;
}

// returns a perpendicular vector
v2d_t v2d_normal(v2d_t a) {
  VEC2_DATA_TYPE t = a.x;
  a.x = a.y;
  a.y = -t;
  return a;
}

// returns the angle off the x-axis
VEC2_DATA_TYPE v2d_theta(v2d_t a) {
  return atan2(a.y, a.x);
}

// returns the projection of a onto b
v2d_t v2d_project(v2d_t a, v2d_t b) {
  v2d_t p;
  VEC2_DATA_TYPE dot = (a.x * b.x) + (a.y * b.y); // dot product of a and b
  VEC2_DATA_TYPE bsq = (b.x * b.x) + (b.y * b.y); // |b|^2

  // i can't deal with this! bail!
  if (bsq == 0.0) return v2d_zero ();

  // so: dot = DOT (b) / (b.x * b.x + b.y * b.y)
  dot /= bsq;

  p.x = dot * b.x;
  p.y = dot * b.y;

  return p;
}

// returns the v2d that is located {percent} of the distance from a to b
v2d_t v2d_interpolate(v2d_t a, v2d_t b, VEC2_DATA_TYPE percent) {
  v2d_t ret;
  ret.x = a.x + ((b.x - a.x) * percent);
  ret.y = a.y + ((b.y - a.y) * percent);
  return ret;
}
