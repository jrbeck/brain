#pragma once

#include <cstdio>
#include <cmath>

#define VEC3_DATA_TYPE double

// these guys prolly did it right...
// https://github.com/id-Software/DOOM-3/blob/master/neo/idlib/math/Vector.h

class Vec3 {
public:
  VEC3_DATA_TYPE x;
  VEC3_DATA_TYPE y;
  VEC3_DATA_TYPE z;

  Vec3();
  Vec3(VEC3_DATA_TYPE x, VEC3_DATA_TYPE y, VEC3_DATA_TYPE z);
  Vec3 zero();

  Vec3& operator=(const Vec3& a);
  VEC3_DATA_TYPE operator[](const int index) const;
  VEC3_DATA_TYPE& operator[](const int index);
  Vec3 operator-() const;
  VEC3_DATA_TYPE operator*(const Vec3& a) const;
  Vec3 operator*(const VEC3_DATA_TYPE a) const;
  Vec3 operator/(const VEC3_DATA_TYPE a) const;
  Vec3 operator+(const Vec3& a) const;
  Vec3 operator-(const Vec3& a) const;
  Vec3& operator+=(const Vec3& a);
  // Vec3& operator-=(const Vec3& a);
  // Vec3& operator/=(const Vec3& a);
  // Vec3& operator/=(const VEC3_DATA_TYPE a);
  // Vec3& operator*=(const VEC3_DATA_TYPE a);
  // friend Vec3 operator*(const VEC3_DATA_TYPE a, const Vec3 b);

  bool isEqual(const Vec3& a) const;
  bool isEqual(const Vec3& a, const VEC3_DATA_TYPE epsilon) const;
  bool operator==(const Vec3& a) const;
  bool operator!=(const Vec3& a) const;

  void print() const;
  void print(const char* label) const;
  void set(const VEC3_DATA_TYPE& value);
  VEC3_DATA_TYPE length() const;

  static VEC3_DATA_TYPE dist(const Vec3& a, const Vec3& b);
  Vec3 unit() const;
  void normalize();
  static Vec3 cross(const Vec3& a, const Vec3& b);
  Vec3 cross(const Vec3& other);
  VEC3_DATA_TYPE dot(const Vec3& other);
  Vec3 project(const Vec3& a, const Vec3& b);
  static Vec3 lerp(const Vec3& a, const Vec3& b, VEC3_DATA_TYPE t);
  Vec3 hadamard(const Vec3& a) const;
  // static Vec3 random(VEC3_DATA_TYPE length);
  // Vec3 getLookVector(VEC3_DATA_TYPE facing, VEC3_DATA_TYPE incline);
  // Vec3 rotateX(const Vec3& a, VEC3_DATA_TYPE angle);
  // Vec3 rotateY(const Vec3& a, VEC3_DATA_TYPE angle);
  // Vec3 rotateZ(const Vec3& a, VEC3_DATA_TYPE angle);

  void reflect(Vec3* normal, Vec3* result) const;
};
