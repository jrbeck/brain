#pragma once

#include "Vec2.h"

class Complex {
public:
  Complex();
  Complex(VEC2_DATA_TYPE r, VEC2_DATA_TYPE i);
  Complex(Vec2 value);

  Complex operator*(const VEC2_DATA_TYPE scalar) const;

  Complex operator+(const Complex& other) const;
  Complex operator*(const Complex& other) const;

  // this is purely for mandelbrot calculations
  void squareAndAdd(const Complex& other);

  VEC2_DATA_TYPE length() const;

  Vec2 mValue;
};

// class Complex {
//   VEC2_DATA_TYPE r;
//   VEC2_DATA_TYPE i;

//   Complex();
//   Complex(VEC2_DATA_TYPE r, VEC2_DATA_TYPE i);
//   static Complex zero();

//   Complex& operator=(const Complex& a);
//   VEC2_DATA_TYPE operator[](const int index) const;
//   VEC2_DATA_TYPE& operator[](const int index);
//   Complex operator-() const;
//   VEC2_DATA_TYPE operator*(const Complex& a) const;
//   Complex operator*(const VEC2_DATA_TYPE a) const;
//   Complex operator/(const VEC2_DATA_TYPE a) const;
//   Complex operator+(const Complex& a) const;
//   Complex operator-(const Complex& a) const;
//   Complex& operator+=(const Complex& a);
//   Complex& operator-=(const Complex& a);
//   // Complex& operator/=(const Complex& a);
//   // Complex& operator/=(const VEC2_DATA_TYPE a);
//   Complex& operator*=(const VEC2_DATA_TYPE a);
//   friend Complex operator*(const VEC2_DATA_TYPE a, const Complex b);

//   bool isEqual(const Complex& a) const;
//   bool isEqual(const Complex& a, const VEC2_DATA_TYPE epsilon) const;
//   bool operator==(const Complex& a) const;
//   bool operator!=(const Complex& a) const;

//   void print() const;
//   void print(const char* label) const;
//   void set(const VEC2_DATA_TYPE& value);
//   VEC2_DATA_TYPE length() const;
//   VEC2_DATA_TYPE ratio() const;

//   static VEC2_DATA_TYPE distance(const Complex& a, const Complex& b);
//   Complex unit() const;
//   void normalize();
//   VEC2_DATA_TYPE dot(const Complex& other);
//   static Complex project(const Complex& a, const Complex& b);
//   static Complex lerp(const Complex& a, const Complex& b, VEC2_DATA_TYPE t);
//   Complex hadamard(const Complex& a) const;
//   // static Complex random(VEC2_DATA_TYPE length);

//   void reflect(Complex* normal, Complex* result) const;
// };
