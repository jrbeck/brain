#include "Vec2.h"

Vec2::Vec2() {
}

Vec2::Vec2(const Vec2& other) {
  x = other.x;
  y = other.y;
}

Vec2::Vec2(VEC2_DATA_TYPE xVal, VEC2_DATA_TYPE yVal) {
  x = xVal;
  y = yVal;
}

Vec2 Vec2::zero() {
  return Vec2(0, 0);
}

Vec2& Vec2::operator=(const Vec2& other) {
  x = other.x;
  y = other.y;
  return *this;
}

VEC2_DATA_TYPE Vec2::operator[](const int index) const {
  return (&x)[index];
}

VEC2_DATA_TYPE& Vec2::operator[](const int index) {
  return (&x)[index];
}

Vec2 Vec2::operator-() const {
  return Vec2(-x, -y);
}

VEC2_DATA_TYPE Vec2::operator*(const Vec2& a) const {
  return (x * a.x) + (y * a.y);
}

Vec2 Vec2::operator*(const VEC2_DATA_TYPE a) const {
  return Vec2(a * x, a * y);
}

Vec2 Vec2::operator/(const VEC2_DATA_TYPE a) const {
  VEC2_DATA_TYPE invA = (VEC2_DATA_TYPE)1.0f / a;
  return Vec2(invA * x, invA * y);
}

Vec2 Vec2::operator+(const Vec2& a) const {
  return Vec2(x + a.x, y + a.y);
}

Vec2 Vec2::operator-(const Vec2& a) const {
  return Vec2(x - a.x, y - a.y);
}

Vec2& Vec2::operator+=(const Vec2 &a) {
  x += a.x;
  y += a.y;
  return *this;
}

Vec2& Vec2::operator-=(const Vec2 &a) {
  x -= a.x;
  y -= a.y;
  return *this;
}

// Vec2& Vec2::operator/=(const Vec2 &a);
// Vec2& Vec2::operator/=(const VEC2_DATA_TYPE a);

Vec2& Vec2::operator*=(const VEC2_DATA_TYPE a) {
  x *= a;
  y *= a;
  return *this;
}

Vec2 operator*(const VEC2_DATA_TYPE a, const Vec2 b) {
  return b * a;
}

bool Vec2::isEqual(const Vec2& a) const {
  return (x == a.x) && (y == a.y);
}
bool Vec2::isEqual(const Vec2& a, const VEC2_DATA_TYPE epsilon) const {
  return (fabs(x - a.x) < epsilon) && (fabs(y - a.y) < epsilon);
}
bool Vec2::operator==(const Vec2& a) const {
  return isEqual(a);
}
bool Vec2::operator!=(const Vec2& a) const {
  return !isEqual(a);
}

void Vec2::print() const {
  print(nullptr);
}

void Vec2::print(const char* label) const {
  if (label == nullptr) {
    printf("Vec2: <%.5f, %.5f>\n", x, y);
  } else {
    printf("%s <%.5f, %.5f>\n", label, x, y);
  }
}

void Vec2::set(const VEC2_DATA_TYPE& value) {
  x = y = value;
}

VEC2_DATA_TYPE Vec2::length() const {
  return sqrt((x * x) + (y * y));
}

VEC2_DATA_TYPE Vec2::ratio() const {
  if (y == 0.0) {
    return 0.0;
  }
  return x / y;
}

VEC2_DATA_TYPE Vec2::distance(const Vec2& a, const Vec2& b) {
  VEC2_DATA_TYPE d_x = a.x - b.x;
  VEC2_DATA_TYPE d_y = a.y - b.y;
  return sqrt((d_x * d_x) + (d_y * d_y));
}

Vec2 Vec2::unit() const {
  VEC2_DATA_TYPE len = (x * x) + (y * y);
  // this is probably not the right way to handle this
  if (len <= 0.0) return Vec2(1, 0);
  len = 1.0 / sqrt(len);
  return Vec2(x * len, y * len);
}

void Vec2::normalize() {
  VEC2_DATA_TYPE len = (x * x) + (y * y);
  // this is probably not the right way to handle this
  if (len <= 0.0) return;
  len = 1.0 / sqrt(len);
  x *= len;
  y *= len;
}

VEC2_DATA_TYPE Vec2::dot(const Vec2& other) {
  return (x * other.x) + (y * other.y);
}

Vec2 Vec2::project(const Vec2& a, const Vec2& b) {
  VEC2_DATA_TYPE bsq = (b.x * b.x) + (b.y * b.y);
  // i can't deal with this! bail!
  if (bsq == 0.0) {
    return Vec2::zero();
  }

  VEC2_DATA_TYPE dot = (a.x * b.x) + (a.y * b.y);
  // so: dot = DOT(b) / |b|^2
  dot /= bsq;

  return Vec2(dot * b.x, dot * b.y);
}

Vec2 Vec2::lerp(const Vec2& a, const Vec2& b, VEC2_DATA_TYPE t) {
  return Vec2(
    a.x + ((b.x - a.x) * t),
    a.y + ((b.y - a.y) * t)
  );
}

Vec2 Vec2::hadamard(const Vec2& a) const {
  return Vec2(
    x * a.x,
    y * a.y
  );
}

// Vec2 Vec2::random(VEC2_DATA_TYPE length) {
//   Vec2 ret;
//
//   ret.x = r_num(-10.0, 10.0);
//   ret.y = r_num(-10.0, 10.0);
//
//   return ret.unit() * length;
// }

// assumption: normal is a unit vector
void Vec2::reflect(Vec2* normal, Vec2* result) const {
  *result = *this - (*normal * ((*this * *normal) * 2.0));
}
