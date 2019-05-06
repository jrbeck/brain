#pragma once

#include <cstdio>
#include <cmath>

typedef double VEC2_DATA_TYPE;

class Vec2 {
public:
  VEC2_DATA_TYPE x;
  VEC2_DATA_TYPE y;

  Vec2();
  Vec2(VEC2_DATA_TYPE x, VEC2_DATA_TYPE y);
  static Vec2 zero();

  Vec2& operator=(const Vec2& a);
  VEC2_DATA_TYPE operator[](const int index) const;
  VEC2_DATA_TYPE& operator[](const int index);
  Vec2 operator-() const;
  VEC2_DATA_TYPE operator*(const Vec2& a) const;
  Vec2 operator*(const VEC2_DATA_TYPE a) const;
  Vec2 operator/(const VEC2_DATA_TYPE a) const;
  Vec2 operator+(const Vec2& a) const;
  Vec2 operator-(const Vec2& a) const;
  Vec2& operator+=(const Vec2& a);
  Vec2& operator-=(const Vec2& a);
  // Vec2& operator/=(const Vec2& a);
  // Vec2& operator/=(const VEC2_DATA_TYPE a);
  Vec2& operator*=(const VEC2_DATA_TYPE a);
  friend Vec2 operator*(const VEC2_DATA_TYPE a, const Vec2 b);

  bool isEqual(const Vec2& a) const;
  bool isEqual(const Vec2& a, const VEC2_DATA_TYPE epsilon) const;
  bool operator==(const Vec2& a) const;
  bool operator!=(const Vec2& a) const;

  void print() const;
  void print(const char* label) const;
  void set(const VEC2_DATA_TYPE& value);
  VEC2_DATA_TYPE length() const;
  VEC2_DATA_TYPE ratio() const;

  static VEC2_DATA_TYPE distance(const Vec2& a, const Vec2& b);
  Vec2 unit() const;
  void normalize();
  VEC2_DATA_TYPE dot(const Vec2& other);
  static Vec2 project(const Vec2& a, const Vec2& b);
  static Vec2 lerp(const Vec2& a, const Vec2& b, VEC2_DATA_TYPE t);
  Vec2 hadamard(const Vec2& a) const;
  // static Vec2 random(VEC2_DATA_TYPE length);

  void reflect(Vec2* normal, Vec2* result) const;
};
