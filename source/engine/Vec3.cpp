#include "Vec3.h"

Vec3::Vec3() {
}

Vec3::Vec3(VEC3_DATA_TYPE x, VEC3_DATA_TYPE y, VEC3_DATA_TYPE z) {
  this->x = x;
  this->y = y;
  this->z = z;
}

Vec3 Vec3::zero() {
  return Vec3(0, 0, 0);
}

Vec3& Vec3::operator=(const Vec3& a) {
  x = a.x;
  y = a.y;
  z = a.z;
  return *this;
}

VEC3_DATA_TYPE Vec3::operator[](const int index) const {
  return (&x)[index];
}

VEC3_DATA_TYPE& Vec3::operator[](const int index) {
  return (&x)[index];
}

Vec3 Vec3::operator-() const {
  return Vec3(-x, -y, -z);
}

VEC3_DATA_TYPE Vec3::operator*(const Vec3& a) const {
  return (x * a.x) + (y * a.y) + (z * a.z);
}

Vec3 Vec3::operator*(const VEC3_DATA_TYPE a) const {
  return Vec3(a * x, a * y, a * z);
}

Vec3 Vec3::operator/(const VEC3_DATA_TYPE a) const {
  VEC3_DATA_TYPE invA = (VEC3_DATA_TYPE)1.0f / a;
  return Vec3(invA * x, invA * y, invA * z);
}

Vec3 Vec3::operator+(const Vec3& a) const {
  return Vec3(x + a.x, y + a.y, z + a.z);
}

Vec3 Vec3::operator-(const Vec3& a) const {
  return Vec3(x - a.x, y - a.y, z - a.z);
}

Vec3& Vec3::operator+=(const Vec3 &a) {
  x += a.x;
  y += a.y;
  z += a.z;
  return *this;
}

// Vec3& Vec3::operator-=(const Vec3 &a);
// Vec3& Vec3::operator/=(const Vec3 &a);
// Vec3& Vec3::operator/=(const VEC3_DATA_TYPE a);
// Vec3& Vec3::operator*=(const VEC3_DATA_TYPE a);
// friend Vec3 Vec3::operator*(const VEC3_DATA_TYPE a, const Vec3 b);

bool Vec3::isEqual(const Vec3& a) const {
  return (x == a.x) && (y == a.y) && (z == a.z);
}
bool Vec3::isEqual(const Vec3& a, const VEC3_DATA_TYPE epsilon) const {
  return (fabs(x - a.x) < epsilon) &&
    (fabs(y - a.y) < epsilon) &&
    (fabs(z - a.z) < epsilon);
}
bool Vec3::operator==(const Vec3& a) const {
  return isEqual(a);
}
bool Vec3::operator!=(const Vec3& a) const {
  return !isEqual(a);
}

void Vec3::print() const {
  print(nullptr);
}

void Vec3::print(const char* label) const {
  if (label == nullptr) {
    printf("Vec3: <%.2f, %.2f, %.2f>\n", x, y, z);
  } else {
    printf("%s <%.3f, %.3f, %.3f>\n", label, x, y, z);
  }
}

void Vec3::set(const VEC3_DATA_TYPE& value) {
  x = y = z = value;
}

VEC3_DATA_TYPE Vec3::length() const {
  return sqrt((x * x) + (y * y) + (z * z));
}

VEC3_DATA_TYPE Vec3::dist(const Vec3& a, const Vec3& b) {
  VEC3_DATA_TYPE d_x = a.x - b.x;
  VEC3_DATA_TYPE d_y = a.y - b.y;
  VEC3_DATA_TYPE d_z = a.z - b.z;
  return sqrt((d_x * d_x) + (d_y * d_y) + (d_z * d_z));
}

Vec3 Vec3::unit() const {
  VEC3_DATA_TYPE len = (x * x) + (y * y) + (z * z);
  // this is probably not the right way to handle this
  if (len <= 0.0) return Vec3(1, 0, 0);
  len = 1.0 / sqrt(len);
  return Vec3(x * len, y * len, z * len);
}

void Vec3::normalize() {
  VEC3_DATA_TYPE len = (x * x) + (y * y) + (z * z);
  // this is probably not the right way to handle this
  if (len <= 0.0) return;
  len = 1.0 / sqrt(len);
  x *= len;
  y *= len;
  z *= len;
}

Vec3 Vec3::cross(const Vec3& a, const Vec3& b) {
  return Vec3(
    (a.y * b.z) - (a.z * b.y),
    (a.z * b.x) - (a.x * b.z),
    (a.x * b.y) - (a.y * b.x)
  );
}

Vec3 Vec3::cross(const Vec3& other) {
  return Vec3(
    (y * other.z) - (z * other.y),
    (z * other.x) - (x * other.z),
    (x * other.y) - (y * other.x)
  );
}

VEC3_DATA_TYPE Vec3::dot(const Vec3& other) {
  return (x * other.x) + (y * other.y) + (z * other.z);
}

Vec3 Vec3::project(const Vec3& a, const Vec3& b) {
  VEC3_DATA_TYPE bsq = (b.x * b.x) + (b.y * b.y) + (b.z * b.z);
  // i can't deal with this! bail!
  if (bsq == 0.0) {
    return Vec3::zero();
  }

  VEC3_DATA_TYPE dot = (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
  // so: dot = DOT(b) / |b|^2
  dot /= bsq;

  return Vec3(dot * b.x, dot * b.y, dot * b.z);
}

Vec3 Vec3::lerp(const Vec3& a, const Vec3& b, VEC3_DATA_TYPE t) {
  return Vec3(
    a.x + ((b.x - a.x) * t),
    a.y + ((b.y - a.y) * t),
    a.z + ((b.z - a.z) * t)
  );
}

Vec3 Vec3::hadamard(const Vec3& a) const {
  return Vec3(
    x * a.x,
    y * a.y,
    z * a.z
  );
}

// Vec3 Vec3::random(VEC3_DATA_TYPE length) {
//   Vec3 ret;
//
//   ret.x = r_num(-10.0, 10.0);
//   ret.y = r_num(-10.0, 10.0);
//   ret.z = r_num(-10.0, 10.0);
//
//   return ret.unit() * length;
// }

// Vec3 Vec3::getLookVector(VEC3_DATA_TYPE facing, VEC3_DATA_TYPE incline) {
//   Vec3 lookVector;
//
//   lookVector.x = cos(incline);
//   lookVector.y = sin(incline);
//   lookVector.z = 0.0;
//
//   return Vec3::rotateY(lookVector, -facing);
// }
//
// Vec3 Vec3::rotateX(Vec3 a, VEC3_DATA_TYPE angle) {
//   Vec3 r;
//
//   VEC3_DATA_TYPE s = sin(angle);
//   VEC3_DATA_TYPE c = cos(angle);
//
//   r.x = a.x;
//   r.y = (a.y * c) - (a.z * s);
//   r.z = (a.y * s) + (a.z * c);
//
//   return r;
// }
//
// Vec3 Vec3::rotateY(Vec3 a, VEC3_DATA_TYPE angle) {
//   Vec3 r;
//
//   VEC3_DATA_TYPE s = sin(angle);
//   VEC3_DATA_TYPE c = cos(angle);
//
//   r.x = (a.z * s) + (a.x * c);
//   r.y = a.y;
//   r.z = (a.z * c) - (a.x * s);
//
//   return r;
// }
//
// Vec3 Vec3::rotateZ(Vec3 a, VEC3_DATA_TYPE angle) {
//   Vec3 r;
//
//   VEC3_DATA_TYPE s = sin(angle);
//   VEC3_DATA_TYPE c = cos(angle);
//
//   r.x = (a.x * c) - (a.y * s);
//   r.y = (a.x * s) + (a.y * c);
//   r.z = a.z;
//
//   return r;
// }


// assumption: normal is a unit vector
void Vec3::reflect(Vec3* normal, Vec3* result) const {
  *result = *this - (*normal * ((*this * *normal) * 2.0));
}
